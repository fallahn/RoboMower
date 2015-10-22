//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/SoundPlayer.hpp>

#include <SFML/Audio/Listener.hpp>

#include <cmath>
#include <cassert>

namespace
{
    const float listenerDepth = 900.f;
    const float attenuation = 1.f;
    const float minDistance2D = 700.f; //bear in mind listener won't be moving and we want to hear everything on screen
    const float minDistance3D = std::sqrt((minDistance2D * minDistance2D) + (listenerDepth * listenerDepth));

    float volume = 60.f; //static value as it is shared by all sound players

    const std::size_t maxSounds = 160u;
}

SoundPlayer::SoundPlayer()
{

}

//public
void SoundPlayer::update()
{
    flushSounds();
}

void SoundPlayer::play(const sf::SoundBuffer& sb, bool loop)
{
    if (m_sounds.size() < maxSounds)
    {
        m_sounds.emplace_back();

        auto& sound = m_sounds.back();
        sound.setBuffer(sb);
        sound.setVolume(volume);
        sound.setLoop(loop);
        sound.play();
    }
}

void SoundPlayer::setListenerPosition(const sf::Vector2f& position)
{
    sf::Listener::setPosition(position.x, -position.y, listenerDepth);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    auto pos = sf::Listener::getPosition();
    return{ pos.x, -pos.y };
}

void SoundPlayer::setVolume(float vol)
{
    volume = vol;
}

float SoundPlayer::getVolume()
{
    return volume;
}

//private
void SoundPlayer::flushSounds()
{
    m_sounds.remove_if([](const sf::Sound& s){return (s.getStatus() == sf::Sound::Stopped); });
}
