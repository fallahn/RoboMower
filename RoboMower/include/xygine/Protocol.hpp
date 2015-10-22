//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//spec for network protocol

#ifndef NET_PROTOCOL_HPP_
#define NET_PROTOCOL_HPP_

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

namespace Client
{
    //packet IDs from client
    enum PacketId
    {
        RequestClientID = 1,
        RequestWorldState,
        StartReady,
        InputUpdate,
        LobbyData,
        Quit,
        Ping /*int16 UID, float original time*/
    };
}

namespace Server
{
    struct EntityState
    {
        sf::Int16 uid;
        sf::Vector2f position;
    };

    //packet IDs from server
    enum PacketId
    {
        ClientID = 1,
        InitialState, /*Uint32 playercount, states, Uint32 alienCount, states, UInt32 humancount, states*/
        LobbyData, /*Uint32 count, Int16 uid - TODO add name and ready status*/
        PlayerConnect,
        PlayerDisconnect,
        UpdateClientState, /*Int32 server time, Int32 size, State entityState*/
        UpdatePlayerState, /*Int16 Id, PlayerInfo*/
        GameEvent, /*Int16 id, Int32 event type*/
        GameStart,
        GameOver,
        Ping /*float current server time*/
    };
}

namespace GameEvent
{
    enum Type
    {

    };
}

#endif //NET_PROTOCOL_HPP_