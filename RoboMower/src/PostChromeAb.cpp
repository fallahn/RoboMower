//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/PostChromeAb.hpp>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace
{
    float accumulatedTime = 0.f;
    const float scanlineCount = 5500.f;
}

PostChromeAb::PostChromeAb()
{
    m_shaderResource.preload(Shader::Type::PostChromeAb, Shader::FullPass::vertex, Shader::PostChromeAb::fragment);
}

//public
void PostChromeAb::apply(const sf::RenderTexture& src, sf::RenderTarget& dst)
{
    float windowRatio = static_cast<float>(dst.getSize().y) / static_cast<float>(src.getSize().y);

    auto& shader = m_shaderResource.get(Shader::Type::PostChromeAb);
    shader.setParameter("u_sourceTexture", src.getTexture());
    shader.setParameter("u_time", accumulatedTime * (10.f * windowRatio));
    shader.setParameter("u_lineCount", windowRatio  * scanlineCount);

    applyShader(shader, dst);
}

void PostChromeAb::update(float dt)
{
    accumulatedTime += dt;
}
