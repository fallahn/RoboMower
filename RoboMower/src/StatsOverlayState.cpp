/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

xygine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <xygine/StatsOverlayState.hpp>
#include <xygine/Reports.hpp>
#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

StatsOverlayState::StatsOverlayState(StateStack& ss, Context c)
    : State(ss, c)
{
    c.renderWindow.setMouseCursorVisible(true);

    const auto& font = c.appInstance.getFont("assets/fonts/Console.ttf");

    ui::Window::Palette palette;
    palette.background = { 0u, 20u, 120u, 190u };
    palette.borderActive = { 0u, 20u, 190u };
    palette.borderNormal = { 0u, 15u, 120u };

    m_windows.emplace_back(std::make_unique<ui::Window>(font, 640, 480, palette));
    auto& window = m_windows.back();
    window->setPosition(20.f, 60.f);
    window->setTitle("Stats");

    m_statsText = std::make_shared<ui::Label>(font);
    m_statsText->setPosition(20.f, 20.f);
    m_statsText->setString(StatsReporter::reporter.getString());
    m_statsText->setCharacterSize(26u);
    window->addControl(m_statsText);

    m_windows.emplace_back(std::make_unique<ui::Window>(font, 840, 880, palette));
    auto& otherwindow = m_windows.back();
    otherwindow->setPosition(700.f, 60.f);
    otherwindow->setTitle("Console");

    m_consoleText = std::make_shared<ui::Label>(font);
    m_consoleText->setPosition(20.f, 20.f);
    m_consoleText->setString(Logger::bufferString());
    m_consoleText->setCharacterSize(26u);
    otherwindow->addControl(m_consoleText);
}

StatsOverlayState::~StatsOverlayState()
{
    getContext().renderWindow.setMouseCursorVisible(false);
}

//public
bool StatsOverlayState::update(float dt)
{
    m_statsText->setString(StatsReporter::reporter.getString());
    m_consoleText->setString(Logger::bufferString());
    for (auto& w : m_windows)
    {
        w->update(dt);
    }
    return true;
}

bool StatsOverlayState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyReleased)
    {
        switch (evt.key.code)
        {
        case sf::Keyboard::F11:
            requestStackPop();
            break;
        default:break;
        }
    }
    
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    for (auto& w : m_windows)
    {
        w->handleEvent(evt, mousePos);
    }
    return false;
}

void StatsOverlayState::handleMessage(const Message&)
{}

void StatsOverlayState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);

    for (const auto& w : m_windows)
        rw.draw(*w);
}