//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//root class containing scene entities

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <xygine/Entity.hpp>
#include <xygine/PostBloom.hpp>
#include <xygine/PostChromeAb.hpp>
#include <xygine/Command.hpp>
#include <xygine/QuadTree.hpp>

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

class Scene final : public sf::Drawable
{
public:
    enum Layer
    {
        BackRear = 0,
        BackMiddle,
        BackFront,
        FrontRear,
        FrontMiddle,
        FrontFront,
        UI,
        Count
    };

    Scene(MessageBus&, bool createBuffers = true);
    ~Scene() = default;
    Scene(const Scene&) = delete;
    const Scene& operator = (const Scene&) = delete;

    void update(float);
    void handleMessage(const Message&);
    void addEntity(Entity::Ptr&, Layer);
    Entity* findEntity(sf::Uint64);
    Entity& getLayer(Layer);

    void setView(const sf::View& v);
    const sf::View& getView() const;

    void sendCommand(const Command&);

    sf::FloatRect getVisibleArea() const;
    void setSceneWidth(float);
    float getSceneWidth() const;

    std::vector<QuadTreeComponent*> queryQuadTree(const sf::FloatRect&);

    void reset();

    void drawDebug(bool);

private:
    QuadTree m_quadTree; //must live longer than any entity
    std::vector<Entity::Ptr> m_layers;
    std::vector<std::pair<Layer, Entity::Ptr>> m_pendingEntities;
    float m_sceneWidth;

    MessageBus& m_messageBus;
    CommandQueue m_commandQueue;

    bool m_drawDebug;

    mutable sf::RenderTexture m_sceneBufferA;
    mutable sf::RenderTexture m_sceneBufferB;
    mutable std::unique_ptr<PostBloom> m_bloomEffect;
    mutable std::unique_ptr<PostChromeAb> m_chromeAbEffect;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif //SCENE_HPP_