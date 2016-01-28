//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <components/PlayerLogic.hpp>
#include <components/PlayerDrawable.hpp>

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
    m_targetIdx     (0u)
{

}

//public
void PlayerLogic::entityUpdate(xy::Entity& entity, float dt)
{
    //auto path = targets[m_targetIdx] - entity.getPosition();
    //if (xy::Util::Vector::lengthSquared(path) > 2)
    //{
    //    entity.move(xy::Util::Vector::normalise(path) * movespeed * dt);
    //}
    //else
    //{
    //    auto particles = entity.getComponents<xy::ParticleSystem>();
    //    for (auto& ps : particles) ps->stop();
    //    
    //    //find new direction and update our entity
    //    const auto& oldTarget = targets[m_targetIdx];        
    //    m_targetIdx = (m_targetIdx + 1) % targets.size();
    //    
    //    auto direction = targets[m_targetIdx] - oldTarget;
    //    bool directionFound = false;
    //    if (direction.x > 0)
    //    {
    //        if (std::abs(direction.y) < direction.x)
    //        {
    //            //direction is right
    //            entity.getComponent<xy::ParticleSystem>("particle_right")->start();
    //            entity.getComponent<PlayerDrawable>()->setDirection(PlayerDrawable::Right);
    //            directionFound = true;
    //        }
    //    }
    //    else if (direction.x < 0)
    //    {
    //        if (-std::abs(direction.y) > direction.x)
    //        {
    //            //direction is left
    //            entity.getComponent<xy::ParticleSystem>("particle_left")->start();
    //            entity.getComponent<PlayerDrawable>()->setDirection(PlayerDrawable::Left);
    //            directionFound = true;
    //        }
    //    }

    //    if (!directionFound)
    //    {
    //        if (direction.y > 0)
    //        {
    //            //direction is down
    //            entity.getComponent<xy::ParticleSystem>("particle_down")->start();
    //            entity.getComponent<PlayerDrawable>()->setDirection(PlayerDrawable::Down);
    //        }
    //        else
    //        {
    //            //direction is up
    //            entity.getComponent<xy::ParticleSystem>("particle_up")->start();
    //            entity.getComponent<PlayerDrawable>()->setDirection(PlayerDrawable::Up);
    //        }
    //    }
    //}
}