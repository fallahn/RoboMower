//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <Music.hpp>
#include <Log.hpp>

#include <cassert>
#include <algorithm>

MusicPlayer::MusicPlayer()
    : m_volume      (100.f)
{

}

void MusicPlayer::play(const std::string& title, bool loop)
{
    if (m_music.openFromFile(title))
    {
        m_music.setVolume(m_volume);
        m_music.setLoop(loop);
        m_music.play();
    }
    else
    {
        //TODO this probably warns anyway
        LOG("failed to open " + title, Logger::Type::Error);
    }
}

void MusicPlayer::stop()
{
    m_music.stop();
}

void MusicPlayer::setPaused(bool paused)
{
    (paused) ? m_music.pause() : m_music.play();
}

bool MusicPlayer::playing() const
{
    return (m_music.getStatus() == sf::SoundSource::Playing);
}

void MusicPlayer::setVolume(float volume)
{
    assert(volume >= 0.f && volume <= 100.f);

    m_volume = volume;
    if(m_music.getStatus() == sf::Music::Playing)
        m_music.setVolume(volume);
}

float MusicPlayer::getVolume() const
{
    return m_volume;
}