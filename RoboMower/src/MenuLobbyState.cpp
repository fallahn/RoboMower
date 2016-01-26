//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <MenuLobbyState.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>
#include <xygine/Protocol.hpp>

#include <xygine//ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Network/Packet.hpp>

namespace
{
    const float clientTimeout = 5.f;
    const float tickRate = 1.f / 20.f;
}

MenuLobbyState::MenuLobbyState(xy::StateStack& stack, Context context)
    : State                 (stack, context),
    m_messageBus            (context.appInstance.getMessageBus()),
    m_font                  (m_fontResource.get("dfg")),
    m_socket                (&context.appInstance.getSocket()),
    m_timeSinceLastPacket   (0.f)
{
    m_cursorSprite.setTexture(m_textureResource.get("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
    buildMenu();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateId = States::ID::MenuLobby;
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
                auto msg = m_messageBus.post<xy::Message::NetworkEvent>(xy::Message::NetworkMessage);
                msg->action = xy::Message::NetworkEvent::RequestDisconnect;

                requestStackPop();
                requestStackPush(States::ID::MenuMain);

                m_socket->disconnect();
                LOG("CLIENT lost connection to server", xy::Logger::Type::Info);
            }
        }
        m_timeSinceLastPacket += dt;
    }

    //send client status
    if (m_tickClock.getElapsedTime().asSeconds() > tickRate)
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(xy::Client::LobbyData);
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

void MenuLobbyState::handleMessage(const xy::Message&)
{

}

//private
void MenuLobbyState::handlePacket(sf::Packet& packet)
{
    sf::Int32 type;
    packet >> type;
    switch (type)
    {
    case xy::Server::LobbyData:
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
    case xy::Server::PlayerDisconnect:
    {
        sf::Int16 uid;
        packet >> uid;
        if (m_texts.find(uid) != m_texts.end())
            m_texts.erase(uid);

        LOG("CLIENT disconnected player " + std::to_string(uid), xy::Logger::Type::Info);
    }
        break;
    case xy::Server::GameStart:
    {
        auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg->type = xy::Message::UIEvent::MenuClosed;
        msg->stateId = States::ID::MenuLobby;

        requestStackClear();
        requestStackPush(States::ID::Game);
        break;
    }
    default: break;
    }
}

void MenuLobbyState::buildMenu()
{
    const auto& font = m_fontResource.get("flaps");
    
    auto startButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    startButton->setText("Start");
    startButton->setAlignment(xy::UI::Alignment::Centre);
    startButton->setPosition(840.f, 770.f);
    startButton->addCallback([this]()
    {
        //check we're host and are allowed to do this
        if (getContext().appInstance.hosting())
        {
            //TODO check all clients are ready
            auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
            msg->type = xy::Message::UIEvent::MenuClosed;
            msg->stateId = States::ID::MenuLobby;

            requestStackClear();
            requestStackPush(States::ID::Game);
        }
    });
    m_uiContainer.addControl(startButton);

    auto backButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    backButton->setText("Back");
    backButton->setAlignment(xy::UI::Alignment::Centre);
    backButton->setPosition(1080.f, 770.f);
    backButton->addCallback([this]()
    {
        requestStackPop();

        auto msg0 = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
        msg0->type = xy::Message::UIEvent::MenuClosed;
        msg0->stateId = States::ID::MenuLobby;

        auto msg1 = m_messageBus.post<xy::Message::NetworkEvent>(xy::Message::NetworkMessage);
        msg1->action = xy::Message::NetworkEvent::RequestDisconnect;

        requestStackPush(States::ID::MenuMain);
    });
    m_uiContainer.addControl(backButton);
}