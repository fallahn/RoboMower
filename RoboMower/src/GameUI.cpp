//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <GameUI.hpp>

#include <xygine/Scene.hpp>
#include <xygine/Entity.hpp>
#include <xygine/MessageBus.hpp>
#include <xygine/Util.hpp>
#include <xygine/AnimatedDrawable.hpp>
#include <xygine/App.hpp>
#include <xygine/Reports.hpp>
#include <xygine/SfDrawableComponent.hpp>

#include <RoundedRectangle.hpp>
#include <components/ButtonLogic.hpp>
#include <components/InstructionBlockLogic.hpp>
#include <components/StackLogicComponent.hpp>
#include <components/ScrollHandleLogic.hpp>
#include <components/InputWindow.hpp>
#include <components/LoopHandle.hpp>
#include <CommandCategories.hpp>
#include <Messages.hpp>

#include <shaders/ShaderIds.hpp>
#include <shaders/CropShader.hpp>
#include <xygine/Shaders.hpp>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
    const float labelPadding = 445.f;
    const float labelTop = 980.f;
    const float textOffset = -6.f;

    const sf::Vector2f labelSize(220.f, 30.f);
    const sf::Vector2f inputSize(50.f, 30.f);

    const sf::Vector2f stackPosition(50.f, 50.f);
    const sf::Vector2f stackSize(320.f, 980.f);

    const sf::Vector2f trayPosition(430.f, 960.f);
    const sf::Vector2f traySize(1450.f, 70.f);

    const sf::Color fillColour(180u, 40u, 20u, 180u);
    const sf::Color borderColour(15u, 30u, 100u);

    const float scrollBarRadius = 8.f;

    const float inputBoxSpacing = 26.f;

    std::unique_ptr<xy::SfDrawableComponent<RoundedRectangle>> makeButtonBackground(xy::MessageBus& messageBus)
    {
        auto rr = std::make_unique<xy::SfDrawableComponent<RoundedRectangle>>(messageBus);
        auto& shape = rr->getDrawable();
        shape.setFillColor({ 220u, 240u, 10u, 180u });
        shape.setOutlineThickness(6.f);
        shape.setOutlineColor({ 10u, 230u, 10u });
        shape.setSize(labelSize);
        return std::move(rr);
    }

    std::unique_ptr<xy::SfDrawableComponent<RoundedRectangle>> makeInputBackground(xy::MessageBus& messageBus)
    {
        auto rr = std::make_unique<xy::SfDrawableComponent<RoundedRectangle>>(messageBus);
        auto& shape = rr->getDrawable();
        shape.setFillColor({ 220u, 40u, 210u, 180u });
        shape.setOutlineThickness(6.f);
        shape.setOutlineColor({ 10u, 230u, 10u });
        shape.setSize(inputSize);
        return std::move(rr);
    }
}

