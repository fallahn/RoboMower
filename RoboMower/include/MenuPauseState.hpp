//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//in game pause menu

#ifndef PAUSE_STATE_HPP_
#define PAUSE_STATE_HPP_

#include <State.hpp>
#include <UIContainer.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>

class MessageBus;
class MenuPauseState final : public State
{
public:
    MenuPauseState(StateStack&, Context);
    ~MenuPauseState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::MenuPaused;
    }
private:
    MessageBus& m_messageBus;
    ui::Container m_uiContainer;
    sf::Sprite m_cursorSprite;

    std::vector<sf::Text> m_texts;

    void buildMenu(const sf::Font&);
    void sendCloseMessage();
};

#endif //PAUSE_STATE_HPP_