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

#include <GameServer.hpp>
#include <NetProtocol.hpp>
#include <Messages.hpp>
#include <TransportStatus.hpp>

#include <xygine/Entity.hpp>
#include <components/PlayerLogic.hpp>

namespace
{
    const float snapshotInterval = 1 / 20.f;
    const sf::Uint8 MAX_PROGRAM_SIZE = 255;
}

using namespace std::placeholders;

GameServer::GameServer()
    : m_scene(m_messageBus),
    m_connection(m_messageBus)
{
    m_packetHandler = std::bind(&GameServer::handlePacket, this, _1, _2, _3, _4, _5);
    m_connection.setPacketHandler(m_packetHandler);
    setup();
}

//public
bool GameServer::start()
{
    return m_connection.start();
}

void GameServer::stop()
{
    m_connection.stop();
}

void GameServer::update(float dt)
{
    while (!m_messageBus.empty())
    {
        const auto& msg = m_messageBus.poll();
        handleMessage(msg);
        m_scene.handleMessage(msg);
    }

    m_scene.update(dt);
    m_connection.update(dt);

    m_snapshotAccumulator += m_snapshotClock.restart().asSeconds();
    while (m_snapshotAccumulator >= snapshotInterval)
    {
        m_snapshotAccumulator -= snapshotInterval;
        sendSnapshot();
    }
}

//private
void GameServer::handleMessage(const xy::Message& msg)
{
    switch (msg.id)
    {
    case DirectionMessage:
    {
        auto& msgData = msg.getData<DirectionEvent>();
        sf::Packet packet;
        packet << xy::PacketID(DirectionUpdate);
        packet << msgData.id << sf::Uint8(msgData.direction);
        m_connection.broadcast(packet);
    }
        break;
    default: break;
    }
}

void GameServer::setup()
{


}

void GameServer::addPlayer(Player& player)
{
    //create entity for scene
    auto pl = xy::Component::create<PlayerLogic>(m_messageBus);
    pl->setClientID(player.id);

    auto entity = xy::Entity::create(m_messageBus);
    entity->addComponent(pl);
    entity->setPosition(224.f, 160.f); //TODO broadcast this to client from map properties
    player.entity = m_scene.addEntity(entity, xy::Scene::Layer::BackFront);

    m_players.push_back(player);
    LOG("SERVER - Adding player " + player.name, xy::Logger::Type::Info);

    //TODO broadcast to all clients
}

void GameServer::removePlayer(xy::ClientID id)
{

}

void GameServer::sendSnapshot()
{
    sf::Packet packet;
    packet << xy::PacketID(PacketIdent::PositionUpdate);
    packet << sf::Uint8(m_players.size());
    for (const auto& p : m_players)
    {
        auto position = p.entity->getPosition();
        packet << p.id << position.x << position.y;
    }
    m_connection.broadcast(packet);
}

void GameServer::handlePacket(const sf::IpAddress& ip, xy::PortNumber port, xy::Network::PacketType type, sf::Packet& packet, xy::Network::ServerConnection* connection)
{
    switch (type)
    {
    default: break;
        //create player on join
    case PacketIdent::PlayerDetails:
    {
        Player player;
        packet >> player.id;
        packet >> player.name;
        addPlayer(player);

        //TODO reply with map properties
    }
        break;

        //receive program
    case PacketIdent::TransmitProgram:
        //TODO assert byte stream is correct size
        //and send request for program again if not
    {
        xy::ClientID clid;
        sf::Uint32 size;
        packet >> clid >> size;
        if (size > 0 && size < MAX_PROGRAM_SIZE)
        {
            sf::Uint8 byte;
            std::vector<sf::Uint8> program;
            while (packet >> byte)
            {
                program.push_back(byte);
            }

            if (program.size() != size)
            {
                //failed transmission, send request for program again
            }
            else
            {
                //find player, set program if they exist
                auto player = std::find_if(m_players.begin(), m_players.end(),
                    [clid](const Player& p) 
                {
                    return p.id == clid;
                });
                if (player != m_players.end())
                {
                    player->entity->getComponent<PlayerLogic>()->setProgram(program);
                    player->entity->getComponent<PlayerLogic>()->start();
                    LOG("SERVER: set program for player " + std::to_string(clid), xy::Logger::Type::Info);

                    sf::Packet response; //TODO operator overload
                    //response << xy::Network::PacketType(TransportStateChanged) << clid << TransportStatus::Playing;
                    //m_connection.send(response, true);
                }
            }
        }
    }
        break;

        //delete player on disconnect
    }
}