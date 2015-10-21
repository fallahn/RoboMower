//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef UI_COMBOBOX_HPP_
#define UI_COMBOBOX_HPP_

#include <UIControl.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace ui
{
    class ComboBox final : public Control
    {
    public:
        using Ptr = std::shared_ptr<ComboBox>;

        ComboBox(const sf::Font& font, const sf::Texture& t);
        ~ComboBox() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event&, const sf::Vector2f&) override;
        void setAlignment(Alignment) override;
        bool contains(const sf::Vector2f& mousePos) const override;

        void addItem(const std::string& name, sf::Int32 value);
        const std::string& getSelectedText() const;
        sf::Int32 getSelectedValue() const;

        void setBackgroundColour(const sf::Color&);
        void setHighlightColour(const sf::Color&);
        void setSelectedIndex(sf::Uint16 index);
        sf::Uint32 size() const;
        void selectItem(const std::string&);
        void selectItem(sf::Uint16);

    private:
        struct Item
        {
            using Ptr = std::unique_ptr<Item>;
            std::string name;
            sf::Int32 value;

            sf::FloatRect bounds;
            sf::Text text;

            Item(const std::string& name, sf::Int32 value, const sf::Font& font);
        };

        std::vector<Item::Ptr> m_items;

        bool m_showItems;
        sf::RectangleShape m_mainShape;
        sf::RectangleShape m_dropDownShape;
        mutable sf::RectangleShape m_highlightShape;

        sf::Uint16 m_selectedIndex;
        sf::Uint16 m_nextIndex;
        sf::Text m_selectedText;

        const sf::Font& m_font;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_COMBOBOX_HPP_