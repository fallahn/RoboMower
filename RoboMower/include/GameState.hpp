//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//main state of the game

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include <State.hpp>
#include <Entity.hpp>
#include <Scene.hpp>
#include <AudioManager.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Network/TcpSocket.hpp>

namespace sf
{
    class Color;
}

class NetworkController;
class GameState final : public State
{
public:
    GameState(StateStack& stateStack, Context context);
    ~GameState() = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::Game;
    }
private :

    MessageBus& m_messageBus;
    Scene m_scene;
    AudioManager m_audioManager;
};

#endif //GAME_STATE_HPP_