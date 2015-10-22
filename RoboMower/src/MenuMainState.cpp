//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MenuMainState.hpp>

#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

using namespace xy;

MenuMainState::MenuMainState(StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

    buildMenu();

    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.type = Message::UIEvent::MenuOpened;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuMain;
    m_messageBus.post(msg);
}

//public
bool MenuMainState::update(float dt)
{
    m_uiContainer.update(dt);
    return false;
}

void MenuMainState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);
    //rw.draw(m_menuSprite);

    rw.draw(m_uiContainer);
    rw.draw(m_cursorSprite);
}

bool MenuMainState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);

    return false;
}

void MenuMainState::handleMessage(const Message& msg)
{
    //if (msg.type == Message::Type::Network)
    //{
    //    switch (msg.network.action)
    //    {
    //    case Message::NetworkEvent::ConnectSuccess:
    //        close();
    //        if (msg.network.stateID == States::ID::Game)
    //        {
    //            requestStackClear();
    //        }
    //        requestStackPush(msg.network.stateID);
    //        break;
    //        //TODO message on connect fail
    //    default: break;
    //    }
    //}
}

//private
void MenuMainState::buildMenu()
{
    const auto& font = getContext().appInstance.getFont("assets/fonts/VeraMono.ttf");
    
    auto button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Single Player");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 475.f);
    button->setCallback([this]()
    {
        Message msg;
        msg.type = Message::Type::Network;
        msg.network.action = Message::NetworkEvent::RequestStartServer;
        msg.network.stateID = States::ID::Game;
        m_messageBus.post(msg);

        close();
        requestStackClear();
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Host Multiplayer");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 575.f);
    button->setCallback([this]()
    {
        close();

        Message msg;
        msg.type = Message::Type::Network;
        msg.network.action = Message::NetworkEvent::RequestStartServer;
        msg.network.stateID = States::ID::MenuLobby;
        m_messageBus.post(msg);
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Join Multiplayer");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 675.f);
    button->setCallback([this]()
    {
        close();
        requestStackPush(States::ID::MenuJoin);
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Options");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 775.f);
    button->setCallback([this]()
    {
        close();
        requestStackPush(States::ID::MenuOptions);
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Quit");
    button->setAlignment(ui::Alignment::Centre);
    button->setPosition(960.f, 875.f);
    button->setCallback([this]()
    {
        getContext().renderWindow.close();
    });
    m_uiContainer.addControl(button);
}

void MenuMainState::close()
{
    requestStackPop();

    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.type = Message::UIEvent::MenuClosed;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuMain;
    m_messageBus.post(msg);
}