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
    namespace Testing
    {
        static const std::string vertex;
        static const std::string fragment = //remember frag Y coord inverted
            "#version 120\n" \

            "uniform vec2 u_position = vec2(50.0, 50.0);\n" \
            "uniform vec2 u_size = vec2(150.0, 300.0);\n" \

            "bool contains(vec2 point)\n" \
            "{\n" \
            "return (point.x > u_position.x && point.x < u_size.x + u_position.x && point.y > u_position.y && point.y < u_position.y + u_size.y);\n" \
            "}\n" \

            "void main()\n" \
            "{\n" \
            "if(!contains(gl_FragCoord.xy)) discard;\n" \
            "gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" \
            "}";
    }
}

#endif //TEST_SHADER_HPP_
