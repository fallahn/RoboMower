//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//simulates chromatic aberration

#ifndef POST_CHROMEAB_HPP_
#define POST_CHROMEAB_HPP_

#include <PostProcess.hpp>
#include <ShaderResource.hpp>

class PostChromeAb final : public PostProcess
{
public:
    PostChromeAb();

    void apply(const sf::RenderTexture&, sf::RenderTarget&) override;
    void update(float);

private:
    ShaderResource m_shaderResource;
};

#endif //POST_CHROMEAB_HPP_