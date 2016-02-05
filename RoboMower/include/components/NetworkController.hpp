//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_NETWORK_CONTROLLER_HPP_
#define RM_NETWORK_CONTROLLER_HPP_

#include <xygine/components/Component.hpp>

class NetworkController final : public xy::Component
{
public:
    explicit NetworkController(xy::MessageBus&);
    ~NetworkController() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;

    void setDestination(const sf::Vector2f&);

private:

    sf::Vector2f m_destination;

};

#endif // RM_NETWORK_CONTROLLER_HPP_
