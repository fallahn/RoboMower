/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/


#ifndef INSTRUCTION_BLOCK_LOGIC_HPP_
#define INSTRUCTION_BLOCK_LOGIC_HPP_

#include <xygine/components/Component.hpp>

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
    void onDelayedStart(xy::Entity&) override;

    Instruction getInstruction() const { return m_instruction; }
    sf::Uint8 getValue() const { return m_value; }
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

    void setPreviousLoop(sf::Int32 pl) { m_previousLoop = pl; }

private:
    enum class State
    {
        Carried,
        Homing,
        Idle
    }m_state;

    Instruction m_instruction;
    sf::Uint8 m_value;
    sf::Vector2f m_target;
    sf::Int32 m_targetIndex;

    sf::Vector2f m_cursorOffset;
    bool m_destroyWhenDone;

    sf::Int32 m_stackIndex;
    sf::Int32 m_previousStackindex;

    sf::Int32 m_previousLoop;

    xy::Entity* m_entity;
};


#endif //INSTRUCTION_BLOCK_LOGIC_HPP_