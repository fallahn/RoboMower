//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <components/PlayerDrawable.hpp>

#include <xygine/Entity.hpp>
#include <xygine/util/Json.hpp>
#include <xygine/util/Random.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <functional>

namespace
{
    const sf::Vector2f tileSize(16.f, 16.f);
    //tiles are drawn 4x larger than the tile set size
    const float tileWidth = 64.f;
    const float tileHeight = 64.f;

    const std::vector<float> jiggles = 
    {
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f),
        xy::Util::Random::value(-1.f, 1.f)
    };
}

PlayerDrawable::PlayerDrawable(xy::MessageBus& mb, sf::Texture& texture, bool local)
    : xy::Component (mb, this),
    m_direction     (Right),
    m_origins       (Count),
    m_currentSprite (nullptr),
    m_vertexCount   (0),
    m_texture       (texture),
    m_jiggleIndex   (0u)
{
    createSprites(local);
    setDirection(Right);
}

//public
void PlayerDrawable::entityUpdate(xy::Entity& ent, float dt)
{
    //animate origin for jiggle when motor is on
    auto origin = m_origins[m_direction];
    origin.x += jiggles[m_jiggleIndex];
    m_jiggleIndex = (m_jiggleIndex + 1) % jiggles.size();
    origin.y += jiggles[m_jiggleIndex];
    m_jiggleIndex = (m_jiggleIndex + 1) % jiggles.size();
    setOrigin(origin);

    //TODO when we rotate can we blend between sprites?
}

void PlayerDrawable::setDirection(Direction dir)
{
    switch (dir)
    {
    case Up:
        m_currentSprite = m_upSprite.data();
        m_vertexCount = m_upSprite.size();
        break;
    case Down:
        m_currentSprite = m_downSprite.data();
        m_vertexCount = m_downSprite.size();
        break;
    case Left:
        m_currentSprite = m_leftSprite.data();
        m_vertexCount = m_leftSprite.size();
        break;
    case Right:
        m_currentSprite = m_rightSprite.data();
        m_vertexCount = m_rightSprite.size();
        break;
    default: break;
    }
    setOrigin(m_origins[dir]);
    m_direction = dir;
}

//private
void PlayerDrawable::createSprites(bool local)
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
                std::function<sf::Vector2f(const std::string&, const picojson::value&)> getValue =
                    [](const std::string& name, const picojson::value& source)
                {
                    if (source.get(name).is<picojson::array>())
                    {
                        auto arr = source.get(name).get<picojson::array>();
                        sf::Vector2f pos
                            (
                                arr[0].is<double>() ? static_cast<float>(arr[0].get<double>()) : 0.f,
                                arr[1].is<double>() ? static_cast<float>(arr[1].get<double>()) : 0.f
                                );
                        return pos;
                    }
                    return sf::Vector2f();
                };

                //up sprite
                auto handle = getValue("handle_u", rootValue);
                auto body = (local) ? getValue("player1_u", rootValue) : getValue("player2_u", rootValue);
                buildSprite(handle, body, Up);

                //down sprite
                handle = getValue("handle_d", rootValue);
                body = (local) ? getValue("player1_d", rootValue) : getValue("player2_d", rootValue);
                buildSprite(handle, body, Down);

                //right sprite
                handle = getValue("handle_h", rootValue);
                body = (local) ? getValue("player1_h", rootValue) : getValue("player2_h", rootValue);
                buildSprite(handle, body, Right);

                //left sprite - we can get this by flipping tex coords of right sprite :)
                buildSprite(handle, body, Left);
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

void PlayerDrawable::buildSprite(const sf::Vector2f& handle, const sf::Vector2f& body, Direction dir)
{
    switch (dir)
    {
    case Up:
        //handle
        m_upSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, handle));
        m_upSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { handle.x + tileSize.x, handle.y }));
        m_upSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight * 2.f }, handle + tileSize));
        m_upSprite.emplace_back(sf::Vertex({ 0.f, tileHeight * 2.f }, { handle.x, handle.y + tileSize.y }));
        //body
        m_upSprite.emplace_back(sf::Vertex({}, body));
        m_upSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { body.x + tileSize.x, body.y }));
        m_upSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, body + tileSize));
        m_upSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, { body.x, body.y + tileSize.y }));
        m_origins[Up] = { tileWidth * 0.5f, tileHeight * 0.5f };
        break;
    case Down:
        m_downSprite.emplace_back(sf::Vertex({}, handle));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { handle.x + tileSize.x, handle.y }));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, handle + tileSize));
        m_downSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, { handle.x, handle.y + tileSize.y }));

        m_downSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, body));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { body.x + tileSize.x, body.y }));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight * 2.f }, body + tileSize));
        m_downSprite.emplace_back(sf::Vertex({ 0.f, tileHeight * 2.f }, { body.x, body.y + tileSize.y }));
        m_origins[Down] = { tileWidth * 0.5f, tileHeight * 1.5f };
        break;
    case Right:
        m_rightSprite.emplace_back(sf::Vertex({}, handle));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { handle.x + tileSize.x, handle.y }));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, handle + tileSize));
        m_rightSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, { handle.x, handle.y + tileSize.y }));

        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, body));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, 0.f }, { body.x + tileSize.x, body.y }));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, tileHeight }, body + tileSize));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { body.x, body.y + tileSize.y }));
        m_origins[Right] = { tileWidth * 1.5f, tileHeight * 0.5f };
        break;
    case Left:
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { handle.x + tileSize.x, handle.y }));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, 0.f }, handle));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, tileHeight }, {handle.x, handle.y + tileSize.y }));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, handle + tileSize));

        m_leftSprite.emplace_back(sf::Vertex({}, { body.x + tileSize.x, body.y }));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, body));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { body.x, body.y + tileSize.y }));
        m_leftSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, body + tileSize));
        m_origins[Left] = { tileWidth * 0.5f, tileHeight * 0.5f };
        break;
    default: break;
    }
}

void PlayerDrawable::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_texture;
    rt.draw(m_currentSprite, m_vertexCount, sf::Quads, states);
}