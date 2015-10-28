//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//handes the in-game UI (not the standard xygine UI components)

#ifndef GAME_UI_HPP_
#define GAME_UI_HPP_

#include <SFML/System/Vector2.hpp>

#include <InstructionSet.hpp>

#include <xygine/State.hpp>

namespace sf
{
    class Event;
}

namespace xy
{
    class Scene;
    class Message;
    class Entity;
    class MessageBus;
    class App;
}

class GameUI final
{
public:
    GameUI(xy::State::Context, xy::Scene&);
    ~GameUI() = default;
    GameUI(const GameUI&) = delete;
    GameUI& operator = (const GameUI&) = delete;

    void update(float, const sf::Vector2f& mousePos);
    void handleEvent(const sf::Event&);
    void handleMessage(const xy::Message&);

private:
    xy::State::Context m_stateContext;
    xy::Scene& m_scene;
    xy::MessageBus& m_messageBus;
    xy::Entity* m_mouseCursor;
    void addInstructionBlock(const sf::Vector2f& position, const sf::Vector2f& offset, Instruction);
};

#endif //GAME_UO_HPP_