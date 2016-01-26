//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

//main menu state

#ifndef MENU_MAIN_STATE_HPP_
#define MENU_MAIN_STATE_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>
#include <xygine/Resource.hpp>

#include <SFML/Graphics/Sprite.hpp>

namespace xy
{
    class MessageBus;
}
class MenuMainState final : public xy::State
{
public:
    MenuMainState(xy::StateStack&, Context);
    ~MenuMainState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::MenuMain;
    }
private:
    xy::MessageBus& m_messageBus;
    xy::UI::Container m_uiContainer;
    sf::Sprite m_cursorSprite;

    xy::TextureResource m_textureResource;
    xy::FontResource m_fontResource;

    void buildMenu();
    void close();
};
#endif //MENU_MAIN_STATE_HPP_