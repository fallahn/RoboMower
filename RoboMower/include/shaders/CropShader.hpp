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

#ifndef CROP_SHADER_HPP_
#define CROP_SHADER_HPP_

#include <string>

namespace Shader
{
    static const std::string version = "#version 120\n";
    static const std::string useTexture = "#define TEXTURE\n";

    namespace Cropping
    {
        static const std::string vertex =
            "void main()\n" \
            "{\n" \
            "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n" \
            "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n" \
            "    gl_FrontColor = gl_Color;\n" \
            "}";
        static const std::string fragment = //remember frag Y coord inverted

            "uniform vec2 u_position = vec2(50.0, 50.0);\n" \
            "uniform vec2 u_size = vec2(150.0, 300.0);\n" \

            "#if defined(TEXTURE)\n" \
            "uniform sampler2D u_texture;\n" \
            "#endif\n" \

            "bool contains(vec2 point)\n" \
            "{\n" \
            "return (point.x > u_position.x && point.x < u_size.x + u_position.x && point.y > u_position.y && point.y < u_position.y + u_size.y);\n" \
            "}\n" \

            "void main()\n" \
            "{\n" \
            "if(!contains(gl_FragCoord.xy)) discard;\n" \
            "#if defined(TEXTURE)\n" \
            "vec4 colour = texture2D(u_texture, gl_TexCoord[0].xy);\n" \
            "gl_FragColor = vec4(colour.rgb * gl_Color.rgb, colour.a);\n" \
            "#else\n" \
            "gl_FragColor = gl_Color;\n" \
            "#endif\n" \
            "}";
    }
}

#endif //CROP_SHADER_HPP_
