//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//menu to host multiplayer game and accept incoming connections

#ifndef MENU_LOBBY_STATE_HPP_
#define MENU_LOBBY_STATE_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>
#include <xygine/Resource.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include <map>

namespace sf
{
    class TcpSocket;
    class Packet;
}

namespace xy
{
    class MessageBus;
}
class MenuLobbyState final : public xy::State
{
public:
    MenuLobbyState(xy::StateStack&, Context);
    ~MenuLobbyState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::MenuLobby;
    }

private:
    xy::MessageBus& m_messageBus;
    xy::UI::Container m_uiContainer;

    std::map<sf::Int16, sf::Text> m_texts;
    sf::Sprite m_cursorSprite;

    sf::Font& m_font;

    xy::TextureResource m_textureResource;
    xy::FontResource m_fontResource;

    sf::TcpSocket* m_socket;
    float m_timeSinceLastPacket;
    sf::Clock m_tickClock;
    void handlePacket(sf::Packet&);

    void buildMenu();
};
#endif //MENU_LOBBY_STATE_HPP_