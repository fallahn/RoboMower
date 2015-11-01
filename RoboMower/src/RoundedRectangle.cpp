//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <components/RoundedRectangle.hpp>

#include <xygine/MessageBus.hpp>
#include <xygine/Entity.hpp>

#include <cassert>

namespace
{
    const sf::Uint8 radiusPoints = 8u;
    const float pi = 3.141592654f;
    const float radian = pi / 180.f;
}


RoundedRectangle::RoundedRectangle(xy::MessageBus& mb, const sf::Vector2f& size, float radius)
    : xy::Component (mb, this),
    m_size          (size),
    m_cornerRadius  (radius),
    m_minimumSize   (10.f, 10.f)
{
    assert(radius > 0 && size.x > 0 && size.y > 0);
    
    clampSize();
    update();
}

//public
xy::Component::Type RoundedRectangle::type() const
{
    return xy::Component::Type::Drawable;
}

void RoundedRectangle::entityUpdate(xy::Entity& e, float)
{
    //TODO dirty flag optimise, or add listener callback to ent
    m_globalBounds = e.getWorldTransform().transformRect(getGlobalBounds());
}

void RoundedRectangle::handleMessage(const xy::Message&){}

void RoundedRectangle::onStart(xy::Entity& e)
{
    m_globalBounds = e.getWorldTransform().transformRect(getGlobalBounds());
}

sf::FloatRect RoundedRectangle::globalBounds() const
{
    return m_globalBounds;
}

sf::FloatRect RoundedRectangle::localBounds() const
{
    return getLocalBounds();
}

void RoundedRectangle::setSize(const sf::Vector2f& size)
{
    m_size = size;
    clampSize();
    update();
}

void RoundedRectangle::setRadius(float radius)
{
    m_cornerRadius = radius;
    update();
}

const sf::Vector2f& RoundedRectangle::getSize() const
{
    return m_size;
}

std::size_t RoundedRectangle::getPointCount() const
{
    return radiusPoints * 4u;
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const
{
    if (index >= getPointCount()) return sf::Vector2f();

    float deltaAngle = 90.f / (radiusPoints - 1);
    sf::Vector2f center;
    sf::Uint32 centerIndex = index / radiusPoints;

    switch (centerIndex)
    {
    case 0:
        center.x = m_size.x - m_cornerRadius;
        center.y = m_cornerRadius;
        break;
    case 1:
        center.x = m_cornerRadius;
        center.y = m_cornerRadius;
        break;
    case 2:
        center.x = m_cornerRadius;
        center.y = m_size.y - m_cornerRadius;
        break;
    case 3:
        center.x = m_size.x - m_cornerRadius;
        center.y = m_size.y - m_cornerRadius;
        break;
    default: break;
    }

    return
    {
        m_cornerRadius * std::cos(deltaAngle * (index - centerIndex) * radian) + center.x,
        -m_cornerRadius * std::sin(deltaAngle * (index - centerIndex) * radian) + center.y
    };
}


//private
void RoundedRectangle::clampSize()
{
    m_size.x = std::max(m_size.x, m_minimumSize.x);
    m_size.y = std::max(m_size.y, m_minimumSize.y);
}
