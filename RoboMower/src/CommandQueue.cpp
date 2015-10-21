//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <Command.hpp>
#include <Entity.hpp>

void CommandQueue::push(const Command& command)
{
    m_queue.push(command);
}

Command CommandQueue::pop()
{
    auto cmd = m_queue.front();
    m_queue.pop();
    return cmd;
}

bool CommandQueue::empty() const
{
    return m_queue.empty();
}
