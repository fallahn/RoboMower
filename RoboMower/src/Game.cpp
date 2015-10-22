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

#include <sfml/window/Event.hpp>

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
        case sf::Keyboard::BackSpace:
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
    switch (msg.type)
    {
    case xy::Message::Type::Network:
        switch (msg.network.action)
        {
        case xy::Message::NetworkEvent::RequestStartServer:
            if (createLocalServer())
            {
                m_stateStack.pushState(msg.network.stateID);
            }
            break;
        default: break;
        }
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
    getRenderWindow().clear(sf::Color::Black);
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