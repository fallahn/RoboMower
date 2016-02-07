//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <components/NetworkController.hpp>

#include <xygine/Entity.hpp>
#include <xygine/util/Vector.hpp>

NetworkController::NetworkController(xy::MessageBus& mb)
    : xy::Component(mb, this)
{

}

//public
void NetworkController::entityUpdate(xy::Entity& entity, float dt)
{
    auto movement = m_destination - entity.getPosition();
    if (xy::Util::Vector::lengthSquared(movement) > 1500.f)
    {
        entity.setPosition(m_destination);
    }
    else
    {
        entity.move(movement * 0.1f);
    }
}

void NetworkController::setDestination(const sf::Vector2f& destination)
{
    m_destination = destination;
}

//private