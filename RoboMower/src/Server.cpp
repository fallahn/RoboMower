//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <network/Server.hpp>

#include <xygine/Log.hpp>

#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

namespace
{
    const sf::Int32 HEARTBEAT_RATE = 1000;
    const sf::Int32 HEARTBEAT_RETRIES = 5;
    const sf::Int32 CLIENT_TIMEOUT = 10000;
}

Server::Server(const PacketHandler& ph)
    : m_lastClientID    (-1),
    m_packetHandler     (ph),
    m_running           (false),
    m_listenThread      (&Server::listen, this),
    m_updateThread      (&Server::update, this),
    m_totalBytesSent    (0u),
    m_totalBytesReceived(0u)
{

}

Server::~Server()
{
    stop();
}

//public
void Server::setTimeoutHandler(const TimeoutHandler& th)
{
    m_timoutHandler = th;
}

bool Server::send(ClientID id, sf::Packet& packet)
{
    sf::Lock lock(m_mutex);

    auto result = m_clients.find(id);
    if (result == m_clients.end()) return false;

    if (m_outgoingSocket.send(packet, result->second.ipAddress, result->second.portNumber) != sf::Socket::Done)
    {
        LOG("SERVER - Failed sending packet to " + std::to_string(id), xy::Logger::Type::Warning);
        return false;
    }

    m_totalBytesSent += packet.getDataSize();
    return true;
}

bool Server::send(const sf::IpAddress& ip, PortNumber port, sf::Packet& packet)
{
    if (m_outgoingSocket.send(packet, ip, port) != sf::Socket::Done)
    {
        LOG("SERVER - Failed sending packet to " + ip.toString(), xy::Logger::Type::Warning);
        return false;
    }
    m_totalBytesSent += packet.getDataSize();
    return true;
}

void Server::broadcast(sf::Packet& packet, ClientID ignore)
{
    sf::Lock lock(m_mutex);
    for (auto& c : m_clients)
    {
        if (c.first != ignore)
        {
            send(c.first, packet);
        }
    }
}

void Server::listen()
{
    sf::IpAddress ip;
    PortNumber port = 0u;
    sf::Packet packet;

    LOG("SERVER - Started listening...", xy::Logger::Type::Info);

    while (m_running)
    {       
        packet.clear();
        auto status = m_incomingSocket.receive(packet, ip, port);
        if (status != sf::Socket::Done)
        {
            if (m_running)
            {
                LOG("SERVER - Error rx packet from: " + ip.toString() + ":" + std::to_string(port) + ", Code: " + std::to_string(status), xy::Logger::Type::Warning);
                continue;
            }
            else
            {
                LOG("SERVER - Socket unbound", xy::Logger::Type::Warning);
                break;
            }
        }
        m_totalBytesReceived += packet.getDataSize();

        PacketID packetID = 0;
        if (!(packet >> packetID))
        {
            //invalid packet
            LOG("SERVER - Invalid packet ID received: " + std::to_string(packetID), xy::Logger::Type::Warning);
            continue;
        }

        Network::PacketType packetType = static_cast<Network::PacketType>(packetID);
        if (packetType < Network::Disconnect || packetType >= Network::Bounds)
        {
            //invalid type
            LOG("SERVER - Invalid packet type received: " + std::to_string(packetType), xy::Logger::Type::Warning);
            continue;
        }

        //handle heartbeat packets
        if (packetType == Network::HeartBeat)
        {
            bool foundClient = false;

            sf::Lock lock(m_mutex);
            for (auto& c : m_clients)
            {
                if (c.second.ipAddress != ip || c.second.portNumber != port)
                {
                    continue;
                }
                foundClient = true;

                if (!c.second.heartbeatWaiting)
                {
                    LOG("SERVER - Invalid heartbeat packet received...", xy::Logger::Type::Warning);
                    break;
                }

                c.second.ping = m_serverTime.asMilliseconds() - c.second.heartbeatSent.asMilliseconds();
                c.second.lastHeartbeat = m_serverTime;
                c.second.heartbeatWaiting = false;
                c.second.heartbeatRetry = 0u;
                break;
            }

            if (!foundClient)
            {
                xy::Logger::log("SERVER - Received heartbeat from unknown client", xy::Logger::Type::Warning, xy::Logger::Output::All);
            }

        }
        //else pass to cutom handler if it exists
        else// if (m_packetHandler) //we can assume a handler as it's required on construction
        {
            m_packetHandler(ip, port, packetID, packet, this);
        }
    }

    LOG("SERVER - Stopped listening...", xy::Logger::Type::Info);
}

void Server::update()
{
    sf::Clock updateClock;
    while (m_running)
    {
        update(updateClock.restart());
    }
}

