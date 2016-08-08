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
    m_direction     (Direction::Right),
    m_origins       (4),
    m_currentSprite (nullptr),
    m_vertexCount   (0),
    m_texture       (texture),
    m_jiggleIndex   (0u)
{
    createSprites(local);
    setDirection(Direction::Right);
}

//public
void PlayerDrawable::entityUpdate(xy::Entity& ent, float dt)
{
    //animate origin for jiggle when motor is on
    auto origin = m_origins[static_cast<int>(m_direction)];
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
    case Direction::Up:
        m_currentSprite = m_upSprite.data();
        m_vertexCount = m_upSprite.size();
        break;
    case Direction::Down:
        m_currentSprite = m_downSprite.data();
        m_vertexCount = m_downSprite.size();
        break;
    case Direction::Left:
        m_currentSprite = m_leftSprite.data();
        m_vertexCount = m_leftSprite.size();
        break;
    case Direction::Right:
        m_currentSprite = m_rightSprite.data();
        m_vertexCount = m_rightSprite.size();
        break;
    default: break;
    }
    setOrigin(m_origins[static_cast<int>(dir)]);
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
                buildSprite(handle, body, Direction::Up);

                //down sprite
                handle = getValue("handle_d", rootValue);
                body = (local) ? getValue("player1_d", rootValue) : getValue("player2_d", rootValue);
                buildSprite(handle, body, Direction::Down);

                //right sprite
                handle = getValue("handle_h", rootValue);
                body = (local) ? getValue("player1_h", rootValue) : getValue("player2_h", rootValue);
                buildSprite(handle, body, Direction::Right);

                //left sprite - we can get this by flipping tex coords of right sprite :)
                buildSprite(handle, body, Direction::Left);
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
    case Direction::Up:
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
        m_origins[static_cast<int>(Direction::Up)] = { tileWidth * 0.5f, tileHeight * 0.5f };
        break;
    case Direction::Down:
        m_downSprite.emplace_back(sf::Vertex({}, handle));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { handle.x + tileSize.x, handle.y }));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, handle + tileSize));
        m_downSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, { handle.x, handle.y + tileSize.y }));

        m_downSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, body));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { body.x + tileSize.x, body.y }));
        m_downSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight * 2.f }, body + tileSize));
        m_downSprite.emplace_back(sf::Vertex({ 0.f, tileHeight * 2.f }, { body.x, body.y + tileSize.y }));
        m_origins[static_cast<int>(Direction::Down)] = { tileWidth * 0.5f, tileHeight * 1.5f };
        break;
    case Direction::Right:
        m_rightSprite.emplace_back(sf::Vertex({}, handle));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { handle.x + tileSize.x, handle.y }));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, handle + tileSize));
        m_rightSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, { handle.x, handle.y + tileSize.y }));

        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, body));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, 0.f }, { body.x + tileSize.x, body.y }));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, tileHeight }, body + tileSize));
        m_rightSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { body.x, body.y + tileSize.y }));
        m_origins[static_cast<int>(Direction::Right)] = { tileWidth * 1.5f, tileHeight * 0.5f };
        break;
    case Direction::Left:
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, { handle.x + tileSize.x, handle.y }));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, 0.f }, handle));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth * 2.f, tileHeight }, {handle.x, handle.y + tileSize.y }));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, handle + tileSize));

        m_leftSprite.emplace_back(sf::Vertex({}, { body.x + tileSize.x, body.y }));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, 0.f }, body));
        m_leftSprite.emplace_back(sf::Vertex({ tileWidth, tileHeight }, { body.x, body.y + tileSize.y }));
        m_leftSprite.emplace_back(sf::Vertex({ 0.f, tileHeight }, body + tileSize));
        m_origins[static_cast<int>(Direction::Left)] = { tileWidth * 0.5f, tileHeight * 0.5f };
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