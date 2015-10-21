//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//plays a given sound effect

#ifndef SOUND_PLAYER_HPP_
#define SOUND_PLAYER_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <map>
#include <list>

class SoundPlayer final
{
public:

    SoundPlayer();
    ~SoundPlayer() = default;
    SoundPlayer(const SoundPlayer&) = delete;
    const SoundPlayer& operator = (const SoundPlayer&) = delete;

    void update();
    void play(const sf::SoundBuffer&, bool = false);
    
    void setListenerPosition(const sf::Vector2f& position);
    sf::Vector2f getListenerPosition() const;

    static void setVolume(float volume);
    static float getVolume();

private:

    std::list<sf::Sound> m_sounds;

    void flushSounds();
};

#endif //SOUND_PLAYER_HPP_