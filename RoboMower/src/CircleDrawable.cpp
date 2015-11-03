//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/CircleDrawable.hpp>

#include <xygine/Entity.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

CircleDrawable::CircleDrawable(xy::MessageBus& mb)
    : xy::Component(mb, this) {}

//public
void CircleDrawable::entityUpdate(xy::Entity& ent, float)
{
    const float width = getRadius() * 2.f;
    m_localBounds = { -getOrigin(), {width, width} };
    m_globalBounds = ent.getWorldTransform().transformRect(m_localBounds);
}

void CircleDrawable::handleMessage(const xy::Message&){}

sf::FloatRect CircleDrawable::globalBounds() const
{
    return m_globalBounds;
}

sf::FloatRect CircleDrawable::localBounds() const
{
    return m_localBounds;
}

void CircleDrawable::setRadius(float r)
{
    m_shape.setRadius(r);
}

float CircleDrawable::getRadius() const
{
    return m_shape.getRadius();
}

void CircleDrawable::setPointCount(std::size_t count)
{
    m_shape.setPointCount(count);
}

std::size_t CircleDrawable::getPointCount() const
{
    return m_shape.getPointCount();
}

sf::Vector2f CircleDrawable::getPoint(std::size_t idx) const
{
    return m_shape.getPoint(idx);
}

void CircleDrawable::setTexture(sf::Texture* t, bool reset)
{
    m_shape.setTexture(t, reset);
}

void CircleDrawable::setTextureRect(const sf::IntRect& rect)
{
    m_shape.setTextureRect(rect);
}

void CircleDrawable::setFillColor(const sf::Color& c)
{
    m_shape.setFillColor(c);
}

void CircleDrawable::setOutlineColor(const sf::Color& c)
{
    m_shape.setOutlineColor(c);
}

void CircleDrawable::setOutlineThickness(float thickness)
{
    m_shape.setOutlineThickness(thickness);
}

const sf::Texture* CircleDrawable::getTexture() const
{
    return m_shape.getTexture();
}

const sf::IntRect& CircleDrawable::getTextureRect() const
{
    return m_shape.getTextureRect();
}

const sf::Color& CircleDrawable::getFillColor() const
{
    return m_shape.getFillColor();
}

const sf::Color& CircleDrawable::getOutlineColor() const
{
    return m_shape.getOutlineColor();
}

float CircleDrawable::getOutlineThickness() const
{
    return m_shape.getOutlineThickness();
}

//private
void CircleDrawable::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_shape, states);
}