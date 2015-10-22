//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//represents an AABB for quad tree partitioning

#ifndef QUADTREE_COMPONENT_HPP_
#define QUADTREE_COMPONENT_HPP_

#include <xygine/Component.hpp>

#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Clock.hpp>

#include <list>

class QuadTree;
class QuadTreeNode;
class QuadTreeComponent final : public Component
{
public:
    QuadTreeComponent(MessageBus&, const sf::FloatRect& bounds);
    ~QuadTreeComponent();

    Component::Type type() const override;
    Component::UniqueType uniqueType() const override;
    void entityUpdate(Entity&, float) override;
    void handleMessage(const Message&) override;
    void onStart(Entity&) override;
    void destroy() override;

    sf::FloatRect localBounds() const override;
    sf::FloatRect globalBounds() const override;
    sf::FloatRect globalBounds(sf::Int32 latency);

    void updateQuadTree();
    void removeFromQuadTree();

    //TODO rather than expose these friend QuadTreeNode?
    void setQuadTree(QuadTree*);
    void setQuadTreeNode(QuadTreeNode*);

    Entity* getEntity();

private:
    sf::Transform m_transform;
    sf::FloatRect m_bounds;

    QuadTree* m_quadTree;
    QuadTreeNode* m_quadTreeNode;

    sf::Clock m_timer;
    std::list<std::pair<sf::Int32, sf::Transform>> m_transformBuffer;

    Entity* m_entity;
};

#endif //QUADTREE_COMPONENT_HPP_