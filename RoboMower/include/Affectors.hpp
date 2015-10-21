//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//affectors for particle systems

#ifndef AFFECTORS_HPP_
#define AFFECTORS_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

struct Particle;
struct ForceAffector
{
    explicit ForceAffector(const sf::Vector2f& force);
    void operator()(Particle& p, float dt);
    void setRandom(const sf::Vector2f& rangeStart, const sf::Vector2f& rangeEnd);

private:
    sf::Vector2f m_force;

    bool m_random;
    sf::Vector2f m_randomStart;
    sf::Vector2f m_randomEnd;
};

struct ColourAffector
{
    ColourAffector(const sf::Color& start, const sf::Color& end, float duration);
    void operator() (Particle& p, float dt);

private:
    float m_duration;
    sf::Color m_start;
    sf::Color m_end;
};

struct RotateAffector
{
    explicit RotateAffector(float rotation);
    void operator() (Particle& p, float dt);

private:
    float m_rotation;
};

struct ScaleAffector
{
    explicit ScaleAffector(const sf::Vector2f& scale);
    void operator() (Particle& p, float dt);
private:
    sf::Vector2f m_scale;
};
#endif //AFFECTORS_HPP_