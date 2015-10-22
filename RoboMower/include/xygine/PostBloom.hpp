//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//creates a 'bloom' effect via post process

#ifndef POST_BLOOM_HPP_
#define POST_BLOOM_HPP_

#include <xygine/PostProcess.hpp>
#include <xygine/ShaderResource.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <array>

class PostBloom final : public PostProcess
{
public:
    PostBloom();

    void apply(const sf::RenderTexture&, sf::RenderTarget&) override;

private:
    using RenderTextureArray = std::array<sf::RenderTexture, 2>;

    ShaderResource m_shaderResource;
    sf::RenderTexture m_brightnessTexture;
    RenderTextureArray m_firstPassTextures;
    RenderTextureArray m_secondPassTextures;

    void initTextures(const sf::Vector2u&);
    void filterBright(const sf::RenderTexture&, sf::RenderTexture&);
    void blurMultipass(RenderTextureArray&);
    void blur(const sf::RenderTexture&, sf::RenderTexture&, const sf::Vector2f&);
    void downSample(const sf::RenderTexture&, sf::RenderTexture&);
    void add(const sf::RenderTexture&, const sf::RenderTexture&, sf::RenderTarget&);
};

#endif //POST_BLOOM_HPP_