//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//plays music as requested by the current state

#ifndef MUSIC_HPP_
#define MUSIC_HPP_

#include <SFML/Audio/Music.hpp>

#include <string>

class MusicPlayer final : private sf::NonCopyable
{
public:
    MusicPlayer();
    ~MusicPlayer() = default;
    MusicPlayer(const MusicPlayer&) = delete;
    const MusicPlayer& operator = (const MusicPlayer&) = delete;

    void play(const std::string& file, bool loop = false);
    void stop();
    void setPaused(bool paused);
    bool playing() const;
    void setVolume(float volume);
    float getVolume() const;

    void update(float);

private:

    float m_volume;
    sf::Music m_music;
};


#endif //MUSIC_HPP_