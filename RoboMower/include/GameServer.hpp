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

#ifndef RM_GAME_SERVER_HPP_
#define RM_GAME_SERVER_HPP_

#include <xygine/network/ServerConnection.hpp>

#include <xygine/Scene.hpp>
#include <xygine/MessageBus.hpp>

namespace xy
{
    class Entity;
}

class GameServer final
{
public:
    GameServer();
    ~GameServer() = default;

    GameServer(const GameServer&) = delete;
    GameServer& operator = (const GameServer&) = delete;

    bool start();
    void stop();
    void update(float);

private:
    struct Player final
    {
        std::string name;
        xy::ClientID id = -1;
        xy::Entity* entity = nullptr;
    };
    std::vector<Player> m_players;

    xy::MessageBus m_messageBus; //TODO server should be encapsulated and have its own messages, right?
    xy::Scene m_scene;

    xy::Network::ServerConnection m_connection;
    xy::Network::ServerConnection::PacketHandler m_packetHandler;
    sf::Clock m_snapshotClock;
    float m_snapshotAccumulator;

    void handleMessage(const xy::Message&);

    void setup();
    void addPlayer(Player&);
    void removePlayer(xy::ClientID);
    void sendSnapshot();

    void handlePacket(const sf::IpAddress&, xy::PortNumber, xy::Network::PacketType, sf::Packet&, xy::Network::ServerConnection*);
};

#endif //RM_GAME_SERVER_HPP_