GameUI::GameUI(xy::State::Context sc, xy::Scene& scene)
    : m_stateContext    (sc),
    m_scene             (scene),
    m_messageBus        (sc.appInstance.getMessageBus()),
    m_mouseCursor       (nullptr),
    m_instructionStack  (nullptr),
    m_inputWindowVisible(false)
{
    //command list
    auto rr = std::make_unique<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape = rr->getDrawable();
    shape.setFillColor(fillColour);
    shape.setOutlineThickness(10.f);
    shape.setOutlineColor(borderColour);
    shape.setSize(stackSize);

    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);
    entity->setPosition(stackPosition);
    auto backPanel = m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    //the logic is separated from the background so we can scroll it
    auto subEnt = std::make_unique<xy::Entity>(m_messageBus);
    subEnt->addCommandCategories(CommandCategory::InstructionList);
    auto scl = std::make_unique<StackLogicComponent>(m_messageBus, labelSize, stackSize);
    subEnt->addComponent<StackLogicComponent>(scl);
    m_instructionStack = subEnt.get();
    backPanel->addChild(subEnt); //backPanel has to be added first so it has a valid scene pointer

    //command tray
    rr = std::make_unique<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape2 = rr->getDrawable();
    shape2.setFillColor(fillColour);
    shape2.setOutlineColor(borderColour);
    shape2.setOutlineThickness(8.f);
    shape2.setSize(traySize);
    shape2.setRadius(20.f);

    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);
    entity->setPosition(trayPosition);
    m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    //create 'buttons' in tray
    auto it = instructionLabels.begin();
    for (auto i = 0u; i < buttonCount; ++i, ++it)
    {
        entity = std::make_unique<xy::Entity>(m_messageBus);
        entity->setPosition(labelPadding + (i * labelSpacing), labelTop);
        entity->addCommandCategories(CommandCategory::TrayIcon);
        auto rr = makeButtonBackground(m_messageBus);
        entity->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);
        auto bls = std::make_unique<ButtonLogicScript>(m_messageBus, it->first);
        entity->addComponent<ButtonLogicScript>(bls);

        auto text = std::make_unique<xy::SfDrawableComponent<sf::Text>>(m_messageBus);
        auto& td = text->getDrawable();
        td.setFont(sc.appInstance.getFont("assets/fonts/Console.ttf"));
        td.setString(it->second);
        td.setColor(sf::Color::Black);
        xy::Util::Position::centreOrigin(td);
        text->setPosition(labelSize / 2.f);
        text->move(0.f, textOffset);

        entity->addComponent<xy::SfDrawableComponent<sf::Text>>(text);
        m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);
    }

    //scroll bar
    rr = std::make_unique<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape3 = rr->getDrawable();
    shape3.setFillColor(fillColour);
    shape3.setOutlineColor(borderColour);
    shape3.setOutlineThickness(3.f);
    shape3.setSize({ scrollBarRadius * 2.f, stackSize.y });
    shape3.setRadius(scrollBarRadius);
    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->setPosition(stackPosition + sf::Vector2f(stackSize.x + 20.f, 0.f));
    entity->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);

    auto cd = std::make_unique<xy::SfDrawableComponent<sf::CircleShape>>(m_messageBus);
    auto& circle = cd->getDrawable();
    circle.setRadius(scrollBarRadius);
    circle.setFillColor(borderColour);
    circle.setOutlineThickness(-1.f);
    circle.setOutlineColor(fillColour);

    subEnt = std::make_unique<xy::Entity>(m_messageBus);
    subEnt->addComponent<xy::SfDrawableComponent<sf::CircleShape>>(cd);

    auto shl = std::make_unique<ScrollHandleLogic>(m_messageBus);
    shl->setLength(stackSize.y - 8.f);
    subEnt->addComponent<ScrollHandleLogic>(shl);

    subEnt->addCommandCategories(CommandCategory::ScrollHandle);
    entity->addChild(subEnt);

    m_scene.addEntity(entity, xy::Scene::Layer::FrontMiddle);


    //add mouse cursor
    auto ad = std::make_unique<xy::SfDrawableComponent<sf::Sprite>>(m_messageBus);
    auto& sprite = ad->getDrawable();
    sprite.setTexture(sc.appInstance.getTexture("assets/images/ui/cursor.png"));

    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<xy::SfDrawableComponent<sf::Sprite>>(ad);
    entity->addCommandCategories(CommandCategory::Cursor);
    entity->setPosition(sc.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(sc.renderWindow)));

    m_mouseCursor = m_scene.addEntity(entity, xy::Scene::Layer::UI);


    //cropping shader
    m_shaderResource.preload(Shader::Id::Crop, Shader::version + Shader::Cropping::vertex,Shader::version + Shader::Cropping::fragment);
    auto& shader = m_shaderResource.get(Shader::Id::Crop);

    auto pos = sc.renderWindow.mapCoordsToPixel(stackPosition, sc.renderWindow.getDefaultView());
    auto size = sc.renderWindow.mapCoordsToPixel(stackSize, sc.renderWindow.getDefaultView());
    //pos.y = sc.renderWindow.getSize().y - pos.y; //because glsl y inversion
    shader.setParameter("u_position", sf::Vector2f(pos));
    shader.setParameter("u_size", sf::Vector2f(size));

    m_shaderResource.preload(Shader::Id::CropText, Shader::version + Shader::Cropping::vertex, Shader::version + Shader::useTexture + Shader::Cropping::fragment);
    auto& textShader = m_shaderResource.get(Shader::Id::CropText);
    textShader.setParameter("u_position", sf::Vector2f(pos));
    textShader.setParameter("u_size", sf::Vector2f(size));
}

//public
void GameUI::update(float dt, const sf::Vector2f& mousePos)
{
    m_mouseCursor->setWorldPosition(mousePos);

    xy::Command dragCommand;
    dragCommand.category = CommandCategory::InstructionBlock;
    dragCommand.action = [this](xy::Entity& entity, float) //TODO if we really have to send this every frame make it a member function
    {
        auto lc = entity.getComponent<InstructionBlockLogic>();
        if (lc->carried())
        {
            auto position = m_mouseCursor->getPosition();
            entity.setWorldPosition(position - lc->getCursorOffset());
            auto msg = m_messageBus.post<InstructionBlockEvent>(MessageId::InstructionBlockMessage);
            msg->component = lc;
            msg->action = InstructionBlockEvent::Moved;
            msg->position = position;
        }
    };
    m_scene.sendCommand(dragCommand);

    //resend to scroll bar (probably make this a member function too)
    dragCommand.category = CommandCategory::ScrollHandle;
    dragCommand.action = [this](xy::Entity& entity, float)
    {
        if (entity.getComponent<ScrollHandleLogic>()->carried())
        {
            auto mousePos = m_mouseCursor->getPosition();
            auto currentPos = entity.getWorldPosition();
        
            entity.setWorldPosition({ currentPos.x, mousePos.y });

            //raise a scrolled message
            auto msg = m_messageBus.post<ScrollbarEvent>(MessageId::ScrollbarMessage);
            msg->position = entity.getComponent<ScrollHandleLogic>()->getPosition();
            //REPORT("Scroll Position", std::to_string(msg->position));
        }
    };
    m_scene.sendCommand(dragCommand);
}

