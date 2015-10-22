//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/MessageBus.hpp>
#include <xygine/Log.hpp>

Message MessageBus::poll()
{
    Message m = m_currentMessages.front();
    m_currentMessages.pop();

    return m;
}

void MessageBus::post(const Message& m)
{
    m_pendingMessages.push(m);
}

bool MessageBus::empty()
{
    if (m_currentMessages.empty())
    {
        m_currentMessages.swap(m_pendingMessages);
        return true;
    }
    return false;
}

std::size_t MessageBus::pendingMessageCount() const
{
    return m_currentMessages.size();
}