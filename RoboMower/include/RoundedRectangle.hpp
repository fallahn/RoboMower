//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//creates a rounded rectangle drawable

#ifndef ROUND_RECT_HPP_
#define ROUND_RECT_HPP_

#include <xygine/components/Component.hpp>
#include <xygine/ShaderProperty.hpp>

#include <SFML/Graphics/Shape.hpp>

namespace xy
{
    class MessageBus;
}

class RoundedRectangle final : public sf::Shape
{
public:
    RoundedRectangle(const sf::Vector2f& size = sf::Vector2f(8.f, 8.f), float radius = 8.f);
    ~RoundedRectangle() = default;
    RoundedRectangle(const RoundedRectangle&) = delete;
    RoundedRectangle& operator = (const RoundedRectangle&) = delete;

    void setSize(const sf::Vector2f&);
    void setRadius(float);
    const sf::Vector2f& getSize() const;
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t) const override;

private:
    sf::Vector2f m_size;
    float m_cornerRadius;
    sf::Vector2f m_minimumSize;

    bool m_transformDirty;
    sf::FloatRect m_globalBounds;

    void clampSize();
};

#endif //ROUND_RECT_HPP_