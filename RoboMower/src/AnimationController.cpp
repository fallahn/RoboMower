//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <AnimationController.hpp>
#include <MessageBus.hpp>
#include <Entity.hpp>
#include <AnimatedDrawable.hpp>

#include <cassert>

AnimationController::AnimationController(MessageBus& mb)
    : Component (mb),
    m_drawable  (nullptr)
{}

//public
Component::Type AnimationController::type() const
{
    return Component::Type::Script;
}

Component::UniqueType AnimationController::uniqueType() const
{
    return Component::UniqueType::AnimationController;
}

void AnimationController::entityUpdate(Entity&, float){}

void AnimationController::handleMessage(const Message& msg)
{
    if (msg.type == Message::Type::Entity 
        && msg.entity.entity->getUID() == getParentUID())
    {


    }
}

void AnimationController::onStart(Entity& entity)
{
    m_drawable = entity.getComponent<AnimatedDrawable>(Component::UniqueType::AnimatedDrawable);
    assert(m_drawable);
}