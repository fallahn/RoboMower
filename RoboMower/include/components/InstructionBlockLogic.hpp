//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================


#ifndef INSTRUCTION_BLOCK_LOGIC_HPP_
#define INSTRUCTION_BLOCK_LOGIC_HPP_

#include <xygine/Component.hpp>

#include <InstructionSet.hpp>

#include <SFML/System/Vector2.hpp>

class InstructionBlockLogic final : public xy::Component
{
public: 
    InstructionBlockLogic(xy::MessageBus&, Instruction);
    ~InstructionBlockLogic() = default;
    InstructionBlockLogic(const InstructionBlockLogic&) = delete;
    InstructionBlockLogic& operator = (const InstructionBlockLogic&) = delete;

    Component::Type type() const override { return Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const xy::Message&) override;

    Instruction getInstruction() const { return m_instruction; }
    void setCarried(bool);
    bool carried() const;

    void setTarget(const sf::Vector2f&);
    void setCursorOffset(const sf::Vector2f&);
    const sf::Vector2f& getCursorOffset() const;

private:
    enum class State
    {
        Carried,
        Homing,
        Idle
    }m_state;

    Instruction m_instruction;
    sf::Vector2f m_target;
    sf::Vector2f m_cursorOffset;
};


#endif //INSTRUCTION_BLOCK_LOGIC_HPP_