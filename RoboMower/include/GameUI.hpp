//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//handes the in-game UI (not the standard xygine UI components)

#ifndef GAME_UI_HPP_
#define GAME_UI_HPP_

#include <SFML/System/Vector2.hpp>

#include <InstructionSet.hpp>

#include <xygine/State.hpp>
#include <xygine/ShaderResource.hpp>

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
    class TextureResource;
    class FontResource;
}

class GameUI final
{
public:
    GameUI(xy::State::Context, xy::TextureResource&, xy::FontResource&, xy::Scene&);
    ~GameUI() = default;
    GameUI(const GameUI&) = delete;
    GameUI& operator = (const GameUI&) = delete;

    void update(float, const sf::Vector2f& mousePos);
    void handleEvent(const sf::Event&);
    void handleMessage(const xy::Message&);

private:
    xy::ShaderResource m_shaderResource;
    xy::FontResource& m_fontResource;
    xy::TextureResource& m_textureResource;

    xy::State::Context m_stateContext;
    xy::Scene& m_scene;
    xy::MessageBus& m_messageBus;
    xy::Entity* m_mouseCursor;
    xy::Entity* m_instructionStack;
    bool m_inputWindowVisible;
    void addInstructionBlock(const sf::Vector2f& position, const sf::Vector2f& offset, Instruction);
    void showInputWindow(sf::Uint64 destId);
};

#endif //GAME_UI_HPP_