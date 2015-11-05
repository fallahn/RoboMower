//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <Game.hpp>
#include <StateIds.hpp>
#include <GameState.hpp>
#include <MenuBackgroundState.hpp>
#include <MenuJoinState.hpp>
#include <MenuLobbyState.hpp>
#include <MenuMainState.hpp>
#include <MenuOptionState.hpp>
#include <MenuPauseState.hpp>
#include <StatsOverlayState.hpp>

#include <SFML/Window/Event.hpp>

namespace
{
    sf::Color clearColour(40u, 40u, 40u);
}

Game::Game()
    : m_stateStack({ getRenderWindow(), *this })
{
    registerStates();

#ifndef _DEBUG_
    //normally intro
    m_stateStack.pushState(States::ID::MenuMain);
#else
    m_stateStack.pushState(States::ID::MenuBackground);
    m_stateStack.pushState(States::ID::MenuMain);
#endif //_DEBUG_
}

//private
void Game::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyReleased)
    {
        switch (evt.key.code)
        {
#ifdef _DEBUG_
        case sf::Keyboard::PageUp:
            m_stateStack.pushState(States::ID::Stats);
            break;
#endif //_DEBUG_
        default: break;
        }
    }    
    
    m_stateStack.handleEvent(evt);
}

void Game::handleMessage(const xy::Message& msg)
{
    switch (msg.id)
    {
    case xy::Message::Type::NetworkMessage:
    {
        auto& msgData = msg.getData<xy::Message::NetworkEvent>();
        switch (msgData.action)
        {
        case xy::Message::NetworkEvent::RequestStartServer:
            if (createLocalServer())
            {
                m_stateStack.pushState(msgData.stateID);
            }
            break;
        default: break;
        }
        break;
    }
    case xy::Message::Type::UIMessage:
    {
        auto& msgData = msg.getData<xy::Message::UIEvent>();
        switch (msgData.type)
        {
        case xy::Message::UIEvent::ResizedWindow:
            m_stateStack.updateView();
            break;
        default: break;
        }
        break;
    }
    default: break;
    }
    
    m_stateStack.handleMessage(msg);
}

void Game::updateApp(float dt)
{
    m_stateStack.update(dt);
}

void Game::pauseApp(float dt)
{
    //do nothing, we're paused!
}

void Game::draw()
{
    getRenderWindow().clear(clearColour);
    m_stateStack.draw();
    getRenderWindow().display();
}

void Game::finalise()
{
    m_stateStack.clearStates();
    m_stateStack.applyPendingChanges();
}

void Game::registerStates()
{
    m_stateStack.registerState<MenuBackgroundState>(States::ID::MenuBackground);
    m_stateStack.registerState<MenuMainState>(States::ID::MenuMain);
    m_stateStack.registerState<MenuLobbyState>(States::ID::MenuLobby);
    m_stateStack.registerState<MenuJoinState>(States::ID::MenuJoin);
    m_stateStack.registerState<MenuOptionState>(States::ID::MenuOptions);
    m_stateStack.registerState<MenuPauseState>(States::ID::MenuPaused);
    m_stateStack.registerState<GameState>(States::ID::Game);
    m_stateStack.registerState<StatsOverlayState>(States::ID::Stats);
}
