//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//message bus to allow inter-component communication

#ifndef MESSAGE_BUS_HPP_
#define MESSAGE_BUS_HPP_

#include <StateIds.hpp>

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
        Network,
        Alien,
        Human
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
            Spawned,
            FiredWeapon
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

    struct AlienEvent
    {
        enum
        {
            Died,
            Spawned,
            PickedUp,
            Targetted
        }action;
        sf::Uint64 entityID;
    };

    struct HumanEvent
    {
        enum
        {
            Died
        }action;
        sf::Uint64 entityID;
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
        AlienEvent alien;
        HumanEvent human;
    };
};

class MessageBus final
{
public:
    MessageBus();
    ~MessageBus() = default;
    MessageBus(const MessageBus&) = delete;
    const MessageBus& operator = (const MessageBus&) = delete;

    //read and despatch all messages on the message stack
    Message poll();
    //places a message on the message stack
    void send(const Message& msg);

    bool empty();

    std::size_t pendingMessageCount() const;

private:
    std::queue<Message> m_messages;
    std::queue<Message> m_deferredMessages;
    bool m_polling;
};

#endif