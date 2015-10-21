//==============================================================================
// D.O.P.E - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//draws a section of terrain

#ifndef TERRAIN_DRAWABLE_HPP_
#define TERRAIN_DRAWABLE_HPP_

#include <Component.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>

class TerrainDrawable final : public Component, public sf::Drawable, public sf::Transformable
{
public:
    using Ptr = std::unique_ptr<TerrainDrawable>;

    TerrainDrawable(float width, float startHeight, float endHeight, int deviation, MessageBus&);
    ~TerrainDrawable() = default;

    Component::Type type() const override;
    Component::UniqueType uniqueType() const override;
    void entityUpdate(Entity&, float) override;
    void handleMessage(const Message&) override;

    sf::FloatRect localBounds() const override;
    sf::FloatRect globalBounds() const override;

    Ptr clone(MessageBus&) const;

private:

    std::vector<sf::Vertex> m_vertices;
    sf::FloatRect m_bounds;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif //TERRAIN_DRAWABLE