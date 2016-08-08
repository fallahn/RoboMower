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

#include <components/InstructionBlockLogic.hpp>
#include <Messages.hpp>

#include <xygine/Entity.hpp>
#include <xygine/util/Vector.hpp>
#include <xygine/Reports.hpp>

namespace
{
    const float speed = 14.f;
}

InstructionBlockLogic::InstructionBlockLogic(xy::MessageBus& mb, Instruction inst)
    : Component         (mb, this),
    m_state             (State::Carried),
    m_instruction       (inst),
    m_value             (0),
    m_targetIndex       (-1),
    m_destroyWhenDone   (true),
    m_stackIndex        (-1),
    m_previousStackindex(-1),
    m_entity            (nullptr)
{
    xy::Component::MessageHandler mh;
    mh.id = InputBoxMessage;
    mh.action = [this](xy::Component* c, const xy::Message& msg)
    {
        const auto& msgData = msg.getData<InputBoxEvent>();
        if (msgData.action == InputBoxEvent::WindowClosed)
        {
            //see if our child was targetted and store a copy of the value
            auto targetEnt = msgData.entityId;
            const auto& children = m_entity->getChildren();
            const auto& child = std::find_if(children.begin(), children.end(), 
                [targetEnt](const xy::Entity::Ptr& e)
            {
                return e->getUID() == targetEnt;
            });

            if (child != children.end())
            {
                m_value = sf::Uint8(msgData.value);
                LOG("Set instruction block value to " + std::to_string(m_value), xy::Logger::Type::Info);
            }
        }
    };
    addMessageHandler(mh);
}

//public
void InstructionBlockLogic::entityUpdate(xy::Entity& entity, float dt)
{
    switch (m_state)
    {
    case State::Carried:
    case State::Idle:
    default: break;
    case State::Homing:
        auto dir = m_target - entity.getWorldPosition();
        const auto len = xy::Util::Vector::lengthSquared(dir);
        if (len < 2.5f)
        {
            m_state = State::Idle;
            entity.setWorldPosition(m_target);

            //raise event to say we were dropped or destroyed
            auto msg = sendMessage<InstructionBlockEvent>(MessageId::InstructionBlockMessage);        
            msg->position = m_target;

            if (m_destroyWhenDone)
            {
                entity.destroy();
                msg->action = InstructionBlockEvent::Destroyed;
                msg->component = nullptr;
            }
            else
            {
                msg->action = InstructionBlockEvent::Dropped; 
                msg->component = this;
            }
        }
        else
        {
            entity.move(dir * speed * dt);
        }
        break;
    }
}

void InstructionBlockLogic::onDelayedStart(xy::Entity& entity)
{
    m_entity = &entity;
    XY_ASSERT(m_entity, "failed to get parent entity");
}

void InstructionBlockLogic::setCarried(bool c)
{
    m_state = (c) ? State::Carried : State::Homing;

    if (c)
    {
        auto msg = sendMessage<InstructionBlockEvent>(MessageId::InstructionBlockMessage);
        msg->action = InstructionBlockEvent::PickedUp;
        msg->component = this;
        msg->position = m_target;
    }
}

bool InstructionBlockLogic::carried() const
{
    return (m_state == State::Carried);
}

void InstructionBlockLogic::setTarget(const sf::Vector2f& target, bool destroyWhenDone)
{
    m_target = target;
    m_destroyWhenDone = destroyWhenDone;
}

void InstructionBlockLogic::setTargetIndex(sf::Int32 idx)
{
    m_targetIndex = idx;
}

sf::Int32 InstructionBlockLogic::getTargetIndex() const
{
    return m_targetIndex;
}

void InstructionBlockLogic::setCursorOffset(const sf::Vector2f& offset)
{
    m_cursorOffset = offset;
}

const sf::Vector2f& InstructionBlockLogic::getCursorOffset() const
{
    return m_cursorOffset;
}

void InstructionBlockLogic::setStackIndex(sf::Int32 idx)
{
    m_previousStackindex = m_stackIndex;
    m_stackIndex = idx;
}

sf::Int32 InstructionBlockLogic::getStackIndex() const
{
    return m_stackIndex;
}

sf::Int32 InstructionBlockLogic::getPreviousStackIndex() const
{
    return m_previousStackindex;
}
