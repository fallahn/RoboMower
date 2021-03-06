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

//main state of the game

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include <StateIds.hpp>
#include <InstructionSet.hpp>
#include <GameUI.hpp>

#include <xygine/State.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Scene.hpp>
#include <xygine/Resource.hpp>
#include <xygine/ui/Window.hpp>
#include <xygine/network/ClientConnection.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <map>

namespace sf
{
    class Color;
}

class GameState final : public xy::State
{
public:
    GameState(xy::StateStack& stateStack, Context context);
    ~GameState() = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const xy::Message&) override;
    xy::StateID stateID() const override
    {
        return States::ID::Game;
    }
private:

    xy::MessageBus& m_messageBus;
    xy::Scene m_scene;

    xy::TextureResource m_textureResource;
    xy::FontResource m_fontResource;

    GameUI m_gameUI;
    xy::Network::ClientConnection m_connection;
    bool m_programFinished;

    std::map<xy::ClientID, xy::Entity*> m_playerEntities;

    xy::Network::ClientConnection::PacketHandler m_packetHandler;
    void handlePacket(xy::Network::PacketType type, sf::Packet& packet, xy::Network::ClientConnection* connection);

    void buildMap();
    void sendProgram();
};

#endif //GAME_STATE_HPP_