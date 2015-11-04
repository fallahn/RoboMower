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

    void setTarget(const sf::Vector2f&, bool destroyWhenDone = true);
    void setTargetIndex(sf::Int32);
    sf::Int32 getTargetIndex() const;

    void setCursorOffset(const sf::Vector2f&);
    const sf::Vector2f& getCursorOffset() const;

    void setStackIndex(sf::Int32);
    sf::Int32 getStackIndex() const;
    sf::Int32 getPreviousStackIndex() const;

private:
    enum class State
    {
        Carried,
        Homing,
        Idle
    }m_state;

    Instruction m_instruction;
    sf::Vector2f m_target;
    sf::Int32 m_targetIndex;

    sf::Vector2f m_cursorOffset;
    bool m_destroyWhenDone;

    sf::Int32 m_stackIndex;
    sf::Int32 m_previousStackindex;

};


#endif //INSTRUCTION_BLOCK_LOGIC_HPP_