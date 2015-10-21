//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//menu to join multiplayer game

#ifndef MENU_JOIN_STATE_HPP_
#define MENU_JOIN_STATE_HPP_

#include <State.hpp>

#include <UIContainer.hpp>
#include <UILabel.hpp>

#include <SFML/Graphics/Sprite.hpp>

class MessageBus;
class MenuJoinState final : public State
{
public:
    MenuJoinState(StateStack&, Context);
    ~MenuJoinState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::MenuJoin;
    }
private:
    MessageBus& m_messageBus;
    ui::Container m_uiContainer;
    sf::Sprite m_cursorSprite;

    ui::Label::Ptr m_statusLabel;

    void buildMenu();
    void sendCloseMessage();
};

#endif //MENU_JION_STATE_HPP_