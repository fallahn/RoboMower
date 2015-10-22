/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

xygine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace xy
{
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
}
#endif //PARTICLE_HPP_