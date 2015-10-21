//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//makes a Text instance component compatible

#ifndef TEXT_DRAWABLE_HPP_
#define TEXT_DRAWABLE_HPP_

#include <Component.hpp>

#include <SFML/Graphics/Text.hpp>

class TextDrawable final : public Component, public sf::Text
{
public:
    explicit TextDrawable(MessageBus&);
    ~TextDrawable() = default;

    Component::Type type() const override;
    Component::UniqueType uniqueType() const override;
    void entityUpdate(Entity&, float) override;
    void handleMessage(const Message&) override;

    sf::FloatRect localBounds() const override;
    sf::FloatRect globalBounds() const override;

private:

};


#endif //TEXT_DRAWABLE_HPP_