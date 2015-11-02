//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/CircleDrawable.hpp>

#include <xygine/Entity.hpp>

CircleDrawable::CircleDrawable(xy::MessageBus& mb)
    : xy::Component(mb, this) {}

//public
void CircleDrawable::entityUpdate(xy::Entity& ent, float)
{
    const float width = getRadius() * 2.f;
    m_localBounds = { -getOrigin(), {width, width} };
    m_globalBounds = ent.getWorldTransform().transformRect(m_localBounds);
}

void CircleDrawable::handleMessage(const xy::Message&){}

sf::FloatRect CircleDrawable::globalBounds() const
{
    return m_globalBounds;
}

sf::FloatRect CircleDrawable::localBounds() const
{
    return m_localBounds;
}