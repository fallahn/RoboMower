//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//main menu state

#ifndef MENU_MAIN_STATE_HPP_
#define MENU_MAIN_STATE_HPP_

#include <xygine/State.hpp>
#include <xygine/ui/UIContainer.hpp>

#include <SFML/Graphics/Sprite.hpp>

class MessageBus;
class MenuMainState final : public State
{
public:
    MenuMainState(StateStack&, Context);
    ~MenuMainState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::MenuMain;
    }
private:
    MessageBus& m_messageBus;
    ui::Container m_uiContainer;
    sf::Sprite m_cursorSprite;

    void buildMenu();
    void close();
};

#endif //MENU_MAIN_STATE_HPP_