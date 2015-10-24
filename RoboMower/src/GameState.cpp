//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <GameState.hpp>

#include <xygine/TextDrawable.hpp>
#include <xygine/ParticleSystem.hpp>
#include <xygine/AnimationController.hpp>
#include <xygine/Reports.hpp>
#include <xygine/Entity.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>
#include <xygine/Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>

#include <components/RoundedRectangle.hpp>
#include <components/ComponentIds.hpp>

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
    : State                 (stateStack, context),
    m_messageBus            (context.appInstance.getMessageBus()),
    m_scene                 (m_messageBus)
{
    //m_audioManager.mute(context.appInstance.getAudioSettings().muted);
    m_scene.setView(context.defaultView);
    //m_scene.drawDebug(true);

    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));


    buildUI();
}

bool GameState::update(float dt)
{
    m_audioManager.update(dt);
    m_scene.update(dt);

    return true;
}

void GameState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().renderWindow.getDefaultView());   
    rw.draw(m_scene);

    rw.setView(getContext().defaultView);
    rw.draw(m_cursorSprite);
}

bool GameState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
    //TODO pass to UI
    m_cursorSprite.setPosition(mousePos);
    
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
    default: break;
    }
    return true;
}

void GameState::handleMessage(const xy::Message& msg)
{ 
    m_audioManager.handleMessage(msg);
    m_scene.handleMessage(msg);
}

//private
void GameState::buildUI()
{
    //command list
    auto rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(320.f, 980.f));
    rr->setFillColor({ 20u, 40u, 180u, 180u });
    rr->setOutlineThickness(10.f);
    rr->setOutlineColor({ 15u, 30u, 100u });
    
    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<RoundedRectangle>(rr);
    entity->setPosition(50.f, 50.f);

    m_scene.getLayer(xy::Scene::Layer::UI).addChild(entity);

    //command tray
    rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(1450.f, 180.f), 39.f);
    rr->setFillColor({ 180u, 40u, 20u, 180u });
    rr->setOutlineColor({ 100u, 30u, 15u });
    rr->setOutlineThickness(8.f);

    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<RoundedRectangle>(rr);
    entity->setPosition(420.f, 850.f);

    m_scene.getLayer(xy::Scene::Layer::UI).addChild(entity);

    addInstructionBlock();
}

void GameState::addInstructionBlock()
{
    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->setPosition(455.f, 885.f); //TODO place based on type
        
    auto rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(220.f, 110.f));
    rr->setFillColor({ 220u, 240u, 10u, 180u });
    rr->setOutlineThickness(6.f);
    rr->setOutlineColor({ 10u, 230u, 10u });

    entity->addComponent<RoundedRectangle>(rr);

    auto text = std::make_unique<xy::TextDrawable>(m_messageBus);
    text->setFont(getContext().appInstance.getFont("flaps"));
    text->setString("Pen Up");
    text->setColor(sf::Color::Black);
    xy::Util::Position::centreOrigin(*text);
    text->setPosition(110.f, 55.f);

    entity->addComponent<xy::TextDrawable>(text);

    //TODO add logic component

    auto b = entity->getComponent<RoundedRectangle>(ComponentId::RoundedRectangle);
    auto r = b->getGlobalBounds();
    xy::StatsReporter::reporter.report("Global Bounds", std::to_string(r.left) + ", " + std::to_string(r.top) + ",\n " + std::to_string(r.width) + ", " + std::to_string(r.height));

    m_scene.getLayer(xy::Scene::Layer::UI).addChild(entity);
}