//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <GameServer.hpp>

namespace
{
    const float snapshotInterval = 1 / 20.f;
}

using namespace std::placeholders;

GameServer::GameServer()
    : m_scene(m_messageBus)
{
    m_packetHandler = std::bind(&GameServer::handlePacket, this, _1, _2, _3, _4, _5);
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
void GameServer::setup()
{


}

void GameServer::addPlayer()
{

}

void GameServer::sendSnapshot()
{

}

void GameServer::handlePacket(const sf::IpAddress& ip, PortNumber port, Network::PacketType type, sf::Packet& packet, Network::ServerConnection* connection)
{
    //create player on join
    //*robustly* receive program, need to check acks
    //delete player on disconnect
}