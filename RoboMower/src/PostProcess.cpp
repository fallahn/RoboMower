//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/PostProcess.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/VertexArray.hpp>

PostProcess::PostProcess(){}

//protected
void PostProcess::applyShader(const sf::Shader& shader, sf::RenderTarget& dest)
{
    auto outputSize = static_cast<sf::Vector2f>(dest.getSize());

    sf::VertexArray verts(sf::TrianglesStrip, 4);
    verts[0] = { {}, { 0.f, 1.f } };
    verts[1] = { { outputSize.x, 0.f }, { 1.f, 1.f } };
    verts[2] = { { 0.f, outputSize.y }, sf::Vector2f() };
    verts[3] = { outputSize, { 1.f, 0 } };

    //All the seagulls are belong to us.
    sf::RenderStates states;
    states.shader = &shader;
    states.blendMode = sf::BlendNone;

    dest.draw(verts, states);
}
