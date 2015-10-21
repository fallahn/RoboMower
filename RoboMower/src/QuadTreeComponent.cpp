//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <QuadTreeComponent.hpp>
#include <QuadTreeNode.hpp>
#include <Entity.hpp>

#include <cassert>

namespace
{
    const std::size_t maxBuffer = 37u; //should allow ~600ms input lag
}

QuadTreeComponent::QuadTreeComponent(MessageBus& mb, const sf::FloatRect& bounds)
    : Component     (mb),
    m_bounds        (bounds),
    m_quadTree      (nullptr),
    m_quadTreeNode  (nullptr),
    m_entity        (nullptr)
{

}

QuadTreeComponent::~QuadTreeComponent()
{
    removeFromQuadTree();
}

//public
Component::Type QuadTreeComponent::type() const
{
    return Component::Type::Physics;
}

Component::UniqueType QuadTreeComponent::uniqueType() const
{
    return Component::UniqueType::QuadTreeComponent;
}

void QuadTreeComponent::entityUpdate(Entity& entity, float dt)
{
    if (m_transformBuffer.size() == maxBuffer) m_transformBuffer.pop_front();
    m_transformBuffer.push_back(std::make_pair(m_timer.getElapsedTime().asMilliseconds(), m_transform));

    m_transform = entity.getTransform();
    updateQuadTree(); //TODO can we only call this when needed? or is there no point checking?
}

void QuadTreeComponent::handleMessage(const Message&){}

void QuadTreeComponent::onStart(Entity& entity)
{
    m_transform = entity.getTransform();
    m_entity = &entity;
}

void QuadTreeComponent::destroy()
{
    removeFromQuadTree();
    Component::destroy();
}

sf::FloatRect QuadTreeComponent::localBounds() const
{
    return m_bounds;
}

sf::FloatRect QuadTreeComponent::globalBounds() const
{
    return m_transform.transformRect(m_bounds);
}

sf::FloatRect QuadTreeComponent::globalBounds(sf::Int32 latency)
{
    auto time = m_timer.getElapsedTime().asMilliseconds() - latency;
    assert(time > 0);
    //reverse iterate because the newest transform is at the back
    auto transform = std::find_if(m_transformBuffer.rbegin(), m_transformBuffer.rend(),
        [time](const std::pair<sf::Int32, sf::Transform>& pair)
    {
        return pair.first <= time;
    });

    if (transform != m_transformBuffer.rend())
    {
        //LOG("SERVER found transform", Logger::Type::Info);
        return transform->second.transformRect(m_bounds);
    }

    return m_transform.transformRect(m_bounds);    
}

void QuadTreeComponent::updateQuadTree()
{
    if (m_quadTreeNode) m_quadTreeNode->update(this);
}

void QuadTreeComponent::removeFromQuadTree()
{
    if (m_quadTreeNode)
    {
        m_quadTreeNode->remove(this);
        m_quadTreeNode = nullptr;
        //LOG("SERVER Removing myself from quad tree", Logger::Type::Info);
    }
    //LOG("Tried to remove myself from quad tree, but node pointer is null!", Logger::Type::Info);
}

void QuadTreeComponent::setQuadTree(QuadTree* qt)
{
    m_quadTree = qt;
}

void QuadTreeComponent::setQuadTreeNode(QuadTreeNode* qn)
{
    m_quadTreeNode = qn;
}

Entity* QuadTreeComponent::getEntity()
{
    return m_entity;
}