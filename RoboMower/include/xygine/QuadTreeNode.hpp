//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//node within the quad tree containing quad tree components

#ifndef QUADTREE_NODE_HPP_
#define QUADTREE_NODE_HPP_

#include <xygine/QuadTreeComponent.hpp>

#include <SFML/Graphics/Vertex.hpp>

#include <array>
#include <memory>
#include <unordered_set>

class QuadTree;
class QuadTreeNode final
{
public:
    using Ptr = std::unique_ptr<QuadTreeNode>;
    using Set = std::unordered_set<QuadTreeComponent*>;

    QuadTreeNode();
    QuadTreeNode(const sf::FloatRect& area, sf::Int32 level, QuadTreeNode* parent, QuadTree* quadTree);
    QuadTreeNode(const QuadTreeNode&) = delete;
    QuadTreeNode& operator = (const QuadTreeNode&) = delete;
    ~QuadTreeNode() = default;

    void create(const sf::FloatRect& area, sf::Int32 level, QuadTreeNode* parent, QuadTree* quadTree);
    QuadTree* getTree() const;
    void add(QuadTreeComponent*);
    const sf::FloatRect& getArea() const;

    sf::Int32 getNumComponentsBelow() const;

    void update(QuadTreeComponent*);
    void remove(QuadTreeComponent*);

    const Set& getComponents() const;
    bool hasChildren() const;
    const std::array<Ptr, 4u>& getChildren() const;

    //used to draw debug output
    void getVertices(std::vector<sf::Vertex>&);

private:
    QuadTreeNode* m_parent;
    QuadTree* m_quadTree;

    bool m_hasChildren;
    std::array<Ptr, 4u> m_children;
    Set m_components;

    sf::FloatRect m_area;
    sf::Int32 m_level;
    sf::Int32 m_numComponentsBelow;

    void getSubComponents();
    sf::Vector2i getPossiblePosition(QuadTreeComponent*);
    void addToThis(QuadTreeComponent*);
    bool addToChildren(QuadTreeComponent*); //true if successful
    void destroyChildren();

    void split();
    void join();
    void clearDestroyed();
};

#endif //QUADTREE_NODE_HPP_