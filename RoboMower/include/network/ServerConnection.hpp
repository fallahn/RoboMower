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

//namespace Network
//{
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
            : ipAddress(ip),
            portNumber(port),
            lastHeartbeat(heartBeat) {}
    };

    using ClientList = std::unordered_map<ClientID, ClientInfo>;

    class ServerConnection final
    {
    public:
        using PacketHandler = std::function<void(const sf::IpAddress&, PortNumber, Network::PacketType, sf::Packet&, ServerConnection*)>;
        using TimeoutHandler = std::function<void(ClientID)>;

        ServerConnection();
        ~ServerConnection();

        ServerConnection(const ServerConnection&) = delete;
        ServerConnection& operator = (const ServerConnection&) = delete;

        void setPacketHandler(const PacketHandler&);
        void setTimeoutHandler(const TimeoutHandler&);

        bool send(ClientID, sf::Packet&);
        bool send(const sf::IpAddress&, PortNumber, sf::Packet&);
        void broadcast(sf::Packet&, ClientID ignore = Network::NullID);

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

        void update(float);

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
        sf::Mutex m_mutex;

        std::atomic<size_t> m_totalBytesSent;
        std::atomic<size_t> m_totalBytesReceived;

        void listen();

        void init();
        void handlePacket(const sf::IpAddress&, PortNumber, Network::PacketType, sf::Packet&);
    };
//}
#endif //RM_SERVER_HPP_
