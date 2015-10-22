//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//menu to host multiplayer game and accept incoming connections

#ifndef MENU_LOBBY_STATE_HPP_
#define MENU_LOBBY_STATE_HPP_

#include <xygine/State.hpp>
#include <xygine/ui/UIContainer.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include <map>

namespace sf
{
    class TcpSocket;
    class Packet;
}

class MessageBus;
class MenuLobbyState final : public State
{
public:
    MenuLobbyState(StateStack&, Context);
    ~MenuLobbyState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::MenuLobby;
    }

private:
    MessageBus& m_messageBus;
    ui::Container m_uiContainer;

    std::map<sf::Int16, sf::Text> m_texts;
    sf::Sprite m_cursorSprite;

    sf::Font& m_font;

    sf::TcpSocket* m_socket;
    float m_timeSinceLastPacket;
    sf::Clock m_tickClock;
    void handlePacket(sf::Packet&);

    void buildMenu();
};

#endif //MENU_LOBBY_STATE_HPP_