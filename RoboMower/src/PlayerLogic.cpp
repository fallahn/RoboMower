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
#include <InstructionSet.hpp>

#include <xygine/Entity.hpp>
#include <xygine/components/ParticleSystem.hpp>
#include <xygine/util/Vector.hpp>

#include <array>
#include <map>

namespace
{
    //const std::array<sf::Vector2f, 4u> targets =
    //{ 
    //    sf::Vector2f(1056.f, 160.f),
    //    sf::Vector2f(1065.f, 736.f),
    //    sf::Vector2f(224.f, 736.f),
    //    sf::Vector2f(224.f, 160.f)
    //};

    const float movespeed = 200.f;

    //TODO we need to marry this up with what's actually loaded
    const sf::Vector2f tileSize(64.f, 64.f);
    const float rotationTime = 0.5f;

    std::map<Instruction, std::function<bool(xy::Entity&, float)>> instructions;
}

PlayerLogic::PlayerLogic(xy::MessageBus& mb)
    : xy::Component     (mb, this),
    //m_targetIdx         (0u),
    m_clientID          (-1),
    m_currentDirection  (Direction::Right),
    m_transportStatus   (TransportStatus::Stopped),
    m_programCounter    (0),
    m_currentParameter  (0)
{
    instructions.insert(std::make_pair(Instruction::NOP, 
        [this](xy::Entity&, float)
    {return true; }));
    
    instructions.insert(std::make_pair(Instruction::EngineOn,
        [this](xy::Entity& entity, float dt)
    {
        return true;
    }));

    instructions.insert(std::make_pair(Instruction::EngineOff,
        [this](xy::Entity& entity, float dt)
    {
        return true;
    }));

    instructions.insert(std::make_pair(Instruction::Forward,
        [this](xy::Entity& entity, float dt)
    {
        auto path = m_target - entity.getPosition();
        if (xy::Util::Vector::lengthSquared(path) > 2)
        {
            entity.move(xy::Util::Vector::normalise(path) * movespeed * dt);
            return false;
        }
        return true;
    }));

    instructions.insert(std::make_pair(Instruction::Right,
        [this](xy::Entity& entity, float dt)
    {
        if (m_rotationTimer.getElapsedTime().asSeconds() > rotationTime)
        {
            m_rotationTimer.restart();
            m_currentDirection = static_cast<Direction>((static_cast<sf::Uint8>(m_currentDirection) + 1) % static_cast<sf::Uint8>(Direction::Count));
            m_currentParameter--;
            return (m_currentParameter == 0);
        }
        return false;
    }));

    instructions.insert(std::make_pair(Instruction::Left,
        [this](xy::Entity& entity, float dt)
    {
        return true;
    }));

    instructions.insert(std::make_pair(Instruction::Loop,
        [this](xy::Entity& entity, float dt)
    {
        return true;
    }));

    m_currentAction = instructions[Instruction::NOP];
}

//public
void PlayerLogic::entityUpdate(xy::Entity& entity, float dt)
{
    if (m_transportStatus == TransportStatus::Playing)
    {       
        Direction direction = m_currentDirection;
        if(m_currentAction(entity, dt))
        {
            //action completed get next instruction and its parameter
            Instruction instruction = static_cast<Instruction>(m_program[m_programCounter++]);
            m_currentParameter = m_program[m_programCounter++];
            
            //set up inital action values
            switch (instruction)
            {
            default: break;
            case Instruction::EngineOn: break;
            case Instruction::EngineOff: break;
            case Instruction::Forward:
            {
                //calc the target from the current direction and entity position
                m_target = entity.getPosition();
                switch (m_currentDirection)
                {
                default: break;
                case Direction::Left:
                    m_target.x -= tileSize.x * static_cast<float>(m_currentParameter);
                    break;
                case Direction::Right:
                    m_target.x += tileSize.x * static_cast<float>(m_currentParameter);
                    break;
                case Direction::Up:
                    m_target.y -= tileSize.y * static_cast<float>(m_currentParameter);
                    break;
                case Direction::Down:
                    m_target.y += tileSize.y * static_cast<float>(m_currentParameter);
                    break;
                }
            }
                break;
            case Instruction::Right:
            case Instruction::Left:
                m_rotationTimer.restart();
                break;
            }
            //update the current action
            m_currentAction = instructions[instruction];
            if (m_programCounter == m_program.size())
            {
                stop();
                LOG("Finished running program", xy::Logger::Type::Info);
            }
        }

        //check if action changed our direction and message if so
        if (direction != m_currentDirection)
        {
            auto msg = sendMessage<DirectionEvent>(DirectionMessage);
            msg->id = m_clientID;
            msg->direction = m_currentDirection;
        }
    }
}

void PlayerLogic::setClientID(xy::ClientID id)
{
    m_clientID = id;
}

void PlayerLogic::start()
{
    m_transportStatus = TransportStatus::Playing;
}

void PlayerLogic::pause()
{
    m_transportStatus = TransportStatus::Paused;
}

void PlayerLogic::rewind()
{
    if (m_transportStatus != TransportStatus::Playing)
    {
        stop();
        //TODO reset current entity position
    }
}

//private
void PlayerLogic::stop()
{
    m_transportStatus = TransportStatus::Stopped;
    m_programCounter = 0;
    m_currentAction = instructions[Instruction::NOP];
}

//auto path = targets[m_targetIdx] - entity.getPosition();
//if (xy::Util::Vector::lengthSquared(path) > 2)
//{
//    entity.move(xy::Util::Vector::normalise(path) * movespeed * dt);
//}
//else
//{
//    //find new direction and update our entity
//    const auto& oldTarget = targets[m_targetIdx];
//    m_targetIdx = (m_targetIdx + 1) % targets.size();

//    auto msg = sendMessage<DirectionEvent>(DirectionMessage);
//    msg->id = m_clientID;

//    auto direction = targets[m_targetIdx] - oldTarget;
//    bool directionFound = false;
//    if (direction.x > 0)
//    {
//        if (std::abs(direction.y) < direction.x)
//        {
//            //direction is right
//            msg->direction = Direction::Right;
//            directionFound = true;
//        }
//    }
//    else if (direction.x < 0)
//    {
//        if (-std::abs(direction.y) > direction.x)
//        {
//            //direction is left
//            msg->direction = Direction::Left;
//            directionFound = true;
//        }
//    }

//    if (!directionFound)
//    {
//        if (direction.y > 0)
//        {
//            //direction is down
//            msg->direction = Direction::Down;
//        }
//        else
//        {
//            //direction is up
//            msg->direction = Direction::Up;
//        }
//    }
//}