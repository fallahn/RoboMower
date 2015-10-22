//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef AUDIO_MANAGER_HPP_
#define AUDIO_MANAGER_HPP_

#include <xygine/SoundPlayer.hpp>
#include <xygine/Music.hpp>

class Message;
class AudioManager final
{
public:
    AudioManager();
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    const AudioManager& operator = (const AudioManager&) = delete;

    void update(float);
    void handleMessage(const Message&);

    void mute(bool);

private:

    float m_fadeInTime;
    float m_currentFadeTime;

    MusicPlayer m_musicPlayer;
    SoundPlayer m_soundPlayer;

    enum SoundIds
    {
        Size
    };

    std::vector<sf::SoundBuffer> m_impactSounds;
    std::vector<sf::SoundBuffer> m_fxSounds;

    bool m_muted;
};

#endif //AUDIO_MANAGER_HPP_