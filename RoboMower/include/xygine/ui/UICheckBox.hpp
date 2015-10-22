//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//checkbox control with text label

#ifndef UI_CHECKBOX_HPP_
#define UI_CHECKBOX_HPP_

#include <xygine/ui/UIControl.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <functional>

namespace ui
{
    class CheckBox final : public Control
    {
    public:
        using Ptr =  std::shared_ptr<CheckBox>;
        using Callback = std::function<void(const CheckBox*)>;

        enum class Event
        {
            CheckChanged
        };

        CheckBox(const sf::Font& font, const sf::Texture& t);
        ~CheckBox() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event&, const sf::Vector2f&) override;
        void setAlignment(Alignment) override;
        bool contains(const sf::Vector2f& mousePos) const override;

        void setText(const std::string& text);
        void setTextColour(const sf::Color&);
        void setFont(const sf::Font&);
        void setFontSize(sf::Uint8);

        bool checked() const;
        void check(bool checked = true);
        
        void setCallback(Callback, Event);

    private:
        enum State
        {
            Normal = 0,
            Selected,
            CheckedNormal,
            CheckedSelected
        };

        const sf::Texture& m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        std::vector<sf::IntRect> m_subRects;
        bool m_checked;
        Alignment m_alignment;

        Callback m_checkChanged;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_CHECKBOX_HPP_