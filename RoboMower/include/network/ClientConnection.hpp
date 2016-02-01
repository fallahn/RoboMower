//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_NET_CLIENT_HPP_
#define RM_NET_CLIENT_HPP_

#include <network/NetworkConfig.hpp>

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <atomic>

namespace Network
{
    class ClientConnection final
    {
    public:
        using PacketHandler = std::function<void(PacketType, sf::Packet&, ClientConnection*)>;

        ClientConnection();
        ~ClientConnection();

        ClientConnection(const ClientConnection&) = delete;
        ClientConnection& operator = (const ClientConnection&) = delete;

        bool connect();
        bool disconnect();
        void update(float);

        bool send(sf::Packet&);

        const sf::Time& getTime() const;
        const sf::Time& getLastHeartbeat() const;
        void setTime(const sf::Time&);
        void setServerInfo(const sf::IpAddress&, PortNumber);

        void setPacketHandler(const PacketHandler&);
        void removePacketHandler();

        bool connected() const;

        sf::Mutex& getMutex();

    private:

        sf::UdpSocket m_socket;
        sf::IpAddress m_serverIp;
        PortNumber m_serverPort;

        PacketHandler m_packetHandler;
        std::atomic_bool m_connected;

        sf::Time m_serverTime;
        sf::Time m_lastHeartbeat;

        sf::Thread m_listenThread;
        sf::Mutex m_mutex;

        void handlePacket(PacketType, sf::Packet&);

        void listen();
    };
}
#endif //RM_NET_CLIENT_HPP_