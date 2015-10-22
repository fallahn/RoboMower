//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//contains one or more ui controls

#ifndef UI_CONTAINER_HPP_
#define UI_CONTAINER_HPP_

#include <xygine/ui/UIControl.hpp>
#include <xygine/SoundPlayer.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>

namespace ui
{
    //not final, forms inherit from this
    class Container : public Control
    {
    public:
        typedef std::shared_ptr<Container> Ptr;

        Container();
        Container(Container&& c):m_selectedIndex(c.m_selectedIndex), m_background(c.m_background){}
        Container& operator=(Container&&){ return *this; }

        ~Container() = default;

        void addControl(Control::Ptr control);

        virtual bool selectable() const override;
        virtual void handleEvent(const sf::Event& e, const sf::Vector2f& mousePos) override;
        virtual void update(float dt) override;
        virtual void setAlignment(Alignment a) override{} //TODO flow layout

        void setBackgroundColour(const sf::Color&);
        void setBackgroundTexture(const sf::Texture&);
    private:
        std::vector<Control::Ptr> m_controls;
        sf::Int16 m_selectedIndex;

        sf::RectangleShape m_background;

        bool hasSelection() const;
        void select(sf::Int16 index);
        void selectNext();
        void selectPrevious();

        virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_CONTAINER_HPP_