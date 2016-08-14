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

#include <components/LoopHandle.hpp>

#include <xygine/Entity.hpp>
#include <xygine/Reports.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace
{
    const sf::Color hoverColour(10u, 230u, 10u);
}

LoopHandle::LoopHandle(xy::MessageBus& mb, const sf::Texture& texture, float verticalSpacing)
    : xy::Component     (mb, this),
    m_texture           (texture),
    m_verticalSpacing   (verticalSpacing),
    m_vertexSpacing     (0.f),
    m_enabled           (false),
    m_size              (0)
{
    //this assumes the texture is square and can be divided into a 3x3 grid
    //so that the vertex array can be stretched horizonatally and vertically
    auto texSize = static_cast<sf::Vector2f>(texture.getSize());
    m_vertexSpacing = texSize.y / 3.f;

    m_vertices = 
    {
        {{0.f, 0.f},{0.f, 0.f}},                                       {{texSize.x, 0.f}, {texSize.x, 0.f}},
        {{0.f, -m_vertexSpacing }, {0.f, m_vertexSpacing }},           {{texSize.x, -m_vertexSpacing }, {texSize.x, m_vertexSpacing }},
        {{0.f, -m_vertexSpacing * 2.f}, {0.f, m_vertexSpacing * 2.f}}, {{texSize.x, -m_vertexSpacing * 2.f}, {texSize.x, m_vertexSpacing * 2.f}},
        {{0.f, -texSize.y}, {0.f, texSize.y}},                         {{texSize.x, -texSize.y}, texSize}
    };

    setSize(1);
}

//public
void LoopHandle::entityUpdate(xy::Entity& entity, float)
{
    m_globalBounds = entity.getWorldTransform().transformRect(m_localBounds);

    //REPORT("Mouse Position", "x: " + std::to_string(m_mousePosition.x) + ", y: " + std::to_string(m_mousePosition.y));

    if (m_mouseArea.contains(m_mousePosition))
    {
        for (auto& v : m_vertices)
        {
            v.color = hoverColour;
        }
    }
    else
    {
        for (auto& v : m_vertices)
        {
            v.color = sf::Color::White;
        }
    }
}

sf::FloatRect LoopHandle::globalBounds() const
{
    return m_globalBounds;
}

sf::FloatRect LoopHandle::localBounds() const
{
    return m_localBounds;
}

void LoopHandle::setEnabled(bool en)
{
    m_enabled = en;
}

std::size_t LoopHandle::getSize() const
{
    return (m_enabled) ? m_size : 0;
}

void LoopHandle::setSize(std::size_t size)
{
    //update vert array
    float height = size * m_verticalSpacing + m_vertexSpacing;
    m_vertices[4].position.y = -height;
    m_vertices[5].position.y = -height;
    height += m_vertexSpacing;
    m_vertices[6].position.y = -height;
    m_vertices[7].position.y = -height;

    //update localBounds
    m_localBounds = {m_vertices.front().position, m_vertices.back().position};
    setOrigin(m_localBounds.width + /*borderThickness*/6.f, -(m_verticalSpacing / 2.f));
    m_size = size;

    //update mouse grabbing area
    m_mouseArea = m_localBounds;
    m_mouseArea.height /= 3.f;
    m_mouseArea.top += m_mouseArea.height * 2.f;
}

//private
void LoopHandle::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    if (m_enabled)
    {
        states.transform *= getTransform();
        states.texture = &m_texture;
        rt.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::TrianglesStrip, states);
    }
}