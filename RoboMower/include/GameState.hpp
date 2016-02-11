//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//main state of the game

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include <StateIds.hpp>
#include <InstructionSet.hpp>
#include <GameUI.hpp>

#include <xygine/State.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Scene.hpp>
#include <xygine/Resource.hpp>
#include <xygine/ui/Window.hpp>
#include <xygine/network/ClientConnection.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <map>

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

    xy::TextureResource m_textureResource;
    xy::FontResource m_fontResource;

    GameUI m_gameUI;
    xy::Network::ClientConnection m_connection;

    xy::UI::Window m_reportWindow;

    std::map<xy::ClientID, xy::Entity*> m_playerEntities;

    xy::Network::ClientConnection::PacketHandler m_packetHandler;
    void handlePacket(xy::Network::PacketType type, sf::Packet& packet, xy::Network::ClientConnection* connection);

    void buildMap();
};

#endif //GAME_STATE_HPP_