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

//defines the layout logic for the instruction stack pane

#ifndef STACK_LOGIC_COMPONENT_HPP_
#define STACK_LOGIC_COMPONENT_HPP_

#include <xygine/components/Component.hpp>

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
    float m_maxScrollDistance;
    std::size_t m_instructionCount;

    void updateInstructionCount();
    void instructionBlockHandler(xy::Component* c, const xy::Message& msg);
    void scrollHandler(xy::Component* c, const xy::Message& msg);
};


#endif //STACK_LOGIC_COMPONENT_HPP_