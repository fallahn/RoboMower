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
#include <xygine/AnimatedDrawable.hpp>
#include <xygine/Reports.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Command.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>
#include <xygine/Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>

#include <components/RoundedRectangle.hpp>
#include <components/ComponentIds.hpp>
#include <components/ButtonLogic.hpp>
#include <CommandCategories.hpp>
#include <InstructionSet.hpp>

namespace
{
    const sf::Keyboard::Key upKey = sf::Keyboard::W;
    const sf::Keyboard::Key downKey = sf::Keyboard::S;
    const sf::Keyboard::Key leftKey = sf::Keyboard::A;
    const sf::Keyboard::Key rightKey = sf::Keyboard::D;
    const sf::Keyboard::Key fireKey = sf::Keyboard::Space;

    const float joyDeadZone = 25.f;
    const float joyMaxAxis = 100.f;

    xy::Entity* mouseCursor = nullptr; //we need direct access to cursor entity else lag is horrible
}

GameState::GameState(xy::StateStack& stateStack, Context context)
    : State                 (stateStack, context),
    m_messageBus            (context.appInstance.getMessageBus()),
    m_scene                 (m_messageBus)
{
    //m_audioManager.mute(context.appInstance.getAudioSettings().muted);
    m_scene.setView(context.defaultView);
    //m_scene.drawDebug(true);
    m_scene.setPostEffects(xy::Scene::PostEffect::ChromaticAbberation);

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

    rw.draw(m_scene);

    rw.setView(getContext().defaultView);
}

bool GameState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
    mouseCursor->setWorldPosition(mousePos);

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
    case sf::Event::MouseButtonPressed:
        if (evt.mouseButton.button == sf::Mouse::Left)
        {
            xy::Command cmd;
            cmd.category = CommandCategory::TrayIcon;
            cmd.action = [mousePos](xy::Entity& ent, float)
            {
                if (ent.getComponent<RoundedRectangle>()->globalBounds().contains(mousePos))
                {
                    ent.getComponent<ButtonLogicScript>()->doClick();
                }
            };
            m_scene.sendCommand(cmd);
        }
        else if (evt.mouseButton.button == sf::Mouse::Right)
        {
            
        }
        break;
    case sf::Event::MouseButtonReleased:
        if (evt.mouseButton.button == sf::Mouse::Left)
        {
            
        }
        else if (evt.mouseButton.button == sf::Mouse::Right)
        {
            
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
namespace
{
    std::map<Instruction, std::string> instructionLabels = 
    {
        { Instruction::EngineOn, "Motor On" },
        { Instruction::EngineOff, "Motor Off" },
        { Instruction::Forward, "Forward" },
        { Instruction::Left, "Left" },
        { Instruction::Right, "Right" },
        { Instruction::Loop, "Loop" }
    };
    const sf::Uint8 buttonCount = 6u;

    const float labelSpacing = 240.f;
    const float labelPadding = 435.f;
    const float labelTop = 960.f;

    const sf::Vector2f labelSize(220.f, 50.f);

    std::unique_ptr<RoundedRectangle> makeButtonBackground(xy::MessageBus& messageBus)
    {
        auto rr = std::make_unique<RoundedRectangle>(messageBus, labelSize);
        rr->setFillColor({ 220u, 240u, 10u, 180u });
        rr->setOutlineThickness(6.f);
        rr->setOutlineColor({ 10u, 230u, 10u });
        return std::move(rr);
    }
}

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

    m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);

    //command tray
    rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(1450.f, 90.f), 20.f);
    rr->setFillColor({ 180u, 40u, 20u, 180u });
    rr->setOutlineColor({ 100u, 30u, 15u });
    rr->setOutlineThickness(8.f);

    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<RoundedRectangle>(rr);
    entity->setPosition(420.f, 940.f);

    m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);

    //create 'buttons' in tray
    auto it = instructionLabels.begin();
    for (auto i = 0u; i < buttonCount; ++i, ++it)
    {
        entity = std::make_unique<xy::Entity>(m_messageBus);
        entity->setPosition(labelPadding + (i * labelSpacing), labelTop);
        entity->addCommandCategories(CommandCategory::TrayIcon);
        entity->addComponent<RoundedRectangle>(makeButtonBackground(m_messageBus));
        entity->addComponent<ButtonLogicScript>(std::make_unique<ButtonLogicScript>(m_messageBus, it->first));

        auto text = std::make_unique<xy::TextDrawable>(m_messageBus);
        text->setFont(getContext().appInstance.getFont("flaps"));
        text->setString(it->second);
        text->setColor(sf::Color::Black);
        xy::Util::Position::centreOrigin(*text);
        text->setPosition(labelSize / 2.f);
        text->move(0.f, -6.f);

        entity->addComponent<xy::TextDrawable>(text);

        m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);
    }

    //add mouse cursor
    auto ad = std::make_unique<xy::AnimatedDrawable>(m_messageBus, getContext().appInstance.getTexture("assets/images/ui/cursor.png"));
    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<xy::AnimatedDrawable>(ad);
    entity->addCommandCategories(CommandCategory::Cursor);
    entity->setPosition(getContext().renderWindow.mapPixelToCoords(sf::Mouse::getPosition(getContext().renderWindow)));
    mouseCursor = entity.get();
    m_scene.addEntity(entity, xy::Scene::Layer::UI);
}

void GameState::addInstructionBlock()
{
    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->setPosition(labelPadding, labelTop); //TODO place based on cursor

    entity->addComponent<RoundedRectangle>(makeButtonBackground(m_messageBus));

    auto text = std::make_unique<xy::TextDrawable>(m_messageBus);
    text->setFont(getContext().appInstance.getFont("flaps"));
    text->setString("Pen Up");
    text->setColor(sf::Color::Black);
    xy::Util::Position::centreOrigin(*text);
    text->setPosition(labelSize / 2.f);

    entity->addComponent<xy::TextDrawable>(text);

    //TODO add logic component

    m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);
}