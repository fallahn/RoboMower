//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <TextDrawable.hpp>

TextDrawable::TextDrawable(MessageBus& mb)
    : Component(mb){}

Component::Type TextDrawable::type() const
{
    return Component::Type::Drawable;
}

Component::UniqueType TextDrawable::uniqueType() const
{
    return Component::UniqueType::TextDrawable;
}

void TextDrawable::entityUpdate(Entity&, float){}
void TextDrawable::handleMessage(const Message&){}

sf::FloatRect TextDrawable::localBounds() const
{
    return getLocalBounds();
}

sf::FloatRect TextDrawable::globalBounds() const
{
    return getGlobalBounds();
}