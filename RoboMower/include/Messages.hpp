//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//extends the xygine message types

#ifndef MESSAGES_HPP_
#define MESSAGES_HPP_

#include <xygine/MessageBus.hpp>

#include <InstructionSet.hpp>

class InstructionBlockLogic;

enum MessageId
{
    TrayIconMessage = xy::Message::Count,
    InstructionBlockMessage,
    ScrollbarMessage,
    InputBoxMessage
};

struct TrayIconEvent
{
    enum
    {
        Clicked
    }action;
    Instruction instruction = NOP;
    float absX, absY;
    float relX, relY;
};

struct InstructionBlockEvent
{
    enum
    {
        Moved,
        Dropped,
        PickedUp,
        Destroyed
    }action;
    InstructionBlockLogic* component = nullptr;
    sf::Vector2f position;
};

struct ScrollbarEvent
{
    float position = 0.f;
};

struct InputBoxEvent
{
    enum
    {
        Clicked,
        WindowClosed
    }action;
    sf::Uint64 entityId = 0;
    sf::Uint32 value = 0;
};

#endif //MESSAGE_HPP_