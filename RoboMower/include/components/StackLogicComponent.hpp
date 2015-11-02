//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//defines the layout logic for the instruction stack pane

#ifndef STACK_LOGIC_COMPONENT_HPP_
#define STACK_LOGIC_COMPONENT_HPP_

#include <xygine/Component.hpp>

#include <InstructionSet.hpp>

#include <vector>

class StackLogicComponent final : public xy::Component
{
public:
    StackLogicComponent(xy::MessageBus&, const sf::Vector2f& slotSize, const sf::Vector2f& overallSize);
    ~StackLogicComponent() = default;
    StackLogicComponent(const StackLogicComponent&) = delete;
    StackLogicComponent& operator = (const StackLogicComponent&) = delete;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const xy::Message&) override;

    void onStart(xy::Entity&) override;

private:
    struct SlotData
    {
        sf::FloatRect slotArea;
        sf::FloatRect slotLocalArea;
        sf::Uint64 occupierID = 0;
        Instruction instruction = NOP;
        bool targeted = false;
    };

    std::vector<SlotData> m_slots;
    bool m_updateTransform;

    sf::FloatRect m_globalBounds;
    sf::FloatRect m_localBounds;

    xy::Entity* m_parentEntity;
    float m_verticalDistance;
    std::size_t m_instructionCount;

    void updateInstructionCount();
    sf::Vector2f getNextSlot() const; //down
    sf::Vector2f getPreviousSlot() const;//up
};


#endif //STACK_LOGIC_COMPONENT_HPP_