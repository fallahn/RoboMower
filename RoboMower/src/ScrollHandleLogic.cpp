//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/ScrollHandleLogic.hpp>

#include <xygine/Assert.hpp>
#include <xygine/Entity.hpp>

ScrollHandleLogic::ScrollHandleLogic(xy::MessageBus& mb)
    : xy::Component (mb, this),
    m_carried       (false),
    m_length        (1.f),
    m_position      (0.f)
{

}

//public
void ScrollHandleLogic::entityUpdate(xy::Entity& entity, float dt)
{
    //TODO only update if scroll message marked as dirty
    auto position = entity.getPosition();

    //clamp position
    if (position.y < 0) position.y = 0;
    else if (position.y > m_length) position.y = m_length;
    entity.setPosition(position);

    //update internals
    m_position = position.y / m_length;
}

void ScrollHandleLogic::handleMessage(const xy::Message&)
{

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
    XY_ASSERT(len > 0, "handle length ,ust be greater than 0");
    m_length = len;
}

float ScrollHandleLogic::getPosition() const
{
    return m_position;
}