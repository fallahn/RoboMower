//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MenuLobbyState.hpp>
#include <App.hpp>
#include <Log.hpp>
#include <Protocol.hpp>

#include <UIButton.hpp>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Network/Packet.hpp>

namespace
{
    const float clientTimeout = 5.f;
    const float tickRate = 1.f / 20.f;
}

MenuLobbyState::MenuLobbyState(StateStack& stack, Context context)
    : State                 (stack, context),
    m_messageBus            (context.appInstance.getMessageBus()),
    m_font                  (context.appInstance.getFont("dfg")),
    m_socket                (&context.appInstance.getSocket()),
    m_timeSinceLastPacket   (0.f)
{
    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
    buildMenu();

    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.type = Message::UIEvent::MenuOpened;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuLobby;
    m_messageBus.send(msg);
}


//public
bool MenuLobbyState::update(float dt)
{
    //if (getContext().appInstance.connected())
    {
        sf::Packet packet;
        if (m_socket->receive(packet) == sf::Socket::Done)
        {
            m_timeSinceLastPacket = 0.f;
            handlePacket(packet);
        }
        else
        {
            if (m_timeSinceLastPacket > clientTimeout)
            {
                Message msg;
                msg.type = Message::Type::Network;
                msg.network.action = Message::NetworkEvent::RequestDisconnect;
                m_messageBus.send(msg);

                requestStackPop();
                requestStackPush(States::ID::MenuMain);

                m_socket->disconnect();
                LOG("CLIENT lost connection to server", Logger::Type::Info);
            }
        }
        m_timeSinceLastPacket += dt;
    }

    //send client status
    if (m_tickClock.getElapsedTime().asSeconds() > tickRate)
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::LobbyData);
        //TODO send name(?) and ready status
        m_socket->send(packet);
        m_tickClock.restart();
    }

    m_uiContainer.update(dt);
    return true;
}

void MenuLobbyState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);

    rw.draw(m_uiContainer);

    for (const auto& t : m_texts)
    {
        rw.draw(t.second);
    }

    rw.draw(m_cursorSprite);
}

bool MenuLobbyState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);
    return false;
}

void MenuLobbyState::handleMessage(const Message&)
{

}

//private
void MenuLobbyState::handlePacket(sf::Packet& packet)
{
    sf::Int32 type;
    packet >> type;
    switch (type)
    {
    case Server::LobbyData:
    {
        sf::Uint32 count;
        packet >> count;

        sf::Int16 uid;
        for (auto i = 0u; i < count; ++i)
        {
            packet >> uid;
            m_texts[uid] = sf::Text(std::to_string(uid), m_font);
            m_texts[uid].setPosition(200.f, 300.f + (40.f * i));
        }
    }
        break;
        //erase disconnections from texts
    case Server::PlayerDisconnect:
    {
        sf::Int16 uid;
        packet >> uid;
        if (m_texts.find(uid) != m_texts.end())
            m_texts.erase(uid);

        LOG("CLIENT disconnected player " + std::to_string(uid), Logger::Type::Info);
    }
        break;
    case Server::GameStart:
        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = Message::UIEvent::MenuClosed;
        msg.ui.value = 0.f;
        msg.ui.stateId = States::ID::MenuLobby;
        m_messageBus.send(msg);

        requestStackClear();
        requestStackPush(States::ID::Game);
        break;
    default: break;
    }
}

void MenuLobbyState::buildMenu()
{
    const auto& font = getContext().appInstance.getFont("flaps");
    
    auto startButton = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/button.png"));
    startButton->setText("Start");
    startButton->setAlignment(ui::Alignment::Centre);
    startButton->setPosition(840.f, 770.f);
    startButton->setCallback([this]()
    {
        //check we're host and are allowed to do this
        if (getContext().appInstance.hosting())
        {
            //TODO check all clients are ready
            Message msg;
            msg.type = Message::Type::UI;
            msg.ui.type = Message::UIEvent::MenuClosed;
            msg.ui.value = 0.f;
            msg.ui.stateId = States::ID::MenuLobby;
            m_messageBus.send(msg);

            requestStackClear();
            requestStackPush(States::ID::Game);
        }
    });
    m_uiContainer.addControl(startButton);

    auto backButton = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/button.png"));
    backButton->setText("Back");
    backButton->setAlignment(ui::Alignment::Centre);
    backButton->setPosition(1080.f, 770.f);
    backButton->setCallback([this]()
    {
        requestStackPop();

        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = Message::UIEvent::MenuClosed;
        msg.ui.value = 0.f;
        msg.ui.stateId = States::ID::MenuLobby;
        m_messageBus.send(msg);

        msg.type = Message::Type::Network;
        msg.network.action = Message::NetworkEvent::RequestDisconnect;
        m_messageBus.send(msg);

        requestStackPush(States::ID::MenuMain);
    });
    m_uiContainer.addControl(backButton);
}