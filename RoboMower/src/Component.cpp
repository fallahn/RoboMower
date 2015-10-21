//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <Component.hpp>
#include <MessageBus.hpp>

Component::Component(MessageBus& m)
    : m_messageBus  (m),
    m_destroyed     (false),
    m_parentUID     (0u)
{

}

Component::~Component()
{
    Message msg;
    msg.type = Message::Type::ComponentSystem;
    msg.component.action = Message::ComponentEvent::Deleted;
    msg.component.ptr = this;
    msg.component.entityId = m_parentUID;
    m_messageBus.post(msg);
}

//public
void Component::onStart(Entity&)
{

}

void Component::destroy()
{
    m_destroyed = true;
}

bool Component::destroyed() const
{
    return m_destroyed;
}

void Component::setParentUID(sf::Uint64 uid)
{
    m_parentUID = uid;
}

void Component::setName(const std::string& name)
{
    m_name = name;
}

const std::string& Component::getName() const
{
    return m_name;
}

sf::FloatRect Component::localBounds() const
{
    return sf::FloatRect();
}

sf::FloatRect Component::globalBounds() const
{
    return sf::FloatRect();
}

//protected
void Component::sendMessage(const Message& m)
{
    m_messageBus.post(m);
}

MessageBus& Component::getMessageBus() const
{
    return m_messageBus;
}

sf::Uint64 Component::getParentUID() const
{
    return m_parentUID;
}