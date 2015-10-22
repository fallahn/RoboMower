//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//menu to join multiplayer game

#ifndef MENU_JOIN_STATE_HPP_
#define MENU_JOIN_STATE_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>

#include <xygine/ui/Container.hpp>
#include <xygine/ui/Label.hpp>

#include <SFML/Graphics/Sprite.hpp>

namespace xy
{
    class MessageBus;
}
class MenuJoinState final : public xy::State
{
public:
    MenuJoinState(xy::StateStack&, Context);
    ~MenuJoinState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::MenuJoin;
    }
private:
    xy::MessageBus& m_messageBus;
    xy::ui::Container m_uiContainer;
    sf::Sprite m_cursorSprite;

    xy::ui::Label::Ptr m_statusLabel;

    void buildMenu();
    void sendCloseMessage();
};

#endif //MENU_JOIN_STATE_HPP_