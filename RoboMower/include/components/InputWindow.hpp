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