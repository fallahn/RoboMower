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

#include <components/PlayerLogic.hpp>
#include <components/PlayerDrawable.hpp>
#include <Messages.hpp>

#include <xygine/Entity.hpp>
#include <xygine/components/ParticleSystem.hpp>
#include <xygine/util/Vector.hpp>

#include <array>

namespace
{
    const std::array<sf::Vector2f, 4u> targets =
    { 
        sf::Vector2f(1056.f, 160.f),
        sf::Vector2f(1065.f, 736.f),
        sf::Vector2f(224.f, 736.f),
        sf::Vector2f(224.f, 160.f)
    };

    const float movespeed = 200.f;
}

PlayerLogic::PlayerLogic(xy::MessageBus& mb)
    : xy::Component (mb, this),
    m_targetIdx     (0u),
    m_clientID      (-1)
{

}

//public
void PlayerLogic::entityUpdate(xy::Entity& entity, float dt)
{
    auto path = targets[m_targetIdx] - entity.getPosition();
    if (xy::Util::Vector::lengthSquared(path) > 2)
    {
        entity.move(xy::Util::Vector::normalise(path) * movespeed * dt);
    }
    else
    {        
        //find new direction and update our entity
        const auto& oldTarget = targets[m_targetIdx];        
        m_targetIdx = (m_targetIdx + 1) % targets.size();
        
        auto msg = sendMessage<DirectionEvent>(DirectionMessage);
        msg->id = m_clientID;

        auto direction = targets[m_targetIdx] - oldTarget;
        bool directionFound = false;
        if (direction.x > 0)
        {
            if (std::abs(direction.y) < direction.x)
            {
                //direction is right
                msg->direction = Direction::Right;
                directionFound = true;
            }
        }
        else if (direction.x < 0)
        {
            if (-std::abs(direction.y) > direction.x)
            {
                //direction is left
                msg->direction = Direction::Left;
                directionFound = true;
            }
        }

        if (!directionFound)
        {
            if (direction.y > 0)
            {
                //direction is down
                msg->direction = Direction::Down;
            }
            else
            {
                //direction is up
                msg->direction = Direction::Up;
            }
        }
    }
}

void PlayerLogic::setClientID(xy::ClientID id)
{
    m_clientID = id;
}