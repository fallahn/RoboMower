//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/ButtonLogic.hpp>

#include <xygine/Log.hpp>

ButtonLogicScript::ButtonLogicScript(xy::MessageBus& mb, Instruction inst)
    : Component(mb, this),
    m_instruction(inst){}

//public
void ButtonLogicScript::entityUpdate(xy::Entity& ent, float)
{

}

void ButtonLogicScript::handleMessage(const xy::Message&){}

Instruction ButtonLogicScript::getInstruction() const
{
    return m_instruction;
}

void ButtonLogicScript::doClick()
{
    std::cout << "flaps" << std::endl;
}