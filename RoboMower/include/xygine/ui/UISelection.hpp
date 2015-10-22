//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef UI_SELECTION_HPP_
#define UI_SELECTION_HPP_

#include <xygine/ui/UIControl.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <functional>

namespace ui
{
    class Selection final : public Control
    {
    public:
        using Ptr = std::shared_ptr<Selection>;
        using Callback = std::function<void(const Selection*)>;

        Selection(const sf::Font&, const sf::Texture&, float = 350.f);
        ~Selection() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event&, const sf::Vector2f&) override;
        void setAlignment(Alignment) override;
        bool contains(const sf::Vector2f&) const override;

        void addItem(const std::string&, sf::Int32);
        const std::string& getSelectedText() const;
        sf::Int32 getSelectedValue() const;

        void setSelectedIndex(sf::Uint16);
        sf::Uint32 itemCount() const;
        void selectItem(const std::string&);
        void selectItem(sf::Uint16);

        void setCallback(Callback);

    private:
        enum State
        {
            Normal = 0,
            Selected,
            Count
        };

        std::vector<sf::IntRect> m_subRects;

        struct Item
        {
            using Ptr = std::unique_ptr<Item>;
            std::string name;
            sf::Int32 value;

            Item(const std::string&, sf::Int32);
        };

        std::vector<Item::Ptr> m_items;

        float m_length;
        sf::FloatRect m_bounds;

        sf::Uint16 m_selectedIndex;
        sf::Text m_selectedText;

        sf::Sprite m_prevArrow;
        sf::Sprite m_nextArrow;

        sf::RectangleShape m_background;

        enum class SelectedButton
        {
            Prev,
            Next
        } m_selectedButton;

        Callback m_selectionChanged;

        void draw(sf::RenderTarget&, sf::RenderStates) const override;
        void updateText();
    };
}
#endif //UI_SELECTION_HPP_