//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <ShaderResource.hpp>

#include <SFML/Graphics/Shader.hpp>

#include <cassert>

ShaderResource::ShaderResource(){}

//public
sf::Shader& ShaderResource::get(Shader::Type type)
{
    auto result = m_shaders.find(type);
    assert(result != m_shaders.end()); //did you forget to preload this shader?

    return *result->second;
}

void ShaderResource::preload(Shader::Type type, const std::string& vertShader, const std::string& fragShader)
{
    auto shader = std::make_unique<sf::Shader>();
#ifndef _DEBUG_
    shader->loadFromMemory(vertShader, fragShader);
#else
    assert(shader->loadFromMemory(vertShader, fragShader));
#endif //_DEBUG_

    m_shaders.insert(std::make_pair(type, std::move(shader)));
}