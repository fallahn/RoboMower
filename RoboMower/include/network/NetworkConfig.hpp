//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_NETWORK_CONFIG_HPP_
#define RM_NETWORK_CONFIG_HPP_

#include <SFML/Config.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>

#include <unordered_map>
#include <functional>

namespace Network
{
    enum PacketType
    {
        Disconnect = -1,
        Connect,
        HeartBeat,
        Message = 4,
        //TODO typedef this so we can add custom types

        Bounds //must always be last
    };

    enum
    {
        HighestTimestamp = 2147483647,
        ServerPort = 5600,
        NullID = -1
    };
}

class Server;
struct ClientInfo;

using PacketID = sf::Int8;
using ClientID = sf::Int32;
using PortNumber = sf::Uint16;

using ClientList = std::unordered_map<ClientID, ClientInfo>;
using PacketHandler = std::function<void(const sf::IpAddress&, PortNumber, PacketID, sf::Packet&, Server*)>;
using TimeoutHandler = std::function<void(ClientID)>;

#endif //RM_NETWORK_CONFIG_HPP_
