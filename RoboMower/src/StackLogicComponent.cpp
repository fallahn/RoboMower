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

#include <components/StackLogicComponent.hpp>
#include <components/InstructionBlockLogic.hpp>
#include <components/LoopHandle.hpp>
#include <RoundedRectangle.hpp>
#include <Messages.hpp>
#include <CommandCategories.hpp>

#include <xygine/Entity.hpp>
#include <xygine/Log.hpp>
#include <xygine/Reports.hpp>
#include <xygine/Scene.hpp>
#include <xygine/components/ParticleController.hpp>
#include <xygine/components/SfDrawableComponent.hpp>

#include <SFML/Graphics/Text.hpp>

namespace
{
    const std::size_t maxInstructions = 32;
    const float padding = 22.f;
    const float margin = 8.f;
}

StackLogicComponent::StackLogicComponent(xy::MessageBus& mb, const sf::Vector2f& slotSize, const sf::Vector2f& bounds)
    : xy::Component     (mb, this),
    m_slots             (maxInstructions),
    m_updateTransform   (true),
    m_localBounds       ({}, bounds),
    m_parentEntity      (nullptr),
    m_maxScrollDistance (bounds.y),
    m_instructionCount  (0)
{
    for (auto i = 0u; i < m_slots.size(); ++i)
    {
        m_slots[i].slotLocalArea = { { margin, (i * (slotSize.y + padding)) + (margin * 2.f) }, slotSize };
    }
    m_localBounds.height = m_slots.back().slotLocalArea.top + slotSize.y + margin;
    m_maxScrollDistance = m_localBounds.height - m_maxScrollDistance;

    xy::Component::MessageHandler mh;
    mh.id = MessageId::InstructionBlockMessage;
    mh.action = std::bind(&StackLogicComponent::instructionBlockHandler, this, std::placeholders::_1, std::placeholders::_2);
    addMessageHandler(mh);

    mh.id = MessageId::ScrollbarMessage;
    mh.action = std::bind(&StackLogicComponent::scrollHandler, this, std::placeholders::_1, std::placeholders::_2);
    addMessageHandler(mh);
}

