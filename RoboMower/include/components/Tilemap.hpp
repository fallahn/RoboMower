//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

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