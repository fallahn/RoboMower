//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <GameServer.hpp>
#include <NetProtocol.hpp>
#include <Messages.hpp>

#include <xygine/Entity.hpp>
#include <components/PlayerLogic.hpp>

namespace
{
    const float snapshotInterval = 1 / 20.f;
}

using namespace std::placeholders;

GameServer::GameServer()
    : m_scene(m_messageBus)
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
        packet << PacketID(DirectionUpdate);
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

void GameServer::removePlayer(ClientID id)
{

}

void GameServer::sendSnapshot()
{
    sf::Packet packet;
    packet << PacketID(PacketIdent::PositionUpdate);
    packet << sf::Uint8(m_players.size());
    for (const auto& p : m_players)
    {
        auto position = p.entity->getPosition();
        packet << p.id << position.x << position.y;
    }
    m_connection.broadcast(packet);
}

void GameServer::handlePacket(const sf::IpAddress& ip, PortNumber port, Network::PacketType type, sf::Packet& packet, Network::ServerConnection* connection)
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

        //*robustly* receive program, need to include instruction count?

        //delete player on disconnect
    }
}