void GameUI::handleEvent(const sf::Event& evt)
{ 
    switch (evt.type)
    {
    case sf::Event::TextEntered:
    case sf::Event::KeyPressed:
    {
        xy::Command cmd;
        cmd.category = CommandCategory::InputPopup;
        cmd.action = [evt](xy::Entity& entity, float)
        {
            entity.getComponent<InputWindow>()->handleTextEvent(evt);
        };
        m_scene.sendCommand(cmd);
    }
        
        break;
    case sf::Event::MouseButtonPressed:
        if (evt.mouseButton.button == sf::Mouse::Left)
        {
            auto mousePos = m_mouseCursor->getPosition();

            //check tray icons and instruction blocks, and pick up if necessary
            xy::Command cmd;
            cmd.category = CommandCategory::TrayIcon | CommandCategory::InstructionBlock | CommandCategory::InputBox;
            cmd.action = [this, mousePos](xy::Entity& ent, float)
            {
                if (ent.getComponent<xy::SfDrawableComponent<RoundedRectangle>>()->globalBounds().contains(mousePos))
                {                    
                    if (ent.hasCommandCategories(CommandCategory::TrayIcon))
                    {
                        ent.getComponent<ButtonLogicScript>()->doClick(mousePos, mousePos - ent.getPosition());
                    }
                    else if(ent.hasCommandCategories(CommandCategory::InstructionBlock))
                    {
                        ent.getComponent<InstructionBlockLogic>()->setCarried(true);
                    }
                    else
                    {
                        auto msg = m_messageBus.post<InputBoxEvent>(MessageId::InputBoxMessage);
                        msg->action = InputBoxEvent::Clicked;
                        msg->entityId = ent.getUID();
                    }
                }
            };
            m_scene.sendCommand(cmd);

            cmd.category = CommandCategory::ScrollHandle;
            cmd.action = [mousePos](xy::Entity& entity, float)
            {
                if (entity.getComponent<xy::SfDrawableComponent<sf::CircleShape>>()->globalBounds().contains(mousePos))
                {
                    entity.getComponent<ScrollHandleLogic>()->setCarried(true);
                }
            };
            m_scene.sendCommand(cmd);

            if (m_inputWindowVisible)
            {
                cmd.category = CommandCategory::InputPopup;
                cmd.action = [this, mousePos](xy::Entity& entity, float)
                {
                    if (!entity.getComponent<InputWindow>()->globalBounds().contains(mousePos))
                    {
                        //TODO submit window value first?
                        entity.destroy();
                        m_inputWindowVisible = false;
                    }
                };
                m_scene.sendCommand(cmd);
            }
        }
        else if (evt.mouseButton.button == sf::Mouse::Right)
        {

        }
        break;
    case sf::Event::MouseButtonReleased:
        if (evt.mouseButton.button == sf::Mouse::Left)
        {
            //drop any carried logic blocks
            xy::Command cmd;
            cmd.category = CommandCategory::InstructionBlock;
            cmd.action = [](xy::Entity& entity, float)
            {
                auto lc = entity.getComponent<InstructionBlockLogic>();
                if(lc->carried()) lc->setCarried(false);
            };
            m_scene.sendCommand(cmd);

            cmd.category = CommandCategory::ScrollHandle;
            cmd.action = [](xy::Entity& entity, float)
            {
                entity.getComponent<ScrollHandleLogic>()->setCarried(false);
            };
            m_scene.sendCommand(cmd);
        }
        else if (evt.mouseButton.button == sf::Mouse::Right)
        {

        }
        break;
    case sf::Event::MouseWheelScrolled:
        {
            auto dist = evt.mouseWheelScroll.delta;
        
            xy::Command cmd;
            cmd.category = CommandCategory::ScrollHandle;
            cmd.action = [this, dist](xy::Entity& entity, float)
            {
                auto currentPos = entity.getWorldPosition();

                entity.setWorldPosition({ currentPos.x, currentPos.y - (10.f * dist) });

                //raise a scrolled message
                auto msg = m_messageBus.post<ScrollbarEvent>(MessageId::ScrollbarMessage);
                msg->position = entity.getComponent<ScrollHandleLogic>()->getPosition();
                REPORT("Scroll Position", std::to_string(msg->position));
            };
            m_scene.sendCommand(cmd);
        }

        break;
    default: break;
    }
}

