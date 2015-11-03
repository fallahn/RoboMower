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
#include <xygine/TextDrawable.hpp>
#include <xygine/Util.hpp>
#include <xygine/AnimatedDrawable.hpp>
#include <xygine/App.hpp>
#include <xygine/Reports.hpp>

#include <components/RoundedRectangle.hpp>
#include <components/ButtonLogic.hpp>
#include <components/InstructionBlockLogic.hpp>
#include <components/StackLogicComponent.hpp>
#include <components/CircleDrawable.hpp>
#include <components/ScrollHandleLogic.hpp>
#include <CommandCategories.hpp>
#include <Messages.hpp>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

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

    const sf::Color fillColour(180u, 40u, 20u, 180u);
    const sf::Color borderColour(15u, 30u, 100u);

    std::unique_ptr<RoundedRectangle> makeButtonBackground(xy::MessageBus& messageBus)
    {
        auto rr = std::make_unique<RoundedRectangle>(messageBus, labelSize);
        rr->setFillColor({ 220u, 240u, 10u, 180u });
        rr->setOutlineThickness(6.f);
        rr->setOutlineColor({ 10u, 230u, 10u });
        return std::move(rr);
    }

    std::unique_ptr<RoundedRectangle> makeInputBackground(xy::MessageBus& messageBus)
    {
        auto rr = std::make_unique<RoundedRectangle>(messageBus, inputSize);
        rr->setFillColor({ 220u, 40u, 210u, 180u });
        rr->setOutlineThickness(6.f);
        rr->setOutlineColor({ 10u, 230u, 10u });
        return std::move(rr);
    }
}

GameUI::GameUI(xy::State::Context sc, xy::Scene& scene)
    : m_stateContext    (sc),
    m_scene             (scene),
    m_messageBus        (sc.appInstance.getMessageBus()),
    m_mouseCursor       (nullptr),
    m_instructionStack  (nullptr)
{
    //command list
    auto rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(stackSize));
    rr->setFillColor(fillColour);
    rr->setOutlineThickness(10.f);
    rr->setOutlineColor(borderColour);

    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<RoundedRectangle>(rr);
    entity->setPosition(stackPosition);
    auto backPanel = m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    //the logic is separated from the background so we can scroll it
    auto subEnt = std::make_unique<xy::Entity>(m_messageBus);
    subEnt->addCommandCategories(CommandCategory::InstructionList);
    auto scl = std::make_unique<StackLogicComponent>(m_messageBus, labelSize, sf::Vector2f(stackSize.x, stackSize.y * 2));
    subEnt->addComponent<StackLogicComponent>(scl);
    m_instructionStack = subEnt.get();
    backPanel->addChild(subEnt); //backPanel has to be added first so it has a valid scene pointer

    //command tray
    rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(1450.f, 70.f), 20.f);
    rr->setFillColor(fillColour);
    rr->setOutlineColor(borderColour);
    rr->setOutlineThickness(8.f);

    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<RoundedRectangle>(rr);
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
        entity->addComponent<RoundedRectangle>(rr);
        auto bls = std::make_unique<ButtonLogicScript>(m_messageBus, it->first);
        entity->addComponent<ButtonLogicScript>(bls);

        auto text = std::make_unique<xy::TextDrawable>(m_messageBus);
        text->setFont(sc.appInstance.getFont("assets/fonts/Console.ttf"));
        text->setString(it->second);
        text->setColour(sf::Color::Black);
        xy::Util::Position::centreOrigin(*text);
        text->setPosition(labelSize / 2.f);
        text->move(0.f, textOffset);

        entity->addComponent<xy::TextDrawable>(text);
        m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);
    }

    //scroll bar
    rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(16.f, stackSize.y), 8.f); //TODO make radius a const
    rr->setFillColor(fillColour);
    rr->setOutlineColor(borderColour);
    rr->setOutlineThickness(3.f);
    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->setPosition(stackPosition + sf::Vector2f(stackSize.x + 20.f, 0.f));
    entity->addComponent<RoundedRectangle>(rr);

    auto cd = std::make_unique<CircleDrawable>(m_messageBus);
    cd->setRadius(8.f);
    cd->setFillColor(borderColour);
    cd->setOutlineThickness(-1.f);
    cd->setOutlineColor(fillColour);

    subEnt = std::make_unique<xy::Entity>(m_messageBus);
    subEnt->addComponent<CircleDrawable>(cd);

    auto shl = std::make_unique<ScrollHandleLogic>(m_messageBus);
    shl->setLength(stackSize.y - 8.f);
    subEnt->addComponent<ScrollHandleLogic>(shl);

    subEnt->addCommandCategories(CommandCategory::ScrollHandle);
    entity->addChild(subEnt);

    m_scene.addEntity(entity, xy::Scene::Layer::FrontMiddle);


    //add mouse cursor
    auto ad = std::make_unique<xy::AnimatedDrawable>(m_messageBus, sc.appInstance.getTexture("assets/images/ui/cursor.png"));
    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<xy::AnimatedDrawable>(ad);
    entity->addCommandCategories(CommandCategory::Cursor);
    entity->setPosition(sc.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(sc.renderWindow)));
    m_mouseCursor = m_scene.addEntity(entity, xy::Scene::Layer::UI);
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
            REPORT("Scroll Position", std::to_string(msg->position));
        }
    };
    m_scene.sendCommand(dragCommand);
}