//public
void StackLogicComponent::entityUpdate(xy::Entity& entity, float)
{
    if (m_updateTransform)
    {
        auto tr = entity.getWorldTransform();
        for (auto& s : m_slots)
        {
            s.slotArea = tr.transformRect(s.slotLocalArea);
        }
        m_globalBounds = tr.transformRect(m_localBounds);
        m_updateTransform = false;
    }
#ifdef _DEBUG_
    //std::string targeted;
    //for (auto i = 0u; i < m_slots.size() / 2; ++i)
    //{
    //    targeted = (m_slots[i].targeted) ? "true" : "false";
    //    REPORT("Slot " + std::to_string(i + 10), std::to_string(m_slots[i].occupierID) + ", " + targeted);
    //}
#endif //_DEBUG_
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

void StackLogicComponent::instructionBlockHandler(xy::Component* c, const xy::Message& msg)
{
    //if (msg.id == MessageId::InstructionBlockMessage)
    //{
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
                    auto targetIdx = msgData.component->getTargetIndex();
                    if (targetIdx != i)
                    {
                        if (targetIdx > -1) m_slots[targetIdx].targeted = false;
                        msgData.component->setTargetIndex(i);
                        m_slots[i].targeted = true;
                        msgData.component->setTarget({ area.left, area.top }, false);
                    }

                    //we need to move out occupier if it exists
                    if (m_slots[i].occupierID != 0)
                    {
                        //check previous slot is empty and move any icons up
                        if (i > 0 && m_slots[i - 1].occupierID == 0)
                        {
                            auto idx = i - 1;
                            {
                                sf::Vector2f targetPos(m_slots[idx].slotArea.left, m_slots[idx].slotArea.top);

                                xy::Command cmd;
                                cmd.entityID = m_slots[i].occupierID;
                                cmd.action = [targetPos](xy::Entity& entity, float)
                                {
                                    auto ib = entity.getComponent<InstructionBlockLogic>();
                                    XY_ASSERT(ib, "component doesn't exist");
                                    ib->setTarget(targetPos, false);
                                    ib->setCarried(false);
                                };
                                m_parentEntity->getScene()->sendCommand(cmd);
                                m_slots[i].occupierID = 0;
                                m_slots[i].instruction = Instruction::NOP; //wipe instruction else it may get counted for being something it isn't
                                m_slots[idx].targeted = true;
                            }
                        }
                        else
                        {
                            //move existing instruction tabs down
                            for (auto j = i; j < m_slots.size(); ++j)
                            {
                                if (m_slots[j].occupierID == 0) break; //got to end of occupied slots

                                xy::Command cmd;
                                cmd.entityID = m_slots[j].occupierID;

                                if (j < m_slots.size() - 1)
                                {
                                    //safe to move down
                                    auto idx = j + 1;
                                    sf::Vector2f targetPos(m_slots[idx].slotArea.left, m_slots[idx].slotArea.top);
                                    cmd.action = [targetPos](xy::Entity& entity, float)
                                    {
                                        auto ib = entity.getComponent<InstructionBlockLogic>();
                                        XY_ASSERT(ib, "component doesn't exist");
                                        ib->setTarget(targetPos, false);
                                        ib->setCarried(false);
                                    };
                                    m_slots[j + 1].targeted = true;
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
                                m_slots[j].instruction = Instruction::NOP;
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
                            sf::Vector2f targetPos(m_slots[i - 1].slotArea.left, m_slots[i - 1].slotArea.top);

                            xy::Command cmd;
                            cmd.entityID = m_slots[i].occupierID;
                            cmd.action = [targetPos](xy::Entity& entity, float)
                            {
                                auto ib = entity.getComponent<InstructionBlockLogic>();
                                XY_ASSERT(ib, "component doesn't exist");
                                ib->setTarget(targetPos, false);
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
            auto lastLoop = m_slots.size(); //so we know the last loop before current
            for (auto i = 0u; i < m_slots.size(); ++i)
            {
                if (m_slots[i].slotArea.contains(msgData.position))
                {
                    m_slots[i].occupierID = msgData.component->getParentUID();
                    m_slots[i].instruction = msgData.component->getInstruction();
                    m_slots[i].targeted = false;                    
                    msgData.component->setStackIndex(i);
                    msgData.component->setPreviousLoop(lastLoop);
                    
                    bool child = (m_slots[i].instruction == Instruction::Forward || m_slots[i].instruction == Instruction::Loop);
                    auto instruction = m_slots[i].instruction;

                    //send command to enable cropping shader
                    xy::Command cmd;
                    cmd.entityID = m_slots[i].occupierID;
                    cmd.action = [child, instruction](xy::Entity& entity, float)
                    {
                        entity.getComponent<xy::SfDrawableComponent<sf::Text>>()->setShaderActive();
                        entity.getComponent<xy::SfDrawableComponent<RoundedRectangle>>()->setShaderActive();

                        if (child) //also activate on input box
                        {
                            auto& chent = *entity.getChildren()[0];
                            chent.getComponent<xy::SfDrawableComponent<RoundedRectangle>>()->setShaderActive();
                            chent.getComponent<xy::SfDrawableComponent<sf::Text>>()->setShaderActive();
                        }
                    };
                    m_parentEntity->getScene()->sendCommand(cmd);

                    break;
                }
                //remember this for next iter
                if (m_slots[i].instruction == Instruction::Loop)
                {
                    lastLoop = i;
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
                bool child = (m_slots[i].instruction == Instruction::Forward || m_slots[i].instruction == Instruction::Loop);
                auto instruction = m_slots[i].instruction;

                m_slots[i].occupierID = 0;
                m_slots[i].instruction = Instruction::NOP;
                m_slots[i].targeted = false;
                msgData.component->setStackIndex(-1);
                updateInstructionCount();

                //send command to disable cropping shader
                xy::Command cmd;
                cmd.entityID = msgData.component->getParentUID();
                cmd.action = [child, instruction](xy::Entity& entity, float)
                {
                    entity.getComponent<xy::SfDrawableComponent<sf::Text>>()->setShaderActive(false);
                    entity.getComponent<xy::SfDrawableComponent<RoundedRectangle>>()->setShaderActive(false);
                    if (child) //also deactivate on input box
                    {
                        auto& chent = *entity.getChildren()[0];
                        chent.getComponent<xy::SfDrawableComponent<RoundedRectangle>>()->setShaderActive(false);
                        chent.getComponent<xy::SfDrawableComponent<sf::Text>>()->setShaderActive(false);
                    }
                };
                m_parentEntity->getScene()->sendCommand(cmd);
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
    //}

}

void StackLogicComponent::scrollHandler(xy::Component* c, const xy::Message& msg)
{
    //else if (msg.id == MessageId::ScrollbarMessage)
    //{
        auto& msgData = msg.getData<ScrollbarEvent>();
        const float distance = msgData.position * m_maxScrollDistance;

        xy::Command cmd;
        cmd.category = CommandCategory::InstructionList;
        cmd.action = [distance](xy::Entity& entity, float)
        {
            auto position = entity.getPosition();
            position.y = -distance;
            entity.setPosition(position);
        };
        m_parentEntity->getScene()->sendCommand(cmd);

        m_updateTransform = true;
    //}
}