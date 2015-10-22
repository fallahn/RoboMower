//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//manages spawning of particle systems on parent entity

#ifndef PARTICLE_CONTROLLER_HPP_
#define PARTICLE_CONTROLLER_HPP_

#include <xygine/Component.hpp>

#include <SFML/Graphics/Color.hpp>

#include <vector>

class ParticleSystem;
class ParticleController final : public Component
{
public:
    explicit ParticleController(MessageBus&);
    ~ParticleController() = default;

    Component::Type type() const override;
    Component::UniqueType uniqueType() const override;
    void entityUpdate(Entity&, float) override;
    void handleMessage(const Message&) override;
    void onStart(Entity&);

    void explosion(const sf::Vector2f&, const sf::Color&  = sf::Color::White);
private:

    Entity* m_entity;
};

#endif //PARTICLE_CONTROLLER_HPP_