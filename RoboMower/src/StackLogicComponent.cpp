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
#include <xygine/Scene.hpp>

namespace
{
    const std::size_t maxInstructions = 12;
    const float padding = 32.f;
    const float margin = 8.f;
}

StackLogicComponent::StackLogicComponent(xy::MessageBus& mb, const sf::Vector2f& size)
    : xy::Component     (mb, this),
    m_slots             (maxInstructions),
    m_updateTransform   (true),
    m_parentEntity      (nullptr),
    m_verticalDistance  (0.f),
    m_instructionCount  (0)
{
    for (auto i = 0u; i < m_slots.size(); ++i)
    {
        m_slots[i].slotArea = { { margin, (i * (size.y + padding)) + margin }, size };
        REPORT("Slot " + std::to_string(i), std::to_string(m_slots[i].occupierID));
    }

    m_verticalDistance = m_slots[1].slotArea.top - m_slots[0].slotArea.top;
    LOG("vertical distance: " + std::to_string(m_verticalDistance), xy::Logger::Type::Info);
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

#ifdef _DEBUG_
    for (auto i = 0u; i < m_slots.size(); ++i)
    {
        REPORT("Slot " + std::to_string(i), std::to_string(m_slots[i].occupierID));
    }
#endif //_DEBUG_
}

void StackLogicComponent::handleMessage(const xy::Message& msg)
{
    if (msg.id == MessageId::InstructionBlockMessage)
    {
        auto& msgData = msg.getData<InstructionBlockEvent>();
        
        switch (msgData.action)
        {
        case InstructionBlockEvent::Moved:
        {   
            auto size = m_instructionCount + 1;
            for (auto i = 0u; i < size; ++i)
            {
                const auto& area = m_slots[i].slotArea;
                if (area.contains(msgData.position) && !m_slots[i].targeted)
                {
                    msgData.component->setTarget({ area.left, area.top }, false);
                    m_slots[i].targeted = true;
                    if (m_slots[i].occupierID != 0)
                    {
                        //check previous slot is empty and move any icons up
                        if (i > 0 && m_slots[i - 1].occupierID == 0)
                        {
                            xy::Command cmd;
                            cmd.entityID = m_slots[i].occupierID;
                            cmd.action = [this](xy::Entity& entity, float)
                            {
                                auto ib = entity.getComponent<InstructionBlockLogic>();
                                XY_ASSERT(ib, "component doesn't exist");
                                ib->setTarget(entity.getWorldPosition() + sf::Vector2f(0.f, -m_verticalDistance), false);
                                ib->setCarried(false);
                            };
                            m_parentEntity->getScene()->sendCommand(cmd);
                            m_slots[i].occupierID = 0;
                            m_slots[i - 1].targeted = true;
                        }
                        else
                        {
                            //move existing instruction tabs down
                            for (auto j = i; j < m_slots.size(); ++j)
                            {
                                if (m_slots[j].occupierID == 0) break;

                                xy::Command cmd;
                                cmd.entityID = m_slots[j].occupierID;

                                if (j < m_slots.size() - 1)
                                {
                                    //safe to move down
                                    cmd.action = [this](xy::Entity& entity, float)
                                    {
                                        auto ib = entity.getComponent<InstructionBlockLogic>();
                                        XY_ASSERT(ib, "component doesn't exist");
                                        ib->setTarget(entity.getWorldPosition() + sf::Vector2f(0.f, m_verticalDistance), false);
                                        ib->setCarried(false);
                                    };
                                    m_slots[i + 1].targeted = true;
                                }
                                else
                                {
                                    //pop off end
                                    cmd.action = [](xy::Entity& entity, float)
                                    {
                                        auto ib = entity.getComponent<InstructionBlockLogic>();
                                        XY_ASSERT(ib, "component doesn't exist");
                                        ib->setTarget(entity.getWorldPosition() + sf::Vector2f(0.f, 500.f)); //arbitary number here just to push icon off bottom
                                        ib->setCarried(false);
                                    };
                                }
                                m_parentEntity->getScene()->sendCommand(cmd);
                                m_slots[j].occupierID = 0;
                            }
                        }
                    }
                    break;
                }
            }
        break;
        }
        case InstructionBlockEvent::Dropped:
        {
            for (auto i = 0u; i < m_slots.size(); ++i)
            {
                if (m_slots[i].slotArea.contains(msgData.position))
                {                     
                    m_slots[i].occupierID = msgData.component->getParentUID();
                    m_slots[i].instruction = msgData.component->getInstruction();
                    m_slots[i].targeted = false;
                    msgData.component->setStackIndex(i);
                }
            }

            //recalc size
            updateInstructionCount();
        }
        break;
        case InstructionBlockEvent::PickedUp:
        {
            auto i = msgData.component->getStackIndex();
            if (i >= 0)
            {
                m_slots[i].occupierID = 0;
                m_slots[i].instruction = Instruction::NOP;
                m_slots[i].targeted = false;
                msgData.component->setStackIndex(-1);
                updateInstructionCount();
            }
        }
            break;
        default: break;
        }
    }
}

void StackLogicComponent::onStart(xy::Entity& entity)
{
    m_parentEntity = &entity;
}

//private
void StackLogicComponent::updateInstructionCount()
{
    m_instructionCount = 0;

    for (const auto& s : m_slots)
    {
        if (s.occupierID != 0) m_instructionCount++;
    }

    REPORT("Instruction Count", std::to_string(m_instructionCount));
}