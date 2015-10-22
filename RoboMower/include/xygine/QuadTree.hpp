//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//quad tree class for spatial partitioning

#ifndef QUADTREE_HPP_
#define QUADTREE_HPP_

#include <xygine/QuadTreeNode.hpp>

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

class QuadTree final : public sf::Drawable
{
public:
    QuadTree();
    QuadTree(const QuadTree&) = delete;
    QuadTree& operator = (const QuadTree&) = delete;
    ~QuadTree() = default;

    void create(const sf::FloatRect&);
    bool created() const;
    void reset();
    void add(QuadTreeComponent*);
    const sf::FloatRect& getRootArea() const;

    //std::vector<QuadTreeComponent*> queryPosition(const sf::Vector2f&);
    std::vector<QuadTreeComponent*> queryArea(const sf::FloatRect&);
    
    QuadTreeNode::Set& getOutsideRootSet();

    sf::Int32 minNodeComponents() const;
    sf::Uint32 maxNodeComponents() const;
    sf::Int32 maxLevels() const;

private:
    //store components which move outside of the root area here
    QuadTreeNode::Set m_outsideRootSet;
    QuadTreeNode::Ptr m_rootNode;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif //QUADTREE_HPP_