void Server::update(const sf::Time& time)
{
    m_serverTime += time;
    //check for time overflow
    if (m_serverTime.asMilliseconds() < 0)
    {
        //use sfml data types when networking
        auto maxTime = sf::Int32(Network::HighestTimestamp);
        m_serverTime -= sf::milliseconds(maxTime);
        sf::Lock lock(m_mutex);
        for (auto& c : m_clients)
        {
            c.second.lastHeartbeat = sf::milliseconds(std::abs(c.second.lastHeartbeat.asMilliseconds()) - maxTime);
        }
    }

    sf::Lock lock(m_mutex);
    for (auto it = m_clients.begin(); it != m_clients.end();)
    {
        auto elapsedTime = m_serverTime.asMilliseconds() - it->second.lastHeartbeat.asMilliseconds();
        //update client heartbeat
        if (elapsedTime > HEARTBEAT_RATE)
        {
            //remove timeouts
            if (elapsedTime > CLIENT_TIMEOUT || it->second.heartbeatRetry > HEARTBEAT_RETRIES)
            {
                LOG("SERVER - Client " + std::to_string(it->first) + " has timed out", xy::Logger::Type::Info);
                if (m_timoutHandler)
                {
                    m_timoutHandler(it->first);
                }
                it = m_clients.erase(it);
                continue;
            }
            
            //send heartbeat to remaining
            if (!it->second.heartbeatWaiting || (elapsedTime >= HEARTBEAT_RATE * (it->second.heartbeatRetry + 1)))
            {
                sf::Packet heartbeat;
                heartbeat << Network::HeartBeat;
                heartbeat << m_serverTime.asMilliseconds();
                send(it->first, heartbeat);
                LOG("SERVER - ping!", xy::Logger::Type::Info);

                if (it->second.heartbeatRetry == 0)
                {
                    it->second.heartbeatSent = m_serverTime;
                }
                it->second.heartbeatWaiting = true;
                //++it;

                m_totalBytesSent += heartbeat.getDataSize();
            }
        }
        ++it;
    }
}

ClientID Server::addClient(const sf::IpAddress& ip, PortNumber port)
{
    sf::Lock lock(m_mutex);
    //check not already added
    for (const auto& c : m_clients)
    {
        if (c.second.ipAddress == ip && c.second.portNumber == port)
        {
            return ClientID(Network::NullID);
        }
    }

    //create new client
    ClientID id = m_lastClientID++;
    ClientInfo clientInfo(ip, port, m_serverTime);
    m_clients.emplace(id, clientInfo);
    LOG("SERVER - Added client with ID: " + std::to_string(id), xy::Logger::Type::Info);
    return id;
}

ClientID Server::getClientID(const sf::IpAddress& ip, PortNumber port)
{
    sf::Lock lock(m_mutex);
    for (const auto& c : m_clients)
    {
        if (c.second.ipAddress == ip && c.second.portNumber == port)
        {
            return c.first;
        }
    }

    return ClientID(Network::NullID);
}

bool Server::hasClient(ClientID id) const
{
    return (m_clients.find(id) != m_clients.end());
}

bool Server::hasClient(const sf::IpAddress& ip, PortNumber port)
{
    return (getClientID(ip, port) >= 0);
}

bool Server::getClientInfo(ClientID id, ClientInfo& info)
{
    sf::Lock lock(m_mutex);
    for (const auto& c : m_clients)
    {
        if (c.first == id)
        {
            info = c.second;
            return true;
        }
    }
    return false;
}

bool Server::removeClient(ClientID id)
{
    sf::Lock lock(m_mutex);
    auto result = m_clients.find(id);
    if (result == m_clients.end()) return false;

    sf::Packet packet;
    packet << Network::Disconnect;
    send(id, packet); //hmm. Can't actually guarentee this will be received though...
    m_clients.erase(result);
    return true;
}

bool Server::removeClient(const sf::IpAddress& ip, PortNumber port)
{
    sf::Lock lock(m_mutex);
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        if (it->second.ipAddress == ip && it->second.portNumber == port)
        {
            sf::Packet packet;
            packet << Network::Disconnect;
            send(it->first, packet); //again no guarentee of delivery
            m_clients.erase(it);
            return true;
        }
    }

    return false;
}

void Server::disconnectAll()
{
    if (m_running)
    {
        sf::Packet packet;
        packet << Network::Disconnect;
        broadcast(packet);
        sf::Lock lock(m_mutex);
        m_clients.clear();
    }
}

bool Server::start()
{
    if (m_incomingSocket.bind(sf::Uint16(Network::ServerPort)) != sf::Socket::Done)
    {
        return false;
    }

    if (!m_running)
    {
        m_outgoingSocket.bind(sf::Socket::AnyPort); //TODO make a fixed range? easier for firewalling
        init();
        LOG("SERVER - Opened outgoing port: " + std::to_string(m_outgoingSocket.getLocalPort()), xy::Logger::Type::Info);
        LOG("SERVER - Opened incoming port: " + std::to_string(m_incomingSocket.getLocalPort()), xy::Logger::Type::Info);

        m_running = true;
        m_listenThread.launch();
        m_updateThread.launch();
        return true;
    }
    return false;
}

bool Server::stop()
{
    if (m_running)
    {
        disconnectAll();
        m_running = false;
        m_incomingSocket.unbind();
        return true;
    }
    return false;
}

bool Server::running() const
{
    return m_running;
}

std::size_t Server::getClientCount() const
{
    return m_clients.size();
}

sf::Mutex& Server::getMutex()
{
    return m_mutex;
}

//private
void Server::init()
{
    m_lastClientID = 0u;
    m_running = false;
    m_totalBytesSent = 0u;
    m_totalBytesReceived = 0u;
}