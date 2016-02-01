//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <GameState.hpp>
#include <components/Tilemap.hpp>
#include <components/PlayerDrawable.hpp>
#include <components/PlayerLogic.hpp>

#include <xygine/Reports.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Command.hpp>
#include <xygine/PostChromeAb.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>

#include <xygine/components/ParticleSystem.hpp>
#include <CommandCategories.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>

namespace
{
    const sf::Keyboard::Key upKey = sf::Keyboard::W;
    const sf::Keyboard::Key downKey = sf::Keyboard::S;
    const sf::Keyboard::Key leftKey = sf::Keyboard::A;
    const sf::Keyboard::Key rightKey = sf::Keyboard::D;
    const sf::Keyboard::Key fireKey = sf::Keyboard::Space;

    const float joyDeadZone = 25.f;
    const float joyMaxAxis = 100.f;
}

GameState::GameState(xy::StateStack& stateStack, Context context)
    : State         (stateStack, context),
    m_messageBus    (context.appInstance.getMessageBus()),
    m_scene         (m_messageBus),
    m_gameUI        (context, m_textureResource, m_fontResource, m_scene)
{
    launchLoadingScreen();
    
    m_scene.setView(context.defaultView);
    //m_scene.drawDebug(true);
    auto pp = xy::PostProcess::create<xy::PostChromeAb>();
    m_scene.addPostProcess(pp);

    m_reportText.setFont(m_fontResource.get("assets/fonts/Console.ttf"));
    m_reportText.setPosition(1500.f, 30.f);

    buildMap();

    m_client.setServerInfo({ "127.0.0.1" }, 5600);
    m_client.connect();

    quitLoadingScreen();
}

bool GameState::update(float dt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
    
    m_gameUI.update(dt, mousePos);
    m_scene.update(dt);
    m_client.update(dt);

    m_reportText.setString(xy::StatsReporter::reporter.getString());

    return true;
}

void GameState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.draw(m_scene);
    rw.setView(getContext().defaultView);
    rw.draw(m_reportText);
}

bool GameState::handleEvent(const sf::Event& evt)
{
    switch (evt.type)
    {
    case sf::Event::KeyPressed:
        switch (evt.key.code)
        {
        case upKey:
            
            break;
        case downKey:
            
            break;
        case leftKey:
            
            break;
        case rightKey:
            
            break;
        case fireKey:
            
            break;
        default: break;
        }
        break;
    case sf::Event::KeyReleased:
        switch (evt.key.code)
        {
        case sf::Keyboard::Escape:
        case sf::Keyboard::P:
            requestStackPush(States::ID::MenuPaused);
            break;
        case upKey:
            
            break;
        case downKey:
            
            break;
        case leftKey:
            
            break;
        case rightKey:
            
            break;
        case fireKey:
            
            break;
        default: break;
        }
        break;
    case sf::Event::JoystickButtonPressed:
        switch (evt.joystickButton.button)
        {
        case 0:
            
            break;
        }
        break;
    case sf::Event::JoystickButtonReleased:

        switch (evt.joystickButton.button)
        {
        case 0:
            
            break;
        case 7:

            break;
        default: break;
        }
        break;
    }

    m_gameUI.handleEvent(evt);
    return true;
}

void GameState::handleMessage(const xy::Message& msg)
{ 
    m_gameUI.handleMessage(msg);
    m_scene.handleMessage(msg);
}

//private
void GameState::buildMap()
{
    static const sf::Vector2f mapPos(440.f, 40.f);
 
    //le background
    auto tilemap = xy::Component::create<Tilemap>(m_messageBus, m_textureResource.get("assets/images/tileset.png"));
    auto ent = xy::Entity::create(m_messageBus);
    ent->addComponent(tilemap);
    ent->setPosition(mapPos);

    //player
    auto playerDrawable = xy::Component::create<PlayerDrawable>(m_messageBus, m_textureResource.get("assets/images/tileset.png"), true);
    auto playerEnt = xy::Entity::create(m_messageBus);
    playerEnt->addComponent(playerDrawable);
    playerEnt->setPosition(224.f, 160.f);

    auto playerLogic = xy::Component::create<PlayerLogic>(m_messageBus);
    playerEnt->addComponent(playerLogic);

    xy::ParticleSystem::Definition pd;
    pd.loadFromFile("assets/particles/mow_up.xyp", m_textureResource);
    auto ps = pd.createSystem(m_messageBus);
    playerEnt->addComponent(ps)->setName("particle_up");
    pd.loadFromFile("assets/particles/mow_down.xyp", m_textureResource);
    ps = pd.createSystem(m_messageBus);
    playerEnt->addComponent(ps)->setName("particle_down");
    pd.loadFromFile("assets/particles/mow_left.xyp", m_textureResource);
    ps = pd.createSystem(m_messageBus);
    playerEnt->addComponent(ps)->setName("particle_left");
    pd.loadFromFile("assets/particles/mow_right.xyp", m_textureResource);
    ps = pd.createSystem(m_messageBus);
    playerEnt->addComponent(ps)->setName("particle_right");

    ent->addChild(playerEnt);
    
    m_scene.addEntity(ent, xy::Scene::Layer::BackRear);
}