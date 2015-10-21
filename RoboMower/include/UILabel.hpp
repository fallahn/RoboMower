//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef UI_LABEL_HPP_
#define UI_LABEL_HPP_

#include <UIControl.hpp>

#include <SFML/Graphics/Text.hpp>

namespace ui
{
    class Label final : public Control
    {
    public:
        using Ptr = std::shared_ptr<Label>;

        explicit Label(const sf::Font&);

        bool selectable() const override;
        void handleEvent(const sf::Event&, const sf::Vector2f&) override;
        void setAlignment(Alignment) override;

        void setString(const std::string&);
        void setColour(const sf::Color&);
        void setCharacterSize(sf::Uint32);

    private:

        sf::Text m_text;
        Alignment m_currentAlignment;

        void updateText();
        void draw(sf::RenderTarget&, sf::RenderStates) const override;
    };
}

#endif //UI_LABEL_HPP_