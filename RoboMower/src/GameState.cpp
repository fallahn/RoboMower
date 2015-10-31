//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <GameState.hpp>

#include <xygine/Reports.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Command.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>
#include <xygine/Util.hpp>

#include <xygine/ParticleController.hpp>
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

    std::vector<sf::Vector2f> explodeVelocities =
    {
        { -80.5f, 0.f },
        { -60.f, -8.9f },
        { -20.f, -24.f },
        { 0.f, -40.5f },
        { 28.5f, -24.6f },
        { 64.f, -8.5f },
        { 80.9f, 0.f },
        { 64.f, 9.5f },
        { 28.f, 27.5f },
        { 0.7f, 40.4f },
        { -20.f, 29.6f },
        { -60.f, 9.5f }
    };
}

GameState::GameState(xy::StateStack& stateStack, Context context)
    : State         (stateStack, context),
    m_messageBus    (context.appInstance.getMessageBus()),
    m_scene         (m_messageBus),
    m_gameUI        (context, m_scene)
{
    //m_audioManager.mute(context.appInstance.getAudioSettings().muted);
    m_scene.setView(context.defaultView);
    //m_scene.drawDebug(true);
    m_scene.setPostEffects(xy::Scene::PostEffect::ChromaticAbberation);

    m_reportText.setFont(context.appInstance.getFont("assets/fonts/Console.ttf"));
    m_reportText.setPosition(1500.f, 30.f);


    //create a particle controller for scene TEMP!!
    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addCommandCategories(CommandCategory::ParticleController);

    auto pc = std::make_unique<xy::ParticleController>(m_messageBus);

    xy::ParticleSystem::Definition def;
    def.randomInitialVelocities = explodeVelocities;
    def.releaseCount = 5u;
    def.delay = 0.f;
    def.duration = 0.1f;
    def.followParent = true;

    xy::ForceAffector fa({ 0.f, 150.f });
    xy::ParticleSystem::Affector a = std::bind(fa, std::placeholders::_1, std::placeholders::_2);
    def.affectors.push_back(a);

    entity->addComponent<xy::ParticleController>(pc);
    entity->getComponent<xy::ParticleController>()->addDefinition(0, def);
    m_scene.addEntity(entity, xy::Scene::Layer::UI);
}

bool GameState::update(float dt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
    
    m_gameUI.update(dt, mousePos);
    m_audioManager.update(dt);
    m_scene.update(dt);

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
    m_audioManager.handleMessage(msg);
    m_scene.handleMessage(msg);
}

//private
