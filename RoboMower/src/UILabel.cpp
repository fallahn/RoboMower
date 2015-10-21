//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <UILabel.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

using namespace ui;

Label::Label(const sf::Font& font)
    : m_text            ("", font),
    m_currentAlignment  (Alignment::TopLeft)
{

}

//public
bool Label::selectable() const
{
    return false;
}

void Label::handleEvent(const sf::Event&, const sf::Vector2f&){}

void Label::setAlignment(Alignment alignment)
{
    auto bounds = m_text.getLocalBounds();
    switch (alignment)
    {
    case Alignment::BottomLeft:
        setOrigin(0.f, bounds.height);
        break;
    case Alignment::BottomRight:
        setOrigin(bounds.width, bounds.height);
        break;
    case Alignment::Centre:
        Util::Position::centreOrigin(m_text);
        break;
    case Alignment::TopLeft:
        setOrigin({});
        break;
    case Alignment::TopRight:
        setOrigin(bounds.width, 0.f);
        break;
    default: break;
    }

    m_currentAlignment = alignment;
}

void Label::setString(const std::string& str)
{
    m_text.setString(str);
    updateText();
}

void Label::setColour(const sf::Color& colour)
{
    m_text.setColor(colour);
}

void Label::setCharacterSize(sf::Uint32 size)
{
    m_text.setCharacterSize(size);
    updateText();
}

//private
void Label::updateText()
{
    setAlignment(m_currentAlignment);
}

void Label::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_text, states);
}