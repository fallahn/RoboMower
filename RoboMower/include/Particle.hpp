//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct Particle final : public sf::Transformable
{
    enum class Type
    {
        Trail,
        Echo,
        Sparkle,
        Ident
    };

    sf::Vector2f velocity;
    sf::Color colour;
    float rotation = 0.f;
    float lifetime = 0.f;
    sf::FloatRect textureRect;
};

#endif //PARTICLE_HPP_