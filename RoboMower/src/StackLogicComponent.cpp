//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/StackLogicComponent.hpp>
#include <components/InstructionBlockLogic.hpp>
#include <Messages.hpp>

#include <xygine/Entity.hpp>
#include <xygine/Util.hpp>
#include <xygine/Log.hpp>
#include <xygine/Reports.hpp>

namespace
{
    const std::size_t maxInstructions = 10;
    const float padding = 16.f;
}

StackLogicComponent::StackLogicComponent(xy::MessageBus& mb, const sf::Vector2f& size)
    : xy::Component     (mb, this),
    m_slots             (maxInstructions),
    m_updateTransform   (true)
{
    for (auto i = 0u; i < m_slots.size(); ++i)
    {
        m_slots[i].slotArea = { { 0.f, (i * size.y) + padding }, size };
    }
}

//public
void StackLogicComponent::entityUpdate(xy::Entity& entity, float)
{
    if (m_updateTransform)
    {
        auto tr = entity.getWorldTransform();
        for (auto& s : m_slots)
        {
            s.slotArea = tr.transformRect(s.slotArea);
        }
        m_updateTransform = false;
    }


    int i = 0u;
    for (const auto& s : m_slots)
    {
        REPORT("Slot " + std::to_string(i++), std::to_string(s.occupierID));
    }
}

void StackLogicComponent::handleMessage(const xy::Message& msg)
{
    if (msg.id == MessageId::InstructionBlockMessage)
    {
        auto& msgData = msg.getData<InstructionBlockEvent>();
        
        switch (msgData.action)
        {
        case InstructionBlockEvent::Moved:
            for (auto& slot : m_slots)
            {
                const auto& area = slot.slotArea;
                if (slot.occupierID == 0 && area.contains(msgData.position))
                {
                    msgData.component->setTarget({ area.left, area.top }, false);
                    //LOG("intersects stack", xy::Logger::Type::Info);
                    break;
                }
            }
            break;
        case InstructionBlockEvent::Dropped:
        {
            for (auto i = 0u; i < m_slots.size(); ++i)
            {
                if (m_slots[i].slotArea.contains(msgData.position))
                {
                    m_slots[i].occupierID = msgData.component->getParentUID();
                    m_slots[i].instruction = msgData.component->getInstruction();
                    msgData.component->setStackIndex(i);
                    break;
                }
            }
        }
        break;
        case InstructionBlockEvent::PickedUp:
        {
            auto i = msgData.component->getStackIndex();
            if (i >= 0)
            {
                m_slots[i].occupierID = 0;
                m_slots[i].instruction = Instruction::NOP;
                msgData.component->setStackIndex(-1);
            }
        }
            break;
        default: break;
        }
    }
}