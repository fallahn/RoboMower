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

#include <MenuBackgroundState.hpp>

#include <xygine/App.hpp>

namespace
{
    const std::string version("version 0.0.1");
}

MenuBackgroundState::MenuBackgroundState(xy::StateStack& stack, Context context)
    : State         (stack, context),
    m_messageBus    (context.appInstance.getMessageBus()),
    m_uiContainer   (m_messageBus)
{
    context.appInstance.setMouseCursorVisible(false);

    m_texts.emplace_back(version, m_fontResource.get("assets/fonts/VeraMono.ttf"), 18u);
    m_texts.back().setPosition(10.f, 1050.f);
}

//public
bool MenuBackgroundState::update(float dt)
{
    return true;
}

void MenuBackgroundState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);;

    for (const auto& t : m_texts)
        rw.draw(t);
}

bool MenuBackgroundState::handleEvent(const sf::Event&)
{
    return true;
}

void MenuBackgroundState::handleMessage(const xy::Message&)
{

}