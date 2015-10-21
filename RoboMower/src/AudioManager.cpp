//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <AudioManager.hpp>
#include <MessageBus.hpp>
#include <Util.hpp>
#include <Log.hpp>
#include <FileSystem.hpp>

#include <SFML/Audio/Listener.hpp>

namespace
{
    const float maxMusicVolume = 100.f;
    float musicVolume = 100.f;
    const float maxFxVolume = 85.f;
    float fxVolume = 60.f;

    float fadeDelay = 2.f; //delay starting the fade while the initial state is loaded
    const std::string impactSoundPath = "";
}

AudioManager::AudioManager()
    : m_fadeInTime      (4.f),
    m_currentFadeTime   (0.f),
    m_fxSounds          (SoundIds::Size),
    m_muted             (false)
{   
    m_musicPlayer.setVolume(0.f);
    //m_musicPlayer.play("assets/sound/background.ogg", true);

    m_soundPlayer.setVolume(0.f);
    //auto files = FileSystem::listFiles(impactSoundPath);
    //for (const auto& file : files)
    //{
    //    if (FileSystem::getFileExtension(file) == ".wav")
    //    {
    //        /*m_impactSounds.emplace_back(sf::SoundBuffer());
    //        m_impactSounds.back().loadFromFile(impactSoundPath + file);*/
    //    }
    //}
}


//public
void AudioManager::update(float dt)
{
    if (fadeDelay > 0)
    {
        fadeDelay -= dt;
    }
    else if (m_currentFadeTime < m_fadeInTime)
    {
        float ratio = std::min(m_currentFadeTime / m_fadeInTime, 1.f);
        m_musicPlayer.setVolume(musicVolume * ratio);
        m_soundPlayer.setVolume(fxVolume * ratio);
        m_currentFadeTime += dt;
    }

    m_soundPlayer.update();
}

void AudioManager::handleMessage(const Message& msg)
{
    //You has no seagulls.
    switch (msg.type)
    {
    case Message::Type::Physics:
        switch (msg.physics.event)
        {
        case Message::PhysicsEvent::Collision:
            if (m_muted) break;
                
                break;
        default:break;
        }
        break;
    case Message::Type::Entity:
        if (!m_muted )
        {

        }
        break;
    case Message::Type::UI:
        switch (msg.ui.type)
        {
        case Message::UIEvent::RequestVolumeChange:
            musicVolume = std::min(maxMusicVolume * msg.ui.value, maxMusicVolume);
            fxVolume = std::min(maxFxVolume * msg.ui.value, maxFxVolume);

            if (fadeDelay <= 0)
            {
                m_musicPlayer.setVolume(musicVolume);
                m_soundPlayer.setVolume(fxVolume);
            }

            break;
        case Message::UIEvent::RequestAudioMute:
            m_muted = true;
            m_musicPlayer.setPaused(true);
            break;
        case Message::UIEvent::RequestAudioUnmute:
            if (m_muted)
            {
                m_muted = false;
                m_musicPlayer.setPaused(false);
            }
            break;
        default: break;
        }
        break;
    case Message::Type::Player:
        switch (msg.player.action)
        {
        case Message::PlayerEvent::Spawned:

            break;
        default:break;
        }
        break;
    default: break;
    }
}

void AudioManager::mute(bool m)
{
    m_muted = m;
    m_musicPlayer.setPaused(m);
}