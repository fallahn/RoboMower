//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <MenuLobbyState.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>
#include <xygine//ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

namespace
{
    const float clientTimeout = 5.f;
    const float tickRate = 1.f / 20.f;
}

MenuLobbyState::MenuLobbyState(xy::StateStack& stack, Context context)
    : State                 (stack, context),
    m_messageBus            (context.appInstance.getMessageBus()),
    m_font                  (m_fontResource.get("dfg"))
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
void MenuLobbyState::buildMenu()
{
    const auto& font = m_fontResource.get("flaps");
    
    auto startButton = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/button.png"));
    startButton->setText("Start");
    startButton->setAlignment(xy::UI::Alignment::Centre);
    startButton->setPosition(840.f, 770.f);
    startButton->addCallback([this]()
    {

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

        //auto msg1 = m_messageBus.post<xy::Message::NetworkEvent>(xy::Message::NetworkMessage);
        //msg1->action = xy::Message::NetworkEvent::RequestDisconnect;

        requestStackPush(States::ID::MenuMain);
    });
    m_uiContainer.addControl(backButton);
}