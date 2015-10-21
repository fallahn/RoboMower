//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//game server class

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <Entity.hpp>
#include <Command.hpp>
#include <MessageBus.hpp>
#include <Scene.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <queue>

class GameServer final
{
public:
    GameServer();
    ~GameServer();

    GameServer(const GameServer&) = delete;
    GameServer& operator = (const GameServer&) = delete;

    static sf::Uint16 getPort();
    void setPort(sf::Uint16);

    void setMaxPlayers(sf::Uint32);

private:
    struct RemoteConnection
    {
        using Ptr = std::unique_ptr<RemoteConnection>;
        RemoteConnection();

        sf::TcpSocket socket;
        float lastPacketTime;
        bool ready;
        bool timeout;
        sf::Int16 uid;
        bool inWorld;
    };

    sf::Thread m_thread;
    sf::Clock m_clock;
    sf::TcpListener m_listener;
    bool m_listening;
    float m_clientTimeoutTime;
    sf::Uint32 m_maxPlayers;
    sf::Uint32 m_connectedPlayers;
    std::vector<RemoteConnection::Ptr> m_connections;
    bool m_waitingThreadEnd;
    sf::Mutex m_mutex;

    sf::Vector2f m_worldSize;
    MessageBus m_messageBus;
    Scene m_scene;

    void setListening(bool);
    void executionThread();
    void update(float);
    void tick();
    float now() const;

    void handlePackets();
    void handlePacket(sf::Packet&, RemoteConnection&, bool&);

    void createConnection();
    void handleConnections();
    void handleDisconnections();

    void informWorldState(sf::TcpSocket&);
    void sendToAll(sf::Packet&);
    void pingClients();

    void handleMessage(const Message&);

    std::function<void()> updateClientState;
    void updateClientGameState();
    void updateClientLobbyState();
    void getEntityPositions(const Entity&, sf::Packet&);
    void checkBounds(sf::Vector2f&);
};

#endif //SERVER_HPP_