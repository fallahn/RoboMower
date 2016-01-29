//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_SERVER_HPP_
#define RM_SERVER_HPP_

#include <network/NetworkConfig.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>

#include <SFML/Network/UdpSocket.hpp>

#include <atomic>

struct ClientInfo final
{
    sf::IpAddress ipAddress;
    PortNumber portNumber;
    sf::Time lastHeartbeat;
    sf::Time heartbeatSent;
    bool heartbeatWaiting = false;
    sf::Uint16 heartbeatRetry = 0u;
    sf::Uint32 ping = 0;

    ClientInfo(const sf::IpAddress& ip, PortNumber port, const sf::Time& heartBeat)
        : ipAddress     (ip),
        portNumber      (port),
        lastHeartbeat   (heartBeat) {}
};

class Server final
{
public:
    explicit Server(const PacketHandler&);
    ~Server();

    Server(const Server&) = delete;
    Server& operator = (const Server&) = delete;

    void setTimeoutHandler(const TimeoutHandler&);

    bool send(ClientID, sf::Packet&);
    bool send(const sf::IpAddress&, PortNumber, sf::Packet&);
    void broadcast(sf::Packet&, ClientID ignore = Network::NullID);

    void listen();
    void update();
    void update(const sf::Time&);

    ClientID addClient(const sf::IpAddress&, PortNumber);
    ClientID getClientID(const sf::IpAddress&, PortNumber);
    bool hasClient(ClientID) const;
    bool hasClient(const sf::IpAddress&, PortNumber);
    bool getClientInfo(ClientID, ClientInfo&);
    bool removeClient(ClientID);
    bool removeClient(const sf::IpAddress&, PortNumber);

    void disconnectAll();
    bool start();
    bool stop();

    bool running() const;

    std::size_t getClientCount() const;

    sf::Mutex& getMutex();

private:

    ClientID m_lastClientID;

    sf::UdpSocket m_incomingSocket;
    sf::UdpSocket m_outgoingSocket;

    PacketHandler m_packetHandler;
    TimeoutHandler m_timoutHandler;

    ClientList m_clients;
    sf::Time m_serverTime;

    std::atomic_bool m_running;

    sf::Thread m_listenThread;
    sf::Thread m_updateThread;
    sf::Mutex m_mutex;

    std::atomic<size_t> m_totalBytesSent;
    std::atomic<size_t> m_totalBytesReceived;

    void init();
};

#endif //RM_SERVER_HPP_
