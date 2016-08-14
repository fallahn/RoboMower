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

//displays a handle for loop instructions to display jump destination

#ifndef LOOP_HANDLE_HPP_
#define LOOP_HANDLE_HPP_

#include <xygine/components/Component.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

class LoopHandle final : public xy::Component, public sf::Drawable, public sf::Transformable
{
public:
    LoopHandle(xy::MessageBus&, const sf::Texture&, float verticalSpacing);
    ~LoopHandle() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;

    sf::FloatRect globalBounds() const override;
    sf::FloatRect localBounds() const override;

    void setEnabled(bool);

    std::size_t getSize() const;
    void setSize(std::size_t);

    void setMousePosition(const sf::Vector2f& mp) { m_mousePosition = getTransform().getInverse().transformPoint(mp); }
    void setMaxSize(std::size_t size) { m_maxSize = size; }

private:

    const sf::Texture& m_texture;
    std::vector<sf::Vertex> m_vertices;

    sf::FloatRect m_localBounds;
    sf::FloatRect m_globalBounds;

    float m_verticalSpacing;
    float m_vertexSpacing;

    bool m_enabled;
    std::size_t m_size;

    sf::Vector2f m_mousePosition;
    sf::FloatRect m_mouseArea;
    bool m_mouseDown;
    std::size_t m_maxSize;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif //LOOP_HANDLE_HPP_