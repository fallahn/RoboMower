//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//creates an input window

#ifndef INPUT_WINDOW_HPP_
#define INPUT_WINDOW_HPP_

#include <RoundedRectangle.hpp>

#include <xygine/Component.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class InputWindow final : public xy::Component, public sf::Drawable
{
public:
    InputWindow(xy::MessageBus&);
    ~InputWindow() = default;
    InputWindow(const InputWindow&) = delete;
    InputWindow& operator = (const InputWindow&) = delete;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const xy::Message&) override;

    void handleTextEvent(const sf::Event&);
    void setFont(const sf::Font&);
    void setCharacterSize(sf::Uint32);

private:

    RoundedRectangle m_background;
    sf::RectangleShape m_cursor;
    sf::Text m_text;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif //INPUT_WINDOW_HPP_