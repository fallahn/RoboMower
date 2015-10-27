//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//extends the xygine message types

#ifndef MESSAGES_HPP_
#define MESSAGES_HPP_

#include <xygine/MessageBus.hpp>

#include <InstructionSet.hpp>

enum MessageId
{
    TrayIcon = xy::Message::Count
};

struct TrayIconEvent
{
    enum
    {
        Clicked
    }type;
    Instruction instruction;
};

#endif //MESSAGE_HPP_