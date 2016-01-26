//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//implements the logic for buttons in the UI tray

#ifndef BUTTON_LOGIC_HPP_
#define BUTTON_LOGIC_HPP_

#include <xygine/components/Component.hpp>

#include <InstructionSet.hpp>

#include <SFML/System/Vector2.hpp>

class ButtonLogicScript final : public xy::Component
{
public:
    ButtonLogicScript(xy::MessageBus&, Instruction);
    ~ButtonLogicScript() = default;
    ButtonLogicScript(const ButtonLogicScript&) = delete;
    ButtonLogicScript& operator = (const ButtonLogicScript&) = delete;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }

    void entityUpdate(xy::Entity&, float) override;

    Instruction getInstruction() const;
    void doClick(const sf::Vector2f& abs, const sf::Vector2f& rel);

private:

    Instruction m_instruction;

};

#endif //BUTTON_LOGIC_HPP_