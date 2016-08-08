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

#ifndef RM_TILEMAP_HPP_
#define RM_TILEMAP_HPP_

#include <xygine/components/Component.hpp>

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

namespace picojson
{
    class value;
}

class Tilemap final : public xy::Component, public sf::Drawable
{
public:
    Tilemap(xy::MessageBus&, sf::Texture&);
    ~Tilemap() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;



private:
    enum Tile
    {
        ShortGrassLight,
        ShortGrassDark,
        FenceTopLeft,
        FenceTop,
        FenceTopRight,
        LongGrass,
        Dirt,
        FenceBottomLeft,
        FenceBottom,
        FenceBottomRight,
        FenceLeft,
        FenceRight,
        EdgeNorth,
        EdgeEast,
        EdgeSouth,
        EdgeWest,
        EdgeNorthEast,
        EdgeSouthEast,
        EdgeSouthWest,
        EdgeNorthWest,
        FlowersOne,
        FlowersTwo,
        FlowersThree,
        FlowersFour,
        RockOne,
        RockTwo,
        RockThree,
        Count
    };
    static std::vector<sf::Vector2f> tilePositions;

    sf::Texture& m_texture;
    std::vector<sf::Vertex> m_borderArray;

    std::vector<sf::Vertex> m_lawnArray;


    void loadJson();
    void getValue(const std::string&, const picojson::value&, Tilemap::Tile);
    
    void buildMap();
    void addTile(float x, float y, Tile, std::vector<sf::Vertex>&);

    void draw(sf::RenderTarget&, sf::RenderStates) const;

};

#endif //RM_TILEMAP_HPP_