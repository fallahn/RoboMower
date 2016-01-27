//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_PLAYER_DRAWABLE_HPP_
#define RM_PLAYER_DRAWABLE_HPP_

#include <xygine/components/Component.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

class PlayerDrawable final : public xy::Component, public sf::Drawable, public sf::Transformable
{
public:
    PlayerDrawable(xy::MessageBus&, sf::Texture&, bool local);
    PlayerDrawable() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;

private:

    enum Direction
    {
        Up, Down, Left, Right, Count
    }m_direction;
    std::vector<sf::Vector2f> m_origins;
    std::vector<sf::Vertex> m_upSprite;
    std::vector<sf::Vertex> m_downSprite;
    std::vector<sf::Vertex> m_leftSprite;
    std::vector<sf::Vertex> m_rightSprite;

    sf::Vertex* m_currentSprite;
    std::size_t m_vertexCount;
    sf::Texture& m_texture;
    
    void setDirection(Direction);

    void createSprites(bool local);
    void buildSprite(const sf::Vector2f&, const sf::Vector2f&, Direction);
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif //RM_PLAYER_DRAWABLE_HPP_