//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_NET_PROTOCOL_HPP_
#define RM_NET_PROTOCOL_HPP_

#include <network/NetworkConfig.hpp>

enum PacketIdent
{
    //client id, name
    PlayerDetails = PacketID(Network::PacketType::Count),
    //count, client id, position
    PositionUpdate,
    //clientId, direction
    DirectionUpdate
};

enum class Direction : sf::Uint8
{
    Left,
    Right,
    Up,
    Down
};

#endif //RM_NET_PROTOCOL_HPP_
