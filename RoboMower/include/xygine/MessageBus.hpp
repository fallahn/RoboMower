//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//message bus to allow inter-component communication

#ifndef MESSAGE_BUS_HPP_
#define MESSAGE_BUS_HPP_

#include <xygine/StateIds.hpp>

#include <SFML/Config.hpp>

#include <queue>

class Component;
class Entity;

class Message final
{
public:
    enum class Type
    {
        Audio = 1,
        Physics,
        Entity,
        UI,
        Player,
        ComponentSystem,
        Network
    }type;

    struct AudioEvent
    {
        sf::Uint64 entityId;
    };

    struct PhysicsEvent
    {
        enum
        {
            Collision
        }event;

        sf::Uint64 entityId[2];
    };

    struct EntityEvent
    {
        enum
        {
            Destroyed,
            ChangedDirection
        }action;
        Entity* entity;
        sf::Int32 direction;
    };

    struct UIEvent
    {
        enum
        {
            RequestVolumeChange,
            RequestAudioMute,
            RequestAudioUnmute,
            MenuOpened,
            MenuClosed,
            RequestState,
            RequestDifficultyChange,
            RequestControllerEnable,
            RequestControllerDisable,
            ResizedWindow
        }type;
        float value;
        States::ID stateId;
        Difficulty difficulty;
    };

    struct PlayerEvent
    {
        enum
        {
            Died,
            Spawned
        }action;
        sf::Int32 timestamp;
        sf::Uint64 entityID;
    };

    struct ComponentEvent
    {
        enum
        {
            Deleted
        }action;
        sf::Uint64 entityId;
        Component* ptr;
    };

    struct NetworkEvent
    {
        enum
        {
            RequestStartServer,
            RequestJoinServer,
            RequestDisconnect,
            RequestWorldState,
            ConnectSuccess,
            ConnectFail,
            StartReady
        }action;
        States::ID stateID;
    };

    union
    {
        AudioEvent audio;
        PhysicsEvent physics;
        EntityEvent entity;
        UIEvent ui;
        PlayerEvent player;
        ComponentEvent component;
        NetworkEvent network;
    };
};

class MessageBus final
{
public:
    MessageBus() = default;
    ~MessageBus() = default;
    MessageBus(const MessageBus&) = delete;
    const MessageBus& operator = (const MessageBus&) = delete;

    //read and despatch all messages on the message stack
    Message poll();
    //places a message on the message stack
    void post(const Message& msg);

    bool empty();

    std::size_t pendingMessageCount() const; //used for stats

private:
    std::queue<Message> m_currentMessages;
    std::queue<Message> m_pendingMessages;
};

#endif