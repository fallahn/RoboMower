//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MenuPauseState.hpp>

#include <xygine/App.hpp>
#include <xygine/Util.hpp>

#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

MenuPauseState::MenuPauseState(xy::StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    const auto& font = context.appInstance.getFont("flaps");
    
    buildMenu(font);

    m_texts.emplace_back("PAUSED", font);
    xy::Util::Position::centreOrigin(m_texts.back());
    m_texts.back().setPosition(960.f, 200.f);

    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

    xy::Message msg;
    msg.type = xy::Message::Type::UI;
    msg.ui.type = xy::Message::UIEvent::MenuOpened;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuPaused;
    m_messageBus.post(msg);

    //TODO check if hosting and send pause packet to server
}

//public
bool MenuPauseState::update(float dt)
{
    m_uiContainer.update(dt);
    return true;
}

void MenuPauseState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);
    //rw.draw(m_menuSprite);

    rw.draw(m_uiContainer);
    rw.draw(m_cursorSprite);
}

bool MenuPauseState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);

    return false;
}

void MenuPauseState::handleMessage(const xy::Message&){}

//private
void MenuPauseState::buildMenu(const sf::Font& font)
{
    auto button = std::make_shared<xy::ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Continue");
    button->setAlignment(xy::ui::Alignment::Centre);
    button->setPosition(960.f, 475.f);
    button->setCallback([this]()
    {
        requestStackPop();
        sendCloseMessage();
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Options");
    button->setAlignment(xy::ui::Alignment::Centre);
    button->setPosition(960.f, 575.f);
    button->setCallback([this]()
    {
        requestStackPop();
        requestStackPush(States::ID::MenuOptions);
        sendCloseMessage();

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/start_button.png"));
    button->setText("Quit");
    button->setAlignment(xy::ui::Alignment::Centre);
    button->setPosition(960.f, 675.f);
    button->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::ID::MenuBackground);
        requestStackPush(States::ID::MenuMain);

        sendCloseMessage();

        xy::Message msg;
        msg.type = xy::Message::Type::Network;
        //msg.network.stateID = States::ID::MenuPaused;
        msg.network.action = xy::Message::NetworkEvent::RequestDisconnect;
        m_messageBus.post(msg);
    });
    m_uiContainer.addControl(button);
}

void MenuPauseState::sendCloseMessage()
{
    xy::Message msg;
    msg.type = xy::Message::Type::UI;
    msg.ui.stateId = States::ID::MenuPaused;
    msg.ui.type = xy::Message::UIEvent::MenuClosed;
    m_messageBus.post(msg);
}