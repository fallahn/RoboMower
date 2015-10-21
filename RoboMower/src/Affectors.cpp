//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <Affectors.hpp>
#include <ParticleSystem.hpp>
#include <Util.hpp>

namespace
{
    sf::Uint8 lerp(sf::Uint8 start, sf::Uint8 end, float amount)
    {
        float val = static_cast<float>(end - start) * amount;
        return start + static_cast<sf::Uint8>(val);
    }
}

//------------------------------------
ForceAffector::ForceAffector(const sf::Vector2f& force) 
    : m_force   (force),
    m_random    (false){}

void ForceAffector::operator() (Particle& p, float dt)
{
    if (m_random)
    {
        m_force.x = Util::Random::value(m_randomStart.x, m_randomEnd.x);
        m_force.y = Util::Random::value(m_randomStart.y, m_randomEnd.y);
    }

    p.velocity += m_force * dt;
}

void ForceAffector::setRandom(const sf::Vector2f& rangeStart, const sf::Vector2f& rangeEnd)
{
    assert(rangeStart.x < rangeEnd.x && rangeStart.y < rangeEnd.y);
    
    m_randomStart = rangeStart;
    m_randomEnd = rangeEnd;
    m_random = true;
}

//------------------------------------
ColourAffector::ColourAffector(const sf::Color& start, const sf::Color& end, float duration)
    : m_duration(duration), m_start(start), m_end(end){}

void ColourAffector::operator() (Particle& p, float dt)
{
    float ratio = (m_duration - p.lifetime) / m_duration;
    if (ratio > 1.f) ratio = 1.f;
    if (ratio < 0.f) ratio = 0.f;

    p.colour.r = lerp(m_start.r, m_end.r, ratio);
    p.colour.g = lerp(m_start.g, m_end.g, ratio);
    p.colour.b = lerp(m_start.b, m_end.b, ratio);
}

//------------------------------------
RotateAffector::RotateAffector(float rotation)
    : m_rotation(rotation){}

void RotateAffector::operator() (Particle& p, float dt)
{
    p.rotate(m_rotation * dt);
}

//------------------------------------
ScaleAffector::ScaleAffector(const sf::Vector2f& scale)
    : m_scale(scale){}

void ScaleAffector::operator()(Particle& p, float dt)
{
    p.setScale(p.getScale() + (m_scale *  dt));
}