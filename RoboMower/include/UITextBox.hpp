//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//ui control for text input

#ifndef UI_TEXTBOX_HPP_
#define UI_TEXTBOX_HPP_

#include <UIControl.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ui
{
    class TextBox final : public Control
    {
    public:
        using Ptr = std::shared_ptr<TextBox>;

        TextBox(const sf::Font& font, const sf::Color& background = sf::Color::Black, const sf::Color& border = sf::Color::White);
        ~TextBox() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event& e, const sf::Vector2f& mousePos) override;
        void update(float dt) override;

        void setAlignment(Alignment a) override;
        bool contains(const sf::Vector2f& mousePos) const override;

        const std::string& getText() const;
        void setTexture(const sf::Texture& text);
        void showBorder(bool show = true);
        void setSize(const sf::Vector2f& size);
        void setText(const std::string& text);
        void setLabelText(const std::string&);

        void setMaxLength(sf::Uint16);

    private:
        std::string m_string;
        sf::Text m_text;
        sf::Text m_label;
        sf::RectangleShape m_backShape;
        sf::RectangleShape m_cursorShape;
        sf::Clock m_cursorClock;
        bool m_showCursor;
        sf::Keyboard::Key m_lastKey;

        sf::Color m_borderColour;
        sf::Color m_selectedColour;

        sf::Uint16 m_maxLength;

        enum State
        {
            Selected = 0,
            Normal,
            Size
        };
        std::vector<sf::IntRect> m_subRects;
        sf::FloatRect m_bounds;
        Alignment m_currentAlignment;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_TEXTBOX_H_