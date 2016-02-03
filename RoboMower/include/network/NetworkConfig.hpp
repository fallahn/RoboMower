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
    //make this a class to enforce 1 byte encoding when
    //putting into a packet
    enum class PacketType: sf::Int8
    //enum PacketType
    {
        Disconnect = -1,
        Connect,
        HeartBeat,
        ServerFull,
        Count
    };

    enum
    {
        HighestTimestamp = 2147483647,
        ServerPort = 5600,
        NullID = -1
    };

    static const sf::Uint32 PROTOCOL_ID = 0x444C4142u;
}

using PacketID = sf::Int8;
using ClientID = sf::Int32;
using PortNumber = sf::Uint16;

#endif //RM_NETWORK_CONFIG_HPP_
