//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <components/Tilemap.hpp>

#include <xygine/util/Json.hpp>
#include <xygine/util/Random.hpp>
#include <xygine/parsers/picojson.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <fstream>
#include <bitset>
#include <functional>

namespace
{
    const sf::Vector2f tileSize(16.f, 16.f);
    //tiles are drawn 4x larger than the tile set size
    const float tileWidth = 64.f;
    const float tileHeight = 64.f;

    const sf::Uint8 tileCountX = 20u;
    const sf::Uint8 tileCountY = 14u;

    const sf::Uint8 borderTop = 2u;
    const sf::Uint8 borderLeft = 3u;
}
//actually we probably only need to store positions
std::vector<sf::Vector2f> Tilemap::tilePositions(Tilemap::Count);

Tilemap::Tilemap(xy::MessageBus& mb, sf::Texture& texture)
    : xy::Component (mb, this),
    m_texture       (texture)
{
    //shouldn't really be loading stuff in a ctor
    //but buns to it. We'll just fail gracefully.
    loadJson();
    buildMap();
}

//public
void Tilemap::entityUpdate(xy::Entity&, float) {}

//private
void Tilemap::loadJson()
{
    std::ifstream file("assets/images/tileset.tst");
    if (file.good() && xy::Util::File::validLength(file))
    {
        std::string jsonString;
        std::string temp;
        while (!file.eof())
        {
            file >> temp;
            jsonString += temp;
        }
        if (!jsonString.empty())
        {
            picojson::value rootValue;
            auto err = picojson::parse(rootValue, jsonString);
            if (err.empty())
            {
                getValue("short1", rootValue, Tile::ShortGrassLight);
                getValue("short2", rootValue, Tile::ShortGrassDark);
                getValue("fence_tl", rootValue, Tile::FenceTopLeft);
                getValue("fence_top", rootValue, Tile::FenceTop);
                getValue("fence_tr", rootValue, Tile::FenceTopRight);
                getValue("long", rootValue, Tile::LongGrass);
                getValue("dirt", rootValue, Tile::Dirt);
                getValue("fence_bl", rootValue, Tile::FenceBottomLeft);
                getValue("fence_bottom", rootValue, Tile::FenceBottom);
                getValue("fence_br", rootValue, Tile::FenceBottomRight);
                getValue("fence_left", rootValue, Tile::FenceLeft);
                getValue("fence_right", rootValue, Tile::FenceRight);
                getValue("edge_n", rootValue, Tile::EdgeNorth);
                getValue("edge_e", rootValue, Tile::EdgeEast);
                getValue("edge_s", rootValue, Tile::EdgeSouth);
                getValue("edge_w", rootValue, Tile::EdgeWest);
                getValue("edge_ne", rootValue, Tile::EdgeNorthEast);
                getValue("edge_se", rootValue, Tile::EdgeSouthEast);
                getValue("edge_sw", rootValue, Tile::EdgeSouthWest);
                getValue("edge_nw", rootValue, Tile::EdgeNorthWest);
                getValue("flower1", rootValue, Tile::FlowersOne);
                getValue("flower2", rootValue, Tile::FlowersTwo);
                getValue("flower3", rootValue, Tile::FlowersThree);
                getValue("flower4", rootValue, Tile::FlowersFour);
                getValue("rock1", rootValue, Tile::RockOne);
                getValue("rock2", rootValue, Tile::RockTwo);
                getValue("rock3", rootValue, Tile::RockThree);
            }
            else
            {
                xy::Logger::log("tileset data: " + err, xy::Logger::Type::Error, xy::Logger::Output::All);
            }
        }
        else
        {
            xy::Logger::log("tileset data file is empty", xy::Logger::Type::Error, xy::Logger::Output::All);
        }
    }
    else
    {
        //always write this, and write to log file so we can see this in release builds
        xy::Logger::log("failed to open tileset data file.", xy::Logger::Type::Error, xy::Logger::Output::All);
    }
    file.close();
}

void Tilemap::getValue(const std::string& name, const picojson::value& source, Tilemap::Tile tile)
{
    if (source.get(name).is<picojson::array>())
    {
        auto arr = source.get(name).get<picojson::array>();
        sf::Vector2f pos
            (
                arr[0].is<double>() ? static_cast<float>(arr[0].get<double>()) : 0.f,
                arr[1].is<double>() ? static_cast<float>(arr[1].get<double>()) : 0.f
                );
        tilePositions[tile] = pos;
    }
}

