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

#include <components/RoundedRectangle.hpp>
#include <components/ButtonLogic.hpp>
#include <components/InstructionBlockLogic.hpp>
#include <components/StackLogicComponent.hpp>
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
    const float labelPadding = 435.f;
    const float labelTop = 960.f;
    const float textOffset = -6.f;

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

GameUI::GameUI(xy::State::Context sc, xy::Scene& scene)
    : m_stateContext(sc),
    m_scene         (scene),
    m_messageBus    (sc.appInstance.getMessageBus()),
    m_mouseCursor   (nullptr)
{
    //command list
    auto rr = std::make_unique<RoundedRectangle>(m_messageBus, sf::Vector2f(320.f, 980.f));
    rr->setFillColor({ 20u, 40u, 180u, 180u });
    rr->setOutlineThickness(10.f);
    rr->setOutlineColor({ 15u, 30u, 100u });

    auto entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<RoundedRectangle>(rr);
    entity->setPosition(50.f, 50.f);

    entity->addComponent<StackLogicComponent>(std::make_unique<StackLogicComponent>(m_messageBus, labelSize));

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
        text->setFont(sc.appInstance.getFont("flaps"));
        text->setString(it->second);
        text->setColor(sf::Color::Black);
        xy::Util::Position::centreOrigin(*text);
        text->setPosition(labelSize / 2.f);
        text->move(0.f, textOffset);

        entity->addComponent<xy::TextDrawable>(text);

        m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);
    }

    //add mouse cursor
    auto ad = std::make_unique<xy::AnimatedDrawable>(m_messageBus, sc.appInstance.getTexture("assets/images/ui/cursor.png"));
    entity = std::make_unique<xy::Entity>(m_messageBus);
    entity->addComponent<xy::AnimatedDrawable>(ad);
    entity->addCommandCategories(CommandCategory::Cursor);
    entity->setPosition(sc.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(sc.renderWindow)));
    m_mouseCursor = entity.get();
    m_scene.addEntity(entity, xy::Scene::Layer::UI);
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
            entity.setPosition(position - lc->getCursorOffset());
            auto msg = m_messageBus.post<InstructionBlockEvent>(MessageId::InstructionBlockMessage);
            msg->component = lc;
            msg->action = InstructionBlockEvent::Moved;
            msg->position = position;
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
        }
        else if (evt.mouseButton.button == sf::Mouse::Right)
        {

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

    entity->addComponent<RoundedRectangle>(makeButtonBackground(m_messageBus));

    auto text = std::make_unique<xy::TextDrawable>(m_messageBus);
    text->setFont(m_stateContext.appInstance.getFont("flaps"));
    text->setString(instructionLabels[instruction]);
    text->setColor(sf::Color::Black);
    xy::Util::Position::centreOrigin(*text);
    text->setPosition(labelSize / 2.f);
    text->move(0.f, textOffset);

    entity->addComponent<xy::TextDrawable>(text);

    auto lc = std::make_unique<InstructionBlockLogic>(m_messageBus, instruction);
    lc->setTarget(position - offset);
    lc->setCursorOffset(offset);
    entity->addComponent<InstructionBlockLogic>(lc);

    m_scene.getLayer(xy::Scene::Layer::FrontFront).addChild(entity);
}