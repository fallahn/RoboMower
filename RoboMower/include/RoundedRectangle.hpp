/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

//creates a rounded rectangle drawable

#ifndef ROUND_RECT_HPP_
#define ROUND_RECT_HPP_

#include <xygine/components/Component.hpp>
#include <xygine/ShaderProperty.hpp>

#include <SFML/Graphics/Shape.hpp>

namespace xy
{
    class MessageBus;
}

class RoundedRectangle final : public sf::Shape
{
public:
    RoundedRectangle(const sf::Vector2f& size = sf::Vector2f(8.f, 8.f), float radius = 8.f);
    ~RoundedRectangle() = default;
    RoundedRectangle(const RoundedRectangle&) = delete;
    RoundedRectangle& operator = (const RoundedRectangle&) = delete;

    void setSize(const sf::Vector2f&);
    void setRadius(float);
    const sf::Vector2f& getSize() const;
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t) const override;

private:
    sf::Vector2f m_size;
    float m_cornerRadius;
    sf::Vector2f m_minimumSize;

    void clampSize();
};

#endif //ROUND_RECT_HPP_