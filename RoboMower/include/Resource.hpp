//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//resource manager

#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>

#include <memory>
#include <array>

template <class T>
class BaseResource
{
public:
    BaseResource()
    {
    }
    virtual ~BaseResource() = default;
    BaseResource(const BaseResource&) = delete;
    const BaseResource& operator = (const BaseResource&) = delete;

    T& get(const std::string& path = "default")
    {
        //if we have a valid path check current resources and return if found
        if (!path.empty())
        {
            auto r = m_resources.find(path);
            if (r != m_resources.end())
            {
                return *r->second;
            }
        }
        //else attempt to load from file
        std::unique_ptr<T> r = std::unique_ptr<T>(new T());
        if (path.empty() || !r->loadFromFile(path))
        {
            m_resources[path] = errorHandle(); //error handle should return message endl
        }
        else
        {
            m_resources[path] = std::move(r);
        }

        return *m_resources[path];
    }
protected:
    virtual std::unique_ptr<T> errorHandle() = 0;
private:
    std::map<std::string, std::unique_ptr<T>> m_resources;
};

class TextureResource final : public BaseResource<sf::Texture>
{
private:
    std::unique_ptr<sf::Texture> errorHandle() override
    {
        std::unique_ptr<sf::Texture> t(new sf::Texture());
        sf::Image i;
        i.create(20u, 20u, sf::Color(127u, 127u, 255u));
        t->loadFromImage(i);
        return std::move(t);
    }
};
class ImageResource final : public BaseResource<sf::Image>
{
    std::unique_ptr<sf::Image> errorHandle() override
    {
        std::unique_ptr<sf::Image> i(new sf::Image);
        i->create(20u, 20u, sf::Color::Green);
        return std::move(i);
    }
};

class FontResource final : public BaseResource<sf::Font>
{
public:
    FontResource();
private:
    sf::Font m_font;
    std::unique_ptr<sf::Font> errorHandle() override;
};

#endif //RESOURCES_HPP_