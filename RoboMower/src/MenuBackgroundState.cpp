//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MenuBackgroundState.hpp>

#include <xygine/App.hpp>

namespace
{
    const std::string version("version 0.0.1");
}

MenuBackgroundState::MenuBackgroundState(xy::StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    m_texts.emplace_back(version, context.appInstance.getFont("assets/fonts/VeraMono.ttf"), 18u);
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