//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <StatsOverlayState.hpp>

#include <xygine/Reports.hpp>
#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

StatsOverlayState::StatsOverlayState(xy::StateStack& ss, Context c)
    : State(ss, c)
{
    c.renderWindow.setMouseCursorVisible(true);

    const auto& font = m_fontResource.get("assets/fonts/Console.ttf");

    xy::UI::Window::Palette palette;
    palette.background = { 0u, 20u, 120u, 190u };
    palette.borderActive = { 0u, 20u, 190u };
    palette.borderNormal = { 0u, 15u, 120u };

    m_windows.emplace_back(std::make_unique<xy::UI::Window>(c.renderWindow, font, 640, 480, palette));
    auto& window = m_windows.back();
    window->setPosition(20.f, 60.f);
    window->setTitle("Stats");

    m_statsText = std::make_shared<xy::UI::Label>(font);
    m_statsText->setPosition(20.f, 20.f);
    m_statsText->setString(xy::StatsReporter::reporter.getString());
    m_statsText->setCharacterSize(26u);
    window->addControl(m_statsText);

    m_windows.emplace_back(std::make_unique<xy::UI::Window>(c.renderWindow, font, 840, 880, palette));
    auto& otherwindow = m_windows.back();
    otherwindow->setPosition(700.f, 60.f);
    otherwindow->setTitle("Console");

    m_consoleText = std::make_shared<xy::UI::Label>(font);
    m_consoleText->setPosition(20.f, 20.f);
    m_consoleText->setString(xy::Logger::bufferString());
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
    m_statsText->setString(xy::StatsReporter::reporter.getString());
    m_consoleText->setString(xy::Logger::bufferString());
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
        case sf::Keyboard::PageUp:
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

void StatsOverlayState::handleMessage(const xy::Message&)
{}

void StatsOverlayState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);

    for (const auto& w : m_windows)
        rw.draw(*w);
}