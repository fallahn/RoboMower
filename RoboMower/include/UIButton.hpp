//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef UI_BUTTON_HPP_
#define UI_BUTTON_HPP_

#include <UIControl.hpp>
#include <Resource.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace ui
{
    class Button final : public Control
    {
    public:
        using Ptr = std::shared_ptr<Button>;
        using Callback = std::function<void()>;

        Button(const sf::Font& font, const sf::Texture& texture);
        ~Button() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event& e, const sf::Vector2f& mousePos) override;

        void setAlignment(Alignment a) override;
        bool contains(const sf::Vector2f& mousePos)const override;

        void setCallback(Callback c);
        void setText(const std::string& text);
        void setTextColour(const sf::Color& c);
        void setFontSize(sf::Uint16 size);
        void setTogglable(bool b);


    private:
        enum State
        {
            Normal = 0,
            Selected,
            Active,
            Count
        };

        Callback m_callback;
        const sf::Texture& m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        bool m_toggleButton;

        std::vector<sf::IntRect> m_subRects;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_BUTTON_HPP_