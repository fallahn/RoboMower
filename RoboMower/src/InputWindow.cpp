//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/InputWindow.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace
{
    //TODO layout params
}

InputWindow::InputWindow(xy::MessageBus& mb)
    : xy::Component(mb, this)
{

}

//public
void InputWindow::entityUpdate(xy::Entity&, float)
{

}

void InputWindow::handleMessage(const xy::Message&)
{

}

void InputWindow::handleTextEvent(const sf::Event&)
{

}

void InputWindow::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

void InputWindow::setCharacterSize(sf::Uint32 size)
{
    m_text.setCharacterSize(size);
}

//private
void InputWindow::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.draw(m_background, states);
    rt.draw(m_text, states);
    rt.draw(m_cursor, states);
}