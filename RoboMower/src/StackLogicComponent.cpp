//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/StackLogicComponent.hpp>
#include <components/InstructionBlockLogic.hpp>
#include <components/RoundedRectangle.hpp>
#include <Messages.hpp>
#include <CommandCategories.hpp>

#include <xygine/Entity.hpp>
#include <xygine/Util.hpp>
#include <xygine/Log.hpp>
#include <xygine/Reports.hpp>
#include <xygine/Scene.hpp>
#include <xygine/ParticleController.hpp>

namespace
{
    const std::size_t maxInstructions = 32;
    const float padding = 22.f;
    const float margin = 8.f;
}

StackLogicComponent::StackLogicComponent(xy::MessageBus& mb, const sf::Vector2f& slotSize, const sf::Vector2f& bounds)
    : xy::Component     (mb, this),
    m_slots             (maxInstructions),
    m_localBounds       ({}, bounds),
    //m_updateTransform   (true),
    m_parentEntity      (nullptr),
    m_verticalDistance  (0.f),
    m_instructionCount  (0)
{
    for (auto i = 0u; i < m_slots.size(); ++i)
    {
        m_slots[i].slotLocalArea = { { margin, (i * (slotSize.y + padding)) + (margin * 2.f) }, slotSize };
        REPORT("Slot " + std::to_string(i), std::to_string(m_slots[i].occupierID));
    }

    m_verticalDistance = m_slots[1].slotLocalArea.top - m_slots[0].slotLocalArea.top;
    LOG("vertical distance: " + std::to_string(m_verticalDistance), xy::Logger::Type::Info);
}

//public
void StackLogicComponent::entityUpdate(xy::Entity& entity, float)
{
    //TODO this might be better if only done when entity is transformed (receive scroll message to invalidate?)
    auto tr = entity.getWorldTransform();
    for (auto& s : m_slots)
    {
        s.slotArea = tr.transformRect(s.slotLocalArea);
    }
    m_globalBounds = tr.transformRect(m_localBounds);


#ifdef _DEBUG_
    std::string targeted;
    for (auto i = 0u; i < m_slots.size(); ++i)
    {
        targeted = (m_slots[i].targeted) ? "true" : "false";
        REPORT("Slot " + std::to_string(i), std::to_string(m_slots[i].occupierID) + ", " + targeted);
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
            auto size = std::min(m_instructionCount + 1, maxInstructions);
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
                                        ib->setStackIndex(-1);
                                    };
                                }
                                m_parentEntity->getScene()->sendCommand(cmd);
                                m_slots[j].occupierID = 0;
                            }
                        }
                    }
                    break;
                }

                //we got to the end so therefore test for deletion
                auto idx = msgData.component->getPreviousStackIndex();
                if (idx >= 0 &&
                    !m_globalBounds.contains(msgData.position))
                {
                    msgData.component->setTarget({ 1920.f - m_slots[0].slotArea.width, 20.f }); //TODO somewhere with a bin icon, or set this on mouse up
                    msgData.component->setStackIndex(-1);
                    m_slots[idx].targeted = false;
     
                    if (idx >= 0)
                    {
                        //block was dragged off stack so realign remaining
                        for (auto i = idx + 1; i < maxInstructions; ++i)
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
                            m_slots[i].targeted = false;
                        }
                    }
                }
            }
        break;
        }
        case InstructionBlockEvent::Dropped:
        {
            //update the slot data with the block info and vice versa
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
            //empty the slot if this block previously occupied it
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
        case InstructionBlockEvent::Destroyed:
        {
            auto position = msgData.position;
            //send a message to particle controller
            xy::Command cmd;
            cmd.category = CommandCategory::ParticleController;
            cmd.action = [position](xy::Entity& entity, float)
            {
                entity.getComponent<xy::ParticleController>()->fire(0, position);
            };
            m_parentEntity->getScene()->sendCommand(cmd);
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

sf::Vector2f StackLogicComponent::getNextSlot() const
{
    //start at the end and work up to get farthest slot
    for (auto it = m_slots.rbegin(); it != m_slots.rend(); ++it)
    {
        if (it->occupierID == 0 && !it->targeted)
        {
            return{ it->slotArea.left, it->slotArea.top };
        }
    }
    
    return{};
}

sf::Vector2f StackLogicComponent::getPreviousSlot() const
{
    //start at the beginning and work down to get farthest slot
    for (const auto& s : m_slots)
    {
        if (s.occupierID == 0 && !s.targeted)
        {
            return{ s.slotArea.left, s.slotArea.top };
        }
    }

    return{};
}