//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/ButtonLogic.hpp>
#include <Messages.hpp>

#include <xygine/Log.hpp>

ButtonLogicScript::ButtonLogicScript(xy::MessageBus& mb, Instruction inst)
    : Component(mb, this),
    m_instruction(inst){}

//public
void ButtonLogicScript::entityUpdate(xy::Entity& ent, float)
{

}

Instruction ButtonLogicScript::getInstruction() const
{
    return m_instruction;
}

void ButtonLogicScript::doClick(const sf::Vector2f& abs, const sf::Vector2f& rel)
{
    auto msg = sendMessage<TrayIconEvent>(MessageId::TrayIconMessage);
    msg->action = TrayIconEvent::Clicked;
    msg->instruction = m_instruction;
    msg->absX = abs.x;
    msg->absY = abs.y;
    msg->relX = rel.x;
    msg->relY = rel.y;
}