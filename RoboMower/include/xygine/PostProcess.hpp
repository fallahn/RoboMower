//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//abstract base class for full screen post process effects

#ifndef POST_PROCESS_HPP_
#define POST_PROCESS_HPP_

namespace sf
{
    class RenderTarget;
    class RenderTexture;
    class Shader;
}

class PostProcess
{
public:
    PostProcess();
    virtual ~PostProcess() = default;
    PostProcess(const PostProcess&) = delete;
    const PostProcess& operator = (const PostProcess&) = delete;

    virtual void apply(const sf::RenderTexture&, sf::RenderTarget&) = 0;

protected:
    static void applyShader(const sf::Shader&, sf::RenderTarget&);
};


#endif //POST_PROCESS_HPP_