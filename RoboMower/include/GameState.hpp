//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//main state of the game

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include <StateIds.hpp>
#include <InstructionSet.hpp>

#include <xygine/State.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Scene.hpp>
#include <xygine/AudioManager.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/TcpSocket.hpp>

namespace sf
{
    class Color;
}

class GameState final : public xy::State
{
public:
    GameState(xy::StateStack& stateStack, Context context);
    ~GameState() = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::Game;
    }
private:

    xy::MessageBus& m_messageBus;
    xy::Scene m_scene;
    xy::AudioManager m_audioManager;

    void buildUI();
    void addInstructionBlock(const sf::Vector2f& position, const sf::Vector2f& offset, Instruction);
};

#endif //GAME_STATE_HPP_