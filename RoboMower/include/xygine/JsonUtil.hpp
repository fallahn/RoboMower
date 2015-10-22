//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//utility functions for working with picojson and sfml

#ifndef JSON_UTIL_H_
#define JSON_UTIL_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <xygine/picojson.h>

#include <fstream>

namespace Util
{
    namespace Json
    {

        static inline void parseJsonObject(const picojson::object& o, sf::FloatRect& rect)
        {
            for (const auto& p : o)
            {
                if (p.first == "x")
                {
                    rect.left = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "y")
                {
                    rect.top = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "w")
                {
                    rect.width = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "h")
                {
                    rect.height = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
            }
        }

        static inline void parseJsonObject(const picojson::object& o, sf::Vector2f& v)
        {
            for (const auto& p : o)
            {
                if (p.first == "x" || p.first == "w")
                {
                    v.x = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
                else if (p.first == "y" || p.first == "h")
                {
                    v.y = (p.second.is<double>()) ?
                        static_cast<float>(p.second.get<double>()) : 0;
                }
            }
        }
    }

    namespace File
    {
        static inline bool validLength(std::ifstream& file)
        {
            file.seekg(0, file.end);
            sf::Int32 fileLength = static_cast<sf::Int32>(file.tellg());
            file.seekg(0, file.beg);
            return (fileLength > 0);
        }
    }
}

#endif //JSON_UTIL_H_