void GameUI::handleMessage(const xy::Message& msg)
{
    switch (msg.id)
    {
    case MessageId::TrayIconMessage:
    {
        auto msgData = msg.getData<TrayIconEvent>();
        if (msgData.action == TrayIconEvent::Clicked)
            addInstructionBlock({ msgData.absX, msgData.absY }, { msgData.relX, msgData.relY }, msgData.instruction);
    }
    break;
    case MessageId::InputBoxMessage:
    {
        auto msgData = msg.getData<InputBoxEvent>();
        if (msgData.action == InputBoxEvent::Clicked)
        {
            showInputWindow(msgData.entityId);
        }
        else if (msgData.action == InputBoxEvent::WindowClosed)
        {
            auto value = msgData.value;
            xy::Command cmd;
            cmd.entityID = msgData.entityId;
            cmd.action = [value](xy::Entity& entity, float)
            {
                auto& text = entity.getComponent<xy::SfDrawableComponent<sf::Text>>()->getDrawable();
                text.setString(std::to_string(value));
                xy::Util::Position::centreOrigin(text);
            };
            m_scene.sendCommand(cmd);
        }
    }
    break;
    default: break;
    }
}

//private
void GameUI::addInstructionBlock(const sf::Vector2f& position, const sf::Vector2f& offset, Instruction instruction)
{
    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->setPosition(position);
    entity->addCommandCategories(CommandCategory::InstructionBlock);
    auto rr = makeButtonBackground(m_messageBus);
    rr->setShader(&m_shaderResource.get(Shader::Id::Crop));
    rr->setShaderActive(false);
    entity->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);

    auto text = std::make_unique<xy::SfDrawableComponent<sf::Text>>(m_messageBus);
    auto& td = text->getDrawable();
    td.setFont(m_stateContext.appInstance.getFont("assets/fonts/Console.ttf"));
    td.setString(instructionLabels[instruction]);
    td.setColor(sf::Color::Black);
    xy::Util::Position::centreOrigin(td);
    text->setPosition(labelSize / 2.f);
    text->move(0.f, textOffset);
    text->setShader(&m_shaderResource.get(Shader::Id::CropText));
    text->setShaderActive(false);
    entity->addComponent<xy::SfDrawableComponent<sf::Text>>(text);

    auto lc = std::make_unique<InstructionBlockLogic>(m_messageBus, instruction);
    lc->setTarget(position - offset);
    lc->setCursorOffset(offset);
    entity->addComponent<InstructionBlockLogic>(lc);

    if (instruction != Instruction::EngineOff && instruction != Instruction::EngineOn)
    {
        auto subEnt = std::make_unique<xy::Entity>(m_messageBus);
        subEnt->setPosition({ labelSize.x + inputBoxSpacing, 0.f });
        subEnt->addCommandCategories(CommandCategory::InputBox);
        rr = makeInputBackground(m_messageBus);
        rr->setShader(&m_shaderResource.get(Shader::Id::Crop));
        rr->setShaderActive(false);
        subEnt->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);
     
        text = std::make_unique<xy::SfDrawableComponent<sf::Text>>(m_messageBus);
        auto& td = text->getDrawable();
        td.setFont(m_stateContext.appInstance.getFont("assets/fonts/Console.ttf"));
        td.setString("0");
        xy::Util::Position::centreOrigin(td);
        text->setPosition(inputSize / 2.f);
        text->move(0.f, textOffset);
        text->setShader(&m_shaderResource.get(Shader::Id::CropText));
        text->setShaderActive(false);
        subEnt->addComponent<xy::SfDrawableComponent<sf::Text>>(text);
        
        entity->addChild(subEnt);
    }

    if (instruction == Instruction::Loop)
    {
        //add loop wire
        auto& child = entity->getChildren()[0];
        auto loop = std::make_unique<LoopHandle>(m_messageBus, m_stateContext.appInstance.getTexture("assets/images/loop_handle.png"), labelSize.y + 22.f); //TODO get the padding value from stack
        //loop->setEnabled(true);
        loop->setPosition(-(labelSize.x + inputBoxSpacing), 0.f);
        child->addComponent<LoopHandle>(loop);
    }

    m_instructionStack->addChild(entity);
}

void GameUI::showInputWindow(sf::Uint64 destId)
{
    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->setWorldPosition({ 960.f, 540.f });
    entity->addCommandCategories(CommandCategory::InputPopup);
    auto inputWindow = std::make_unique<InputWindow>(m_messageBus);
    inputWindow->setTargetId(destId);
    inputWindow->setFont(m_stateContext.appInstance.getFont("assets/fonts/Console.ttf"));
    inputWindow->setCharacterSize(80u);
    entity->addComponent<InputWindow>(inputWindow);
    m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    m_inputWindowVisible = true;
}