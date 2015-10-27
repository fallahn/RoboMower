//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//creates a rounded rectangle drawable component

#ifndef ROUND_RECT_HPP_
#define ROUND_RECT_HPP_

#include <xygine/Component.hpp>

#include <SFML/Graphics/Shape.hpp>

namespace xy
{
    class MessageBus;
}

class RoundedRectangle final : public sf::Shape, public xy::Component
{
public:
    RoundedRectangle(xy::MessageBus&, const sf::Vector2f& size, float radius = 14.f);
    ~RoundedRectangle() = default;
    RoundedRectangle(const RoundedRectangle&) = delete;
    RoundedRectangle& operator = (const RoundedRectangle&) = delete;

    xy::Component::Type type() const override;

    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const xy::Message&) override;

    void onStart(xy::Entity&) override;
    //prefer this over the inherited version as it properly
    //takes into account of entity transform
    sf::FloatRect globalBounds() const override;
    sf::FloatRect localBounds() const override;

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