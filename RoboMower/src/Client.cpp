//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <network/Client.hpp>

#include <xygine/Log.hpp>

#include <SFML/System/Clock.hpp>

namespace
{
    const sf::Int32 CLIENT_TIMEOUT = 10000;
    const sf::Int32 CONNECTION_TIMEOUT = 5000;
}

Client::Client()
    : m_serverPort(0u),
    m_connected(false),
    m_listenThread(&Client::listen, this),
    m_updateThread(&Client::update, this)
{

}

Client::~Client()
{
    disconnect();
}

//public
bool Client::connect()
{
    if (!m_connected)
    {
        m_socket.bind(sf::Socket::AnyPort);
        LOG("CLIENT - Bound connection to port: " + std::to_string(m_socket.getLocalPort()), xy::Logger::Type::Info);

        sf::Packet packet;
        packet << PacketID(PacketType::Connect);
        //TODO attach other client info such as player name

        if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
        {
            LOG("CLIENT - Failed to connect to server", xy::Logger::Type::Error);
            m_socket.unbind();
            return false;
        }

        m_socket.setBlocking(false);
        packet.clear();

        sf::IpAddress rxIP;
        PortNumber rxPort = 0u;
        sf::Clock timer;

        LOG("CLIENT - Attempting to connect to: " + m_serverIp.toString() + ":" + std::to_string(m_serverPort), xy::Logger::Type::Info);
        while (timer.getElapsedTime().asMilliseconds() < CONNECTION_TIMEOUT)
        {
            sf::Socket::Status s = m_socket.receive(packet, rxIP, rxPort);
            if (s != sf::Socket::Done)
            {
                continue;
            }
            if (rxIP != m_serverIp)
            {
                continue;
            }

            PacketID id;
            if (!(packet >> id))
            {
                break;
            }

            PacketType packetType = static_cast<PacketType>(id);
            if (packetType != PacketType::Connect)
            {
                continue;
            }

            handlePacket(packetType, packet);

            m_connected = true;
            m_socket.setBlocking(true);
            m_lastHeartbeat = m_serverTime;
            m_listenThread.launch();
            m_updateThread.launch();
            return true;
        }
        LOG("CLIENT - Connect attempt timed out.", xy::Logger::Type::Error);
        m_socket.unbind();
        m_socket.setBlocking(false);
    }
    LOG("CLIENT - Already connected to: " + m_serverIp.toString(), xy::Logger::Type::Error);
    return false;
}

bool Client::disconnect()
{
    if(!m_connected) return false;

    sf::Packet packet;
    packet << PacketID(PacketType::Disconnect);

    //TODO fix this up when updating the protocol so we know 
    //when disconnected has properly been sent
    auto status = m_socket.send(packet, m_serverIp, m_serverPort);
    m_connected = false;
    m_socket.unbind();

    return (status == sf::Socket::Done);
}

bool Client::send(sf::Packet& packet)
{
    if (!m_connected) return false;

    return (m_socket.send(packet, m_serverIp, m_serverPort) == sf::Socket::Done);
}

const sf::Time& Client::getTime() const
{
    return m_serverTime;
}

const sf::Time& Client::getLastHeartbeat() const
{
    return m_lastHeartbeat;
}

void Client::setTime(const sf::Time& time)
{
    m_serverTime = time;
}

void Client::setServerInfo(const sf::IpAddress& ip, PortNumber port)
{
    m_serverIp = ip;
    m_serverPort = port;
}

void Client::setPacketHandler(const PacketHandler& ph)
{
    m_packetHandler = ph;
}

void Client::removePacketHandler()
{
    m_packetHandler = nullptr;
}

bool Client::connected() const
{
    return m_connected;
}

sf::Mutex& Client::getMutex()
{
    return m_mutex;
}

//private
void Client::handlePacket(PacketType type, sf::Packet& packet)
{
    switch (type)
    {
    case PacketType::HeartBeat:
        {
            sf::Packet packet;
            packet << PacketID(PacketType::HeartBeat);
            if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
            {
                LOG("CLIENT - Failed sending heartbeat", xy::Logger::Type::Warning);
            }

            sf::Int32 timestamp;
            packet >> timestamp;
            setTime(sf::milliseconds(timestamp));
            m_lastHeartbeat = m_serverTime;
        }
        return;
    case PacketType::Disconnect:
        disconnect();
        return;
    default:break;
    }


    if (m_packetHandler)
    {
        m_packetHandler(type, packet, this);
    }
}

void Client::listen()
{
    sf::Packet packet;
    sf::IpAddress rxIP;
    PortNumber rxPort = 0u;
    LOG("CLIENT - Started listen thread", xy::Logger::Type::Info);

    while (m_connected)
    {
        packet.clear();
        auto status = m_socket.receive(packet, rxIP, rxPort);
        if (status != sf::Socket::Done)
        {
            if (m_connected)
            {
                LOG("CLIENT - Failed receiving packet from: " + rxIP.toString() + ":" + std::to_string(rxPort) + ", status: " + std::to_string(status), xy::Logger::Type::Error);
                continue;
            }
            else
            {
                LOG("CLIENT - Socket unbound", xy::Logger::Type::Error);
                break;
            }
        }

        if (rxIP != m_serverIp)
        {
            //ignore stranger packets
            continue;
        }

        PacketID packetID;
        if (!(packet >> packetID))
        {
            //ignore unknown IDs
            continue;
        }

        PacketType packetType = static_cast<PacketType>(packetID);
        if (packetType < PacketType::Disconnect || packetType >= PacketType::Bounds)
        {
            //ignore packets out of bounds
            continue;
        }

        //handle the packet
        handlePacket(packetType, packet);
    }
}

void Client::update()
{
    sf::Clock updateClock;
    while (m_connected)
    {
        update(updateClock.restart());
    }
}

void Client::update(const sf::Time& time)
{
    m_serverTime += time;
    if (m_serverTime.asMilliseconds() < 0)
    {
        m_serverTime -= sf::milliseconds(Network::HighestTimestamp);
        m_lastHeartbeat = m_serverTime;
        return;
    }

    if (m_serverTime.asMilliseconds() - m_lastHeartbeat.asMilliseconds() > CLIENT_TIMEOUT)
    {
        LOG("CLIENT - Server connection timed out", xy::Logger::Type::Info);
        disconnect();
    }
}