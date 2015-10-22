//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/ui/UIControl.hpp>

using namespace ui;

Control::Control()
    : m_selected    (false),
    m_active        (false),
    m_visible       (true),
    m_index         (0u)
{

}

//public
bool Control::selected() const
{
    return m_selected;
}

void Control::select()
{
    m_selected = true;
}

void Control::deselect()
{
    m_selected = false;
}

bool Control::active() const
{
    return m_active;
}

void Control::activate()
{
    m_active = true;
}

void Control::deactivate()
{
    m_active = false;
}

bool Control::contains(const sf::Vector2f& mousePos) const
{
    return false;
}

void Control::setVisible(bool visible)
{
    m_visible = visible;
}

bool Control::visible() const
{
    return m_visible;
}