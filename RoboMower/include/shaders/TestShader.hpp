//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef TEST_SHADER_HPP_
#define TEST_SHADER_HPP_

#include <string>

namespace Shader
{
    static const std::string version = "#version 120\n";
    static const std::string useTexture = "#define TEXTURE\n";

    namespace Testing
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

#endif //TEST_SHADER_HPP_
