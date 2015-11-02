//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/ScrollHandleLogic.hpp>
#include <Messages.hpp>

#include <xygine/Assert.hpp>
#include <xygine/Entity.hpp>
#include <xygine/MessageBus.hpp>

ScrollHandleLogic::ScrollHandleLogic(xy::MessageBus& mb)
    : xy::Component (mb, this),
    m_carried       (false),
    m_length        (1.f),
    m_position      (0.f),
    m_update        (true)
{

}

//public
void ScrollHandleLogic::entityUpdate(xy::Entity& entity, float dt)
{
    //only update if scroll message marked as dirty
    if (m_update)
    {
        auto position = entity.getPosition();

        //clamp position
        if (position.y < 0) position.y = 0;
        else if (position.y > m_length) position.y = m_length;
        entity.setPosition(position);

        //update internals
        m_position = position.y / m_length;
        m_update = false;
    }
}

void ScrollHandleLogic::handleMessage(const xy::Message& msg)
{
    if (msg.id == MessageId::ScrollbarMessage)
    {
        m_update = true;
    }
}

void ScrollHandleLogic::setCarried(bool c)
{
    m_carried = c;
}

bool ScrollHandleLogic::carried() const
{
    return m_carried;
}

void ScrollHandleLogic::setLength(float len)
{
    XY_ASSERT(len > 0, "scroll length must be greater than 0");
    m_length = len;
}

float ScrollHandleLogic::getPosition() const
{
    return m_position;
}