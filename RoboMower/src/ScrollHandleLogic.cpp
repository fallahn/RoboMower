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

#include <components/ScrollHandleLogic.hpp>
#include <Messages.hpp>

#include <xygine/Assert.hpp>
#include <xygine/Entity.hpp>
#include <xygine/MessageBus.hpp>

ScrollHandleLogic::ScrollHandleLogic(xy::MessageBus& mb)
    : xy::Component (mb, this),
    m_carried       (false),
    m_length        (1.f),
    m_position      (0.f),
    m_update        (true)
{
    xy::Component::MessageHandler mh;
    mh.id = MessageId::ScrollbarMessage;
    mh.action = std::bind(&ScrollHandleLogic::handleMessage, this, std::placeholders::_1, std::placeholders::_2);
    addMessageHandler(mh);
}

//public
void ScrollHandleLogic::entityUpdate(xy::Entity& entity, float dt)
{
    //only update if scroll message marked as dirty
    if (m_update)
    {
        auto position = entity.getPosition();

        //clamp position
        if (position.y < 0) position.y = 0;
        else if (position.y > m_length) position.y = m_length;
        entity.setPosition(position);

        //update internals
        m_position = position.y / m_length;
        m_update = false;
    }
}

void ScrollHandleLogic::setCarried(bool c)
{
    m_carried = c;
}

bool ScrollHandleLogic::carried() const
{
    return m_carried;
}

void ScrollHandleLogic::setLength(float len)
{
    XY_ASSERT(len > 0, "scroll length must be greater than 0");
    m_length = len;
}

float ScrollHandleLogic::getPosition() const
{
    return m_position;
}

//private
void ScrollHandleLogic::handleMessage(xy::Component* c, const xy::Message& msg)
{
    //if (msg.id == MessageId::ScrollbarMessage)
    {
        m_update = true;
    }
}