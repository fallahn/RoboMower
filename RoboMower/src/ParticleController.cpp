//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/ParticleController.hpp>
#include <xygine/ParticleSystem.hpp>
#include <xygine/Entity.hpp>
#include <xygine/MessageBus.hpp>
#include <xygine/Scene.hpp>

namespace
{
    std::vector<sf::Vector2f> explodeVelocities =
    {
        { -80.5f, 0.f },
        { -60.f, -8.9f },
        { -20.f, -24.f },
        { 0.f, -40.5f },
        { 28.5f, -24.6f },
        { 64.f, -8.5f },
        { 80.9f, 0.f },
        { 64.f, 9.5f },
        { 28.f, 27.5f },
        { 0.7f, 40.4f },
        { -20.f, 29.6f },
        { -60.f, 9.5f }
    };
}

ParticleController::ParticleController(MessageBus& mb)
    : Component (mb),
    m_entity    (nullptr)
{

}

//public
Component::Type ParticleController::type() const
{
    return Component::Type::Script;
}

Component::UniqueType ParticleController::uniqueType() const
{
    return Component::UniqueType::ParticleController;
}

void ParticleController::entityUpdate(Entity&, float){}

void ParticleController::handleMessage(const Message& msg)
{
    switch (msg.type)
    {
    case Message::Type::Player:

        break;
    default: break;
    }
}

void ParticleController::onStart(Entity& entity)
{
    m_entity = &entity;
}

void ParticleController::explosion(const sf::Vector2f& position, const sf::Color& colour)
{
    auto& children = m_entity->getChildren();
    auto child = std::find_if(children.begin(), children.end(), [](const Entity::Ptr& p)
    {
        return !p->getComponent<ParticleSystem>(Component::UniqueType::ParticleSystem)->active();
    });

    if (child != children.end())
    {
        (*child)->setWorldPosition(position);
        (*child)->getComponent<ParticleSystem>(Component::UniqueType::ParticleSystem)->start(5, 0.f, 0.1f);
        (*child)->getComponent<ParticleSystem>(Component::UniqueType::ParticleSystem)->setColour(colour);
        //LOG("CLIENT fired existing explosion particles", Logger::Type::Info);
    }
    else
    {
        auto ent = std::make_unique<Entity>(getMessageBus());
        ent->setPosition(position);
        auto system = std::make_unique<ParticleSystem>(getMessageBus());
        system->setRandomInitialVelocity(explodeVelocities);
        system->followParent(true);
        ForceAffector fa({ 0.f, 150.f });
        system->addAffector(fa);
        system->start(5, 0.f, 0.1f);
        system->setColour(colour);
        ent->addComponent<ParticleSystem>(system);
        m_entity->addChild(ent);
        //LOG("CLIENT fired new explosion particles", Logger::Type::Info);
    }
}