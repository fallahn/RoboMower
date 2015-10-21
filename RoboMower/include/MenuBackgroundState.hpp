//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//renders the background under any active menu state

#ifndef MENU_BACKGROUND_STATE_HPP_
#define MENU_BACKGROUND_STATE_HPP_

#include <State.hpp>
#include <UIContainer.hpp>

#include <SFML/Graphics/Text.hpp>

#include <vector>

class MessageBus;
class MenuBackgroundState final : public State
{
public:
    MenuBackgroundState(StateStack&, Context);
    ~MenuBackgroundState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::MenuBackground;
    }
private:
    MessageBus& m_messageBus;
    ui::Container m_uiContainer;

    std::vector<sf::Text> m_texts;
};

#endif //MENU_BACKGROUND_STATE_HPP_