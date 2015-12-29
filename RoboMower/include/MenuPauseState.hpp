//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//in game pause menu

#ifndef PAUSE_STATE_HPP_
#define PAUSE_STATE_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>
#include <xygine/Resource.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>

namespace xy
{
    class MessageBus;
}
class MenuPauseState final : public xy::State
{
public:
    MenuPauseState(xy::StateStack&, Context);
    ~MenuPauseState() = default;

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::MenuPaused;
    }
private:
    xy::MessageBus& m_messageBus;
    xy::UI::Container m_uiContainer;
    sf::Sprite m_cursorSprite;

    std::vector<sf::Text> m_texts;

    xy::TextureResource m_textureResource;
    xy::FontResource m_fontResource;

    void buildMenu(const sf::Font&);
    void sendCloseMessage();
};
#endif //PAUSE_STATE_HPP_