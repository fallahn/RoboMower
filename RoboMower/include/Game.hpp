//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef GAME_HPP_
#define GAME_HPP_

#include <xygine/App.hpp>

#include <network/Server.hpp>

class Game final : public xy::App
{
public:
    Game();
    ~Game() = default;
    Game(const Game&) = delete;
    Game& operator = (const Game&) = delete;

private:

    xy::StateStack m_stateStack;

    Server m_server;

    void handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;

    void registerStates() override;
    void updateApp(float dt) override;
    void pauseApp(float dt) override;
    void draw() override;

    void finalise() override;
};


#endif //GAME_HPP_