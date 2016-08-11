/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#include <components/InputWindow.hpp>
#include <Messages.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Window/Event.hpp>

#include <xygine/util/Position.hpp>
#include <xygine/Entity.hpp>

namespace
{
    const sf::Vector2f size(200.f, 80.f);
    const sf::Color fillColour(180u, 40u, 20u, 180u);
    const sf::Color borderColour(15u, 30u, 100u);
    const float blinkTime = 0.7f;
    const float cursorPadding = 8.f;
    const float cursorTop = -40.f;
}

InputWindow::InputWindow(xy::MessageBus& mb)
    : xy::Component (mb, this),
    m_targetId      (0),
    m_value         (1),
    m_blink         (true),
    m_close         (false)
{
    m_background.setSize(size);
    m_background.setFillColor(fillColour);
    m_background.setOutlineThickness(10.f);
    m_background.setOutlineColor(borderColour);
    xy::Util::Position::centreOrigin(m_background);

    m_text.setPosition(-56.f, -60.f);
    m_text.setString("1");

    m_cursor.setSize({ 10.f, 58.f });
    m_cursor.setPosition(-1.f, cursorTop);
}

InputWindow::~InputWindow()
{
    auto msg = sendMessage<InputBoxEvent>(MessageId::InputBoxMessage);
    msg->action = InputBoxEvent::WindowClosed;
    msg->entityId = m_targetId;
    msg->value = m_value;
}

//public
void InputWindow::entityUpdate(xy::Entity& entity, float)
{
    m_globalBounds = entity.getWorldTransform().transformRect(m_background.getGlobalBounds());

    if (m_blinkClock.getElapsedTime().asSeconds() > blinkTime)
    {
        m_blink = !m_blink;
        m_blinkClock.restart();
        sf::Color c = (m_blink) ? sf::Color::White : sf::Color::Transparent;
        m_cursor.setFillColor(c);
    }

    if (m_close) entity.destroy();
}

void InputWindow::handleTextEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        if (evt.key.code == sf::Keyboard::BackSpace && !m_strValue.empty())
        {
            m_strValue.pop_back();
        }
        else if (evt.key.code == sf::Keyboard::Return)
        {
            //close input
            m_close = true;
        }
    }
    else
    {
        if (m_strValue.size() < 2)
        {
            //only want numeric input, and only 0 when second digit
            if (evt.text.unicode > (48 - m_strValue.size()) && evt.text.unicode < 58)
            {
                m_strValue += static_cast<char>(evt.text.unicode);
            }
        }
    }

    //update m_value
    m_value = (m_strValue.empty()) ? 1 : std::atoi(m_strValue.c_str());
    m_text.setString(m_strValue);

    auto bounds = m_text.getGlobalBounds();
    float posX = bounds.left + bounds.width + cursorPadding;
    m_cursor.setPosition(posX, cursorTop);
}

void InputWindow::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

void InputWindow::setCharacterSize(sf::Uint32 size)
{
    m_text.setCharacterSize(size);
}

void InputWindow::setTargetId(sf::Uint64 id)
{
    m_targetId = id;
}

sf::FloatRect InputWindow::globalBounds() const
{
    return m_globalBounds;
}

//private
void InputWindow::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.draw(m_background, states);
    rt.draw(m_text, states);
    rt.draw(m_cursor, states);
}