void GameUI::handleEvent(const sf::Event& evt)
{
    switch (evt.type)
    {
    case sf::Event::MouseButtonPressed:
        if (evt.mouseButton.button == sf::Mouse::Left)
        {
            auto mousePos = m_mouseCursor->getPosition();

            //check tray icons and instruction blocks, and pick up if necessary
            xy::Command cmd;
            cmd.category = CommandCategory::TrayIcon | CommandCategory::InstructionBlock;
            cmd.action = [mousePos](xy::Entity& ent, float)
            {
                if (ent.getComponent<RoundedRectangle>()->globalBounds().contains(mousePos))
                {
                    if (ent.hasCommandCategories(CommandCategory::TrayIcon))
                    {
                        ent.getComponent<ButtonLogicScript>()->doClick(mousePos, mousePos - ent.getPosition());
                    }
                    else
                    {
                        ent.getComponent<InstructionBlockLogic>()->setCarried(true);
                    }
                }
            };
            m_scene.sendCommand(cmd);

            cmd.category = CommandCategory::ScrollHandle;
            cmd.action = [mousePos](xy::Entity& entity, float)
            {
                if (entity.getComponent<CircleDrawable>()->globalBounds().contains(mousePos))
                {
                    entity.getComponent<ScrollHandleLogic>()->setCarried(true);
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
            //drop any carried logic blocks
            xy::Command cmd;
            cmd.category = CommandCategory::InstructionBlock;
            cmd.action = [](xy::Entity& entity, float)
            {
                auto lc = entity.getComponent<InstructionBlockLogic>();
                lc->setCarried(false);
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
    entity->addComponent<RoundedRectangle>(rr);

    auto text = std::make_unique<xy::TextDrawable>(m_messageBus);
    text->setFont(m_stateContext.appInstance.getFont("assets/fonts/Console.ttf"));
    text->setString(instructionLabels[instruction]);
    text->setColour(sf::Color::Black);
    xy::Util::Position::centreOrigin(*text);
    text->setPosition(labelSize / 2.f);
    text->move(0.f, textOffset);

    entity->addComponent<xy::TextDrawable>(text);

    auto lc = std::make_unique<InstructionBlockLogic>(m_messageBus, instruction);
    lc->setTarget(position - offset);
    lc->setCursorOffset(offset);
    entity->addComponent<InstructionBlockLogic>(lc);

    if (instruction != Instruction::EngineOff && instruction != Instruction::EngineOn)
    {
        auto subEnt = std::make_unique<xy::Entity>(m_messageBus);
        subEnt->setPosition({ labelSize.x + 26.f, 0.f });
        auto rr = makeInputBackground(m_messageBus);
        subEnt->addComponent<RoundedRectangle>(rr);
     
        //TODO add logic
        
        entity->addChild(subEnt);
    }

    if (instruction == Instruction::Loop)
    {
        //TODO add loop wire
    }

    //m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);
    m_instructionStack->addChild(entity);
}
