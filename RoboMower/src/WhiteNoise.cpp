/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#include <components/WhiteNoise.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <string>

namespace
{
    float accumulatedTime = 0.f;

    const std::string shaderFrag =
        "#version 130\n"

        "uniform sampler2D u_texture;\n"
        "uniform float u_time;\n"

        "const float brightness = 0.25;\n"
        "const float vignetteSoftness = 0.15;\n"
        "const float vignetteRadius = 0.75;\n"
        "const float vignetteAmount = 0.55;\n"

        "void main()\n"
        "{\n"
        "    float value = clamp(fract(sin(dot(gl_TexCoord[0].xy * u_time ,vec2(12.9898,78.233))) * 43758.5453), 0.0, brightness);\n"
        "    float vignette = smoothstep(vignetteRadius, vignetteSoftness, length(gl_TexCoord[0].xy - vec2(0.5)));\n"
        "    float colour = mix(value, value * vignette, vignetteAmount);\n"
        "    gl_FragColor = vec4(vec3(colour), 1.0);\n"
        "}";
}

WhiteNoise::WhiteNoise(xy::MessageBus& mb)
    : xy::Component(mb, this)
{
    sf::Vector2f floatSize(xy::DefaultSceneSize / 4.f);
    sf::Vector2u intSize(floatSize);
    m_texture.create(intSize.x, intSize.y);
    m_renderTexture.create(intSize.x, intSize.y);

    m_vertices =
    {
        sf::Vertex({0.f, 0.f}, {0.f, 0.f}),
        { {xy::DefaultSceneSize.x, 0.f}, {floatSize.x, 0.f} },
        { xy::DefaultSceneSize, floatSize },
        { {0.f, xy::DefaultSceneSize.y}, {0.f, floatSize.y} }
    };

    m_shader.loadFromMemory(shaderFrag, sf::Shader::Fragment);
}

//public
void WhiteNoise::entityUpdate(xy::Entity&, float dt)
{
    m_shader.setUniform("u_time", accumulatedTime += dt);
}

//private
void WhiteNoise::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    m_renderTexture.clear();
    m_renderTexture.draw(sf::Sprite(m_texture), &m_shader);
    m_renderTexture.display();
    
    states.texture = &m_renderTexture.getTexture();
    rt.draw(m_vertices.data(), m_vertices.size(), sf::Quads, states);
}