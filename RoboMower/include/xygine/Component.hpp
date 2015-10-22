//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//abstract base class for entity components

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <string>

class Entity;
class Message;
class MessageBus;
class Component
{
public:
    using Ptr = std::unique_ptr<Component>;

    enum class Type
    {
        Drawable,
        Physics,
        Script
    };

    enum class UniqueType
    {
        AlienController,
        AnimatedDrawable,
        AnimationController,
        GameController,
        HumanController,
        LaserDrawable,
        NetworkController,
        ParticleController,
        ParticleSystem,
        PlayerController,
        QuadTreeComponent,
        TerrainDrawable,
        TextDrawable
    };

    explicit Component(MessageBus&);
    virtual ~Component();
    Component(const Component&) = delete;
    const Component& operator = (const Component&) = delete;

    virtual Type type() const = 0;
    virtual UniqueType uniqueType() const = 0;
    //this is called once per frame by the component's parent entity
    //providing the opportunity to update each other
    virtual void entityUpdate(Entity&, float) = 0;
    virtual void handleMessage(const Message&) = 0;

    //called when the component is first added to an entity
    virtual void onStart(Entity&);

    virtual void destroy();
    bool destroyed() const;

    void setParentUID(sf::Uint64 uid);
    sf::Uint64 getParentUID() const;

    void setName(const std::string&);
    const std::string& getName() const;

    virtual sf::FloatRect localBounds() const;
    virtual sf::FloatRect globalBounds() const;

protected:
    void sendMessage(const Message&);
    MessageBus& getMessageBus() const;

private:
    MessageBus& m_messageBus;
    bool m_destroyed;

    sf::Uint64 m_parentUID;
    std::string m_name;
};

#endif //COMPONENT_HPP_