//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <MenuMainState.hpp>

#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

MenuMainState::MenuMainState(xy::StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    m_cursorSprite.setTexture(m_textureResource.get("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

    buildMenu();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateId = States::ID::MenuMain;

    /*auto msg2 = m_messageBus.post<xy::Message::NetworkEvent>(xy::Message::NetworkMessage);
    msg2->action = xy::Message::NetworkEvent::RequestDisconnect;
    msg2->stateID = States::ID::MenuMain;*/
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

void MenuMainState::handleMessage(const xy::Message& msg)
{

}

//private
void MenuMainState::buildMenu()
{
    const auto& font = m_fontResource.get("assets/fonts/VeraMono.ttf");
    
    auto button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Single Player");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 475.f);
    button->addCallback([this]()
    {
        //auto msg = m_messageBus.post<xy::Message::NetworkEvent>(xy::Message::NetworkMessage);
        //msg->action = xy::Message::NetworkEvent::RequestStartServer;
        //msg->stateID = States::ID::Game;

        close();
        requestStackClear();
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Host Multiplayer");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 575.f);
    button->addCallback([this]()
    {

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Join Multiplayer");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 675.f);
    button->addCallback([this]()
    {

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Options");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 775.f);
    button->addCallback([this]()
    {
        close();
        requestStackPush(States::ID::MenuOptions);
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Quit");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 875.f);
    button->addCallback([this]()
    {
        getContext().renderWindow.close();
    });
    m_uiContainer.addControl(button);
}

void MenuMainState::close()
{
    requestStackPop();

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuClosed;
    msg->stateId = States::ID::MenuMain;
}