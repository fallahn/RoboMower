//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//displays a handle for loop instructions to display jump destination

#ifndef LOOP_HANDLE_HPP_
#define LOOP_HANDLE_HPP_

#include <xygine/components/Component.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

class LoopHandle final : public xy::Component, public sf::Drawable, public sf::Transformable
{
public:
    LoopHandle(xy::MessageBus&, const sf::Texture&, float verticalSpacing);
    ~LoopHandle() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Drawable; }
    void entityUpdate(xy::Entity&, float) override;

    sf::FloatRect globalBounds() const override;
    sf::FloatRect localBounds() const override;

    void setEnabled(bool);

    std::size_t getSize() const;
    void setSize(std::size_t);

private:

    const sf::Texture& m_texture;
    std::vector<sf::Vertex> m_vertices;

    sf::FloatRect m_localBounds;
    sf::FloatRect m_globalBounds;

    float m_verticalSpacing;
    float m_vertexSpacing;

    bool m_enabled;
    std::size_t m_size;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif //LOOP_HANDLE_HPP_