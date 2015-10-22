//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//resource for caching shaders

#ifndef SHADER_RESOURCE_HPP_
#define SHADER_RESOURCE_HPP_

#include <xygine/Shaders.hpp>

#include <map>
#include <memory>

namespace sf
{
    class Shader;
}

class ShaderResource final
{
public:

    ShaderResource();
    ~ShaderResource() = default;
    ShaderResource(const ShaderResource&) = delete;
    const ShaderResource& operator = (const ShaderResource&) = delete;

    sf::Shader& get(Shader::Type);
    void preload(Shader::Type, const std::string&, const std::string&);

private:
    std::map<Shader::Type, std::unique_ptr<sf::Shader>> m_shaders;
};

#endif //SHADER_RESOURCE_HPP_