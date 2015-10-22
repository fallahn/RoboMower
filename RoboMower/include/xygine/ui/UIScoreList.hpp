//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef UI_SCORE_LIST_HPP_
#define UI_SCORE_LIST_HPP_

#include <xygine/ui/UIControl.hpp>
#include <xygine/Score.hpp>

#include <SFML/Graphics/Text.hpp>

#include <memory>

namespace ui
{
    class ScoreList final : public Control
    {
    public:
        explicit ScoreList(const sf::Font&);
        ~ScoreList() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event&, const sf::Vector2f&) override;
        void update(float) override;

        void setAlignment(Alignment) override;
        bool contains(const sf::Vector2f&) const override;

        void scroll(float);
        void setList(const std::vector<Scores::Item>&);
        void setIndex(sf::Uint32);
        
        float getVerticalSpacing() const;
    private:

        std::vector<sf::Text> m_texts;
        sf::FloatRect m_bounds;
        const sf::Font& m_font;
        float m_scrollTargetDistance;
        float m_scrollCurrentDistance;
        float m_scrollSpeed;
        bool m_doScroll;

        void draw(sf::RenderTarget&, sf::RenderStates) const override;
        void updateTexts(float);
    };
}

#endif //UI_SCORE_LIST_HPP_