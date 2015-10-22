/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

xygine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <xygine/MenuMainState.hpp>
#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

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