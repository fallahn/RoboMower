//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/InstructionBlockLogic.hpp>

#include <xygine/Entity.hpp>
#include <xygine/Util.hpp>

namespace
{
    const float speed = 14.f;
}

InstructionBlockLogic::InstructionBlockLogic(xy::MessageBus& mb, Instruction inst)
    : Component     (mb, this),
    m_state         (State::Carried),
    m_instruction   (inst)
{

}

//public
void InstructionBlockLogic::entityUpdate(xy::Entity& entity, float dt)
{
    switch (m_state)
    {
    case State::Carried:
    case State::Idle:
    default: break;
    case State::Homing:
        auto dir = m_target - entity.getPosition();
        const auto len = xy::Util::Vector::lengthSquared(dir);
        if (len < 0.15f)
        {
            m_state = State::Idle;
            entity.setPosition(m_target);
        }
        else
        {
            entity.move(dir * speed * dt);
        }
        break;
    }
}

void InstructionBlockLogic::handleMessage(const xy::Message& msg)
{

}

void InstructionBlockLogic::setCarried(bool c)
{
    m_state = (c) ? State::Carried : State::Homing;
}

bool InstructionBlockLogic::carried() const
{
    return (m_state == State::Carried);
}

void InstructionBlockLogic::setTarget(const sf::Vector2f& target)
{
    m_target = target;
}

void InstructionBlockLogic::setCursorOffset(const sf::Vector2f& offset)
{
    m_cursorOffset = offset;
}

const sf::Vector2f& InstructionBlockLogic::getCursorOffset() const
{
    return m_cursorOffset;
}