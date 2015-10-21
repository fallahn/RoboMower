//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//base class for ui controls

#ifndef UI_CONTROL_HPP_
#define UI_CONTROL_HPP_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>

namespace sf
{
    class Event;
}

namespace ui
{
    enum class Alignment
    {
        TopLeft,
        BottomLeft,
        Centre,
        TopRight,
        BottomRight
    };

    class Control : public sf::Drawable, public sf::Transformable
    {
    public:
        using Ptr = std::shared_ptr<Control>;

        Control();
        virtual ~Control() = default;
        Control(const Control&) = delete;
        const Control& operator = (const Control&) = delete;

        virtual bool selectable() const = 0;
        bool selected() const;

        virtual void select();
        virtual void deselect();

        virtual bool active() const;
        virtual void activate();
        virtual void deactivate();

        virtual void handleEvent(const sf::Event&, const sf::Vector2f&) = 0;
        virtual void update(float dt){};

        virtual void setAlignment(Alignment) = 0;
        virtual bool contains(const sf::Vector2f& mousePos) const;

        void setVisible(bool visible);
        bool visible() const;

    private:

        bool m_selected;
        bool m_active;
        bool m_visible;
        sf::Uint16 m_index;
    };
}

#endif //UI_CONTROL_H_