void Tilemap::buildMap()
{
    //used to create details
    std::bitset<tileCountX*tileCountY> bs(0);
    
    //create border
    for (auto y = 0u; y < tileCountY; ++y)
    {
        for (auto x = 0u; x < tileCountX; ++x)
        {
            //we need to place tiles from top layer down
            
            //top fence
            if ((y == borderTop - 1) && (x >= borderLeft && x < tileCountX - borderLeft))
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeNorth, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceTop, m_borderArray);
                continue;
            }
            //bottom fence
            else if ((y == tileCountY - borderTop) && (x >= borderLeft && x < tileCountX - borderLeft))
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeSouth, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceBottom, m_borderArray);
                continue;
            }
            //left fence
            else if ((x == borderLeft - 1) && (y >= borderTop && y < tileCountY - borderTop))
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeWest, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceLeft, m_borderArray);
                continue;
            }
            //right fence
            else if ((x == tileCountX - borderLeft) && (y >= borderTop && y < tileCountY - borderTop))
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeEast, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceRight, m_borderArray);
                continue;
            }

            //tl
            else if (x == borderLeft - 1 && y == borderTop - 1)
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeNorthWest, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceTopLeft, m_borderArray);
                continue;
            }
            //tr
            else if (x == tileCountX - borderLeft && y == borderTop - 1)
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeNorthEast, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceTopRight, m_borderArray);
                continue;
            }
            //br
            else if (x == tileCountX - borderLeft && y == tileCountY - borderTop)
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeSouthEast, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceBottomRight, m_borderArray);
                continue;
            }
            //bl
            else if (x == borderLeft - 1 && y == tileCountY - borderTop)
            {
                addTile(x * tileWidth, y * tileHeight, Tile::EdgeSouthWest, m_borderArray);
                addTile(x * tileWidth, y * tileHeight, Tile::FenceBottomLeft, m_borderArray);
                continue;
            }

            //dirt border
            else if ((y < borderTop - 1 || y > tileCountY - borderTop)
                    || (x < borderLeft - 1 || x > tileCountX - borderLeft))
            {
                addTile(x * tileWidth, y * tileHeight, Tile::Dirt, m_borderArray);

                //decide if this tile should have detail on it
                static const int proabability = 85;
                if (xy::Util::Random::value(0, 100) > proabability)
                {
                    bs[(y * tileCountX) + x] = 1;
                }
            }
        }
    }

    //smooth the bitset by essentially using a convolution blur
    std::function<int(std::size_t)> getNeighbours = [&bs](std::size_t idx)
    {
        std::size_t retVal = 0;
        sf::Vector2u coord(idx % tileCountX, idx / tileCountX);

        for (auto x = coord.x - 1; x <= coord.x + 1; ++x)
        {
            for (auto y = coord.y - 1; y <= coord.y + 1; ++y)
            {
                if (x >= 0 && x < tileCountX && y >= 0 && y < tileCountY)
                {
                    if (x != coord.x || y != coord.y)
                    {
                        retVal += bs[tileCountX * y + x];
                    }
                }
                else
                {
                    retVal++;
                }
            }
        }

        return retVal;
    };
    for (auto i = 0u; i < 5; ++i)
    {
        for (auto j = 0u; j < bs.size(); ++j)
        {
            auto count = getNeighbours(j);
            if (count < 4) bs[i] = 1;
            else if (count > 4) bs[i] = 0;
        }
    }

    //add the detail tiles
    int tile = 0;
    std::vector<int> usedTiles = { 0 };
    for (auto i = 0u; i < bs.size(); ++i)
    {
        if (bs[i] == 1)
        {
            auto x = i % tileCountX;
            auto y = i / tileCountX;
            
            while (std::find(usedTiles.begin(), usedTiles.end(), tile) != usedTiles.end())
            {
                tile = xy::Util::Random::value(Tile::FlowersOne, Tile::RockThree);
            }
            usedTiles.push_back(tile);
            addTile(x * tileWidth, y * tileHeight, static_cast<Tile>(tile), m_borderArray);

            if (usedTiles.size() == (Tile::RockThree - Tile::FlowersOne))
            {
                usedTiles = { 0 };
            }
        }
    }

    //load lawn data. build this row/column order so we can set specific tiles as needed
    for (auto y = borderTop; y < tileCountY - borderTop; ++y)
    {
        for (auto x = borderLeft; x < tileCountX - borderLeft; ++x)
        {
            addTile(x * tileWidth, y * tileHeight, Tile::LongGrass, m_lawnArray);
        }
    }
}

void Tilemap::addTile(float x, float y, Tile tile, std::vector<sf::Vertex>& vertArray)
{
    vertArray.emplace_back(sf::Vertex({ x, y }, { tilePositions[tile].x, tilePositions[tile].y }));
    vertArray.emplace_back(sf::Vertex({ x + tileWidth, y }, { tilePositions[tile].x + tileSize.x, tilePositions[tile].y }));
    vertArray.emplace_back(sf::Vertex({ x + tileWidth, y + tileHeight }, { tilePositions[tile].x + tileSize.x, tilePositions[tile].y + tileSize.y }));
    vertArray.emplace_back(sf::Vertex({ x, y + tileHeight }, { tilePositions[tile].x, tilePositions[tile].y + tileSize.y}));
}

void Tilemap::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.texture = &m_texture;
    rt.draw(m_borderArray.data(), m_borderArray.size(), sf::Quads, states);
    rt.draw(m_lawnArray.data(), m_lawnArray.size(), sf::Quads, states);
}