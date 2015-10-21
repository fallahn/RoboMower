//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MessageBus.hpp>
#include <Log.hpp>

MessageBus::MessageBus()
: m_polling(false){}

Message MessageBus::poll()
{
    m_polling = true;
    Message m = m_messages.front();
    m_messages.pop();

    return m;
}

void MessageBus::send(const Message& m)
{
    (m_polling) ? m_deferredMessages.push(m) : m_messages.push(m);
}

bool MessageBus::empty()
{
    auto empty = m_messages.empty();
    if (empty)
    {
        m_deferredMessages.swap(m_messages);
        m_polling = false;
    }

    return empty;
}

std::size_t MessageBus::pendingMessageCount() const
{
    return m_messages.size();
}