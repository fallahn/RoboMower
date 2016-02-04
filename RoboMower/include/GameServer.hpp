//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_GAME_SERVER_HPP_
#define RM_GAME_SERVER_HPP_

#include <network/ServerConnection.hpp>

#include <xygine/Scene.hpp>
#include <xygine/MessageBus.hpp>

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
    xy::MessageBus m_messageBus; //TODO server should be encapsulated and have its own messges, right?
    xy::Scene m_scene;

    Network::ServerConnection m_connection;
    Network::ServerConnection::PacketHandler m_packetHandler;
    sf::Clock m_snapshotClock;
    float m_snapshotAccumulator;

    void setup();
    void addPlayer();
    void sendSnapshot();

    void handlePacket(const sf::IpAddress&, PortNumber, Network::PacketType, sf::Packet&, Network::ServerConnection*);
};

#endif //RM_GAME_SERVER_HPP_