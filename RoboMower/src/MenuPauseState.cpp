/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#include <MenuPauseState.hpp>

#include <xygine/App.hpp>
#include <xygine/util/Position.hpp>

#include <xygine/ui/Button.hpp>

#include <SFML/Window/Mouse.hpp>

MenuPauseState::MenuPauseState(xy::StateStack& stack, Context context)
    : State     (stack, context),
    m_messageBus(context.appInstance.getMessageBus()),
    m_uiContainer(m_messageBus)
{
    const auto& font = m_fontResource.get("flaps");
    
    buildMenu(font);

    m_texts.emplace_back("PAUSED", font);
    xy::Util::Position::centreOrigin(m_texts.back());
    m_texts.back().setPosition(960.f, 200.f);

    m_cursorSprite.setTexture(m_textureResource.get("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->type = xy::Message::UIEvent::MenuOpened;
    msg->stateID = States::ID::MenuPaused;

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
    auto button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Continue");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 475.f);
    button->addCallback([this]()
    {
        requestStackPop();
        sendCloseMessage();
    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Options");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 575.f);
    button->addCallback([this]()
    {
        requestStackPop();
        requestStackPush(States::ID::MenuOptions);
        sendCloseMessage();

    });
    m_uiContainer.addControl(button);

    button = std::make_shared<xy::UI::Button>(font, m_textureResource.get("assets/images/ui/start_button.png"));
    button->setText("Quit");
    button->setAlignment(xy::UI::Alignment::Centre);
    button->setPosition(960.f, 675.f);
    button->addCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::ID::MenuBackground);
        requestStackPush(States::ID::MenuMain);

        sendCloseMessage();
        LOG("Didn't quit server yet!", xy::Logger::Type::Warning);
    });
    m_uiContainer.addControl(button);
}

void MenuPauseState::sendCloseMessage()
{
    auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
    msg->stateID = States::ID::MenuPaused;
    msg->type = xy::Message::UIEvent::MenuClosed;
}