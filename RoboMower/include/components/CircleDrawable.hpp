//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef CIRCLE_DRAWABLE_HPP_
#define CIRCLE_DRAWABLE_HPP_

#include <SFML/Graphics/CircleShape.hpp>

#include <xygine/Component.hpp>

class CircleDrawable final : public xy::Component, public sf::Transformable, public sf::Drawable
{
public:
    explicit CircleDrawable(xy::MessageBus&);
    ~CircleDrawable() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const xy::Message&) override;

    sf::FloatRect globalBounds() const override;
    sf::FloatRect localBounds() const override;

    void setRadius(float);
    float getRadius() const;
    void setPointCount(std::size_t);
    std::size_t getPointCount() const;
    sf::Vector2f getPoint(std::size_t) const;
    void setTexture(sf::Texture*, bool = true);
    void setTextureRect(const sf::IntRect&);
    void setFillColor(const sf::Color&);
    void setOutlineColor(const sf::Color&);
    void setOutlineThickness(float);

    const sf::Texture* getTexture() const;
    const sf::IntRect& getTextureRect() const;
    const sf::Color& getFillColor() const;
    const sf::Color& getOutlineColor() const;
    float getOutlineThickness() const;


private:
    sf::FloatRect m_localBounds;
    sf::FloatRect m_globalBounds;

    sf::CircleShape m_shape;
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //CIRCLE_DRAWABLE_HPP_
