//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//creates an input window

#ifndef INPUT_WINDOW_HPP_
#define INPUT_WINDOW_HPP_

#include <RoundedRectangle.hpp>

#include <xygine/components/Component.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

#include <string>

class InputWindow final : public xy::Component, public sf::Drawable
{
public:
    InputWindow(xy::MessageBus&);
    ~InputWindow();
    InputWindow(const InputWindow&) = delete;
    InputWindow& operator = (const InputWindow&) = delete;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;

    void handleTextEvent(const sf::Event&);
    void setFont(const sf::Font&);
    void setCharacterSize(sf::Uint32);

    void setTargetId(sf::Uint64);

    sf::FloatRect globalBounds() const override;

private:

    RoundedRectangle m_background;
    sf::RectangleShape m_cursor;
    sf::Text m_text;
    sf::Uint64 m_targetId;
    sf::FloatRect m_globalBounds;
    sf::Int32 m_value;
    std::string m_strValue;

    sf::Clock m_blinkClock;
    bool m_blink;

    bool m_close;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif //INPUT_WINDOW_HPP_