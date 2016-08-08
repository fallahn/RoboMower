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

#ifndef RM_PLAYER_DRAWABLE_HPP_
#define RM_PLAYER_DRAWABLE_HPP_

#include <xygine/components/Component.hpp>
#include <NetProtocol.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

class PlayerDrawable final : public xy::Component, public sf::Drawable, public sf::Transformable
{
public:
    PlayerDrawable(xy::MessageBus&, sf::Texture&, bool local);
    PlayerDrawable() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;
    void setDirection(Direction);

private:

    Direction m_direction;
    std::vector<sf::Vector2f> m_origins;
    std::vector<sf::Vertex> m_upSprite;
    std::vector<sf::Vertex> m_downSprite;
    std::vector<sf::Vertex> m_leftSprite;
    std::vector<sf::Vertex> m_rightSprite;

    sf::Vertex* m_currentSprite;
    std::size_t m_vertexCount;
    sf::Texture& m_texture;
    
    std::size_t m_jiggleIndex;

    void createSprites(bool local);
    void buildSprite(const sf::Vector2f&, const sf::Vector2f&, Direction);
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif //RM_PLAYER_DRAWABLE_HPP_