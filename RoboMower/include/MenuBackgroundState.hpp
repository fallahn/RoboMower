//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//renders the background under any active menu state

#ifndef MENU_BACKGROUND_STATE_HPP_
#define MENU_BACKGROUND_STATE_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>

#include <SFML/Graphics/Text.hpp>

#include <vector>

namespace xy
{
    class MessageBus;
}
class MenuBackgroundState final : public xy::State
{
public:
    MenuBackgroundState(xy::StateStack&, Context);
    ~MenuBackgroundState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::MenuBackground;
    }
private:
    xy::MessageBus& m_messageBus;
    xy::ui::Container m_uiContainer;

    std::vector<sf::Text> m_texts;
};

#endif //MENU_BACKGROUND_STATE_HPP_