//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef CIRCLE_DRAWABLE_HPP_
#define CIRCLE_DRAWABLE_HPP_

#include <SFML/Graphics/CircleShape.hpp>

#include <xygine/Component.hpp>

class CircleDrawable final : public xy::Component, public sf::CircleShape
{
public:
    explicit CircleDrawable(xy::MessageBus&);
    ~CircleDrawable() = default; //HM. is this right? using pointers to components may not neccesarily properly destroy circleshape

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const xy::Message&) override;

    sf::FloatRect globalBounds() const override;
    sf::FloatRect localBounds() const override;
private:
    sf::FloatRect m_localBounds;
    sf::FloatRect m_globalBounds;

};

#endif //CIRCLE_DRAWABLE_HPP_
