/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#include <GameUI.hpp>

#include <xygine/Scene.hpp>
#include <xygine/Entity.hpp>
#include <xygine/MessageBus.hpp>
#include <xygine/util/Position.hpp>
#include <xygine/components/AnimatedDrawable.hpp>
#include <xygine/App.hpp>
#include <xygine/Reports.hpp>
#include <xygine/components/SfDrawableComponent.hpp>
#include <xygine/Resource.hpp>

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

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>

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
    const sf::Uint8 buttonCount = 5u;// 6u; //temp disable looping until we fix UI

    const float labelSpacing = 240.f;
    const float labelPadding = 445.f;
    const float labelTop = 980.f;

    const sf::Vector2f labelSize(220.f, 30.f);
    const sf::Vector2f inputSize(50.f, 30.f);

    const sf::Vector2f stackPosition(50.f, 40.f);
    const sf::Vector2f stackSize(320.f, 990.f);

    const sf::Vector2f trayPosition(430.f, 960.f);
    const sf::Vector2f traySize(1450.f, 70.f);

    const sf::Vector2f transportPosition(1754.f, 40.f);
    const sf::Vector2f transportSize(124.f, 890.f);

    const sf::Color fillColour(180u, 40u, 20u, 180u);
    const sf::Color borderColour(15u, 30u, 100u);

    const sf::Color buttonFillColour(220u, 240u, 10u, 180u);
    const sf::Color buttonBorderColor(10u, 230u, 10u);

    const float scrollBarRadius = 8.f;

    const float inputBoxSpacing = 26.f;

    std::unique_ptr<xy::SfDrawableComponent<RoundedRectangle>> makeButtonBackground(xy::MessageBus& messageBus)
    {
        auto rr = xy::Component::create<xy::SfDrawableComponent<RoundedRectangle>>(messageBus);
        auto& shape = rr->getDrawable();
        shape.setFillColor(buttonFillColour);
        shape.setOutlineThickness(6.f);
        shape.setOutlineColor(buttonBorderColor);
        shape.setSize(labelSize);
        return std::move(rr);
    }

    std::unique_ptr<xy::SfDrawableComponent<RoundedRectangle>> makeInputBackground(xy::MessageBus& messageBus)
    {
        auto rr = xy::Component::create<xy::SfDrawableComponent<RoundedRectangle>>(messageBus);
        auto& shape = rr->getDrawable();
        shape.setFillColor({ 220u, 40u, 210u, 180u });
        shape.setOutlineThickness(6.f);
        shape.setOutlineColor({ 10u, 230u, 10u });
        shape.setSize(inputSize);
        return std::move(rr);
    }

    std::unique_ptr<xy::SfDrawableComponent<sf::CircleShape>> makeTransportButton(xy::MessageBus& messageBus)
    {
        auto rr = xy::Component::create<xy::SfDrawableComponent<sf::CircleShape>>(messageBus);
        auto& shape = rr->getDrawable();
        //shape.setFillColor(buttonFillColour);
        shape.setOutlineThickness(6.f);
        shape.setOutlineColor(buttonBorderColor);
        shape.setRadius(40.f);
        shape.setOrigin(40.f, 40.f);
        return std::move(rr);
    }
}

GameUI::GameUI(xy::State::Context sc, xy::TextureResource& tr, xy::FontResource& fr, xy::Scene& scene)
    : m_fontResource    (fr),
    m_textureResource   (tr),
    m_transportStatus   (TransportStatus::Stopped),
    m_stateContext      (sc),
    m_scene             (scene),
    m_messageBus        (sc.appInstance.getMessageBus()),
    m_mouseCursor       (nullptr),
    m_instructionStack  (nullptr),
    m_inputWindowVisible(false)
{
    //command list
    auto rr = xy::Component::create<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape = rr->getDrawable();
    shape.setFillColor(fillColour);
    shape.setOutlineThickness(10.f);
    shape.setOutlineColor(borderColour);
    shape.setSize(stackSize);

    auto entity = xy::Entity::create(m_messageBus);
    entity->addComponent(rr);
    entity->setPosition(stackPosition);
    auto backPanel = m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    //the logic is separated from the background so we can scroll it
    auto subEnt = xy::Entity::create(m_messageBus);
    subEnt->addCommandCategories(CommandCategory::InstructionList);
    auto scl = std::make_unique<StackLogicComponent>(m_messageBus, labelSize, stackSize);
    subEnt->addComponent(scl);
    m_instructionStack = subEnt.get();
    backPanel->addChild(subEnt); //backPanel has to be added first so it has a valid scene pointer

    //command tray
    rr = xy::Component::create<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape2 = rr->getDrawable();
    shape2.setFillColor(fillColour);
    shape2.setOutlineColor(borderColour);
    shape2.setOutlineThickness(8.f);
    shape2.setSize(traySize);
    shape2.setRadius(20.f);

    entity = xy::Entity::create(m_messageBus);
    entity->addComponent(rr);
    entity->setPosition(trayPosition);
    m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    //create 'buttons' in tray
    auto it = instructionLabels.begin();
    for (auto i = 0u; i < buttonCount; ++i, ++it)
    {
        entity = xy::Entity::create(m_messageBus);
        entity->setPosition(labelPadding + (i * labelSpacing), labelTop);
        entity->addCommandCategories(CommandCategory::TrayIcon);
        auto rr = makeButtonBackground(m_messageBus);
        entity->addComponent(rr);
        auto bls = std::make_unique<ButtonLogicScript>(m_messageBus, it->first);
        entity->addComponent(bls);

        auto text = std::make_unique<xy::SfDrawableComponent<sf::Text>>(m_messageBus);
        auto& td = text->getDrawable();
        td.setFont(fr.get("assets/fonts/Console.ttf"));
        td.setString(it->second);
        td.setFillColor(sf::Color::Black);
        xy::Util::Position::centreOrigin(td);
        text->setPosition(labelSize / 2.f);
        //text->move(0.f, textOffset);

        entity->addComponent(text);
        m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);
    }

    //scroll bar
    rr = xy::Component::create<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape3 = rr->getDrawable();
    shape3.setFillColor(fillColour);
    shape3.setOutlineColor(borderColour);
    shape3.setOutlineThickness(3.f);
    shape3.setSize({ scrollBarRadius * 2.f, stackSize.y });
    shape3.setRadius(scrollBarRadius);
    entity = xy::Entity::create(m_messageBus);
    entity->setPosition(stackPosition + sf::Vector2f(stackSize.x + 20.f, 0.f));
    entity->addComponent(rr);

    auto cd = xy::Component::create<xy::SfDrawableComponent<sf::CircleShape>>(m_messageBus);
    auto& circle = cd->getDrawable();
    circle.setRadius(scrollBarRadius);
    circle.setFillColor(borderColour);
    circle.setOutlineThickness(-1.f);
    circle.setOutlineColor(fillColour);

    subEnt = xy::Entity::create(m_messageBus);
    subEnt->addComponent(cd);

    auto shl = xy::Component::create<ScrollHandleLogic>(m_messageBus);
    shl->setLength(stackSize.y - 8.f);
    subEnt->addComponent(shl);

    subEnt->addCommandCategories(CommandCategory::ScrollHandle);
    entity->addChild(subEnt);

    m_scene.addEntity(entity, xy::Scene::Layer::FrontMiddle);


    //add mouse cursor
    auto ad = xy::Component::create<xy::SfDrawableComponent<sf::Sprite>>(m_messageBus);
    auto& sprite = ad->getDrawable();
    sprite.setTexture(tr.get("assets/images/ui/cursor.png"));

    entity = xy::Entity::create(m_messageBus);
    entity->addComponent(ad);
    entity->addCommandCategories(CommandCategory::Cursor);
    entity->setPosition(sc.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(sc.renderWindow)));

    m_mouseCursor = m_scene.addEntity(entity, xy::Scene::Layer::UI);


    //cropping shader
    m_shaderResource.preload(Shader::Id::Crop, Shader::version + Shader::Cropping::vertex,Shader::version + Shader::Cropping::fragment);
    auto& shader = m_shaderResource.get(Shader::Id::Crop);

    auto pos = sc.renderWindow.mapCoordsToPixel(stackPosition, sc.renderWindow.getDefaultView());
    auto size = sc.renderWindow.mapCoordsToPixel(stackSize, sc.renderWindow.getDefaultView());
    //pos.y = sc.renderWindow.getSize().y - pos.y; //because glsl y inversion
    shader.setUniform("u_position", sf::Vector2f(pos));
    shader.setUniform("u_size", sf::Vector2f(size));

    m_shaderResource.preload(Shader::Id::CropText, Shader::version + Shader::Cropping::vertex, Shader::version + Shader::useTexture + Shader::Cropping::fragment);
    auto& textShader = m_shaderResource.get(Shader::Id::CropText);
    textShader.setUniform("u_position", sf::Vector2f(pos));
    textShader.setUniform("u_size", sf::Vector2f(size));


    //transport controls
    rr = xy::Component::create<xy::SfDrawableComponent<RoundedRectangle>>(m_messageBus);
    auto& shape4 = rr->getDrawable();
    shape4.setFillColor(fillColour);
    shape4.setOutlineColor(borderColour);
    shape4.setOutlineThickness(8.f);
    shape4.setSize(transportSize);
    
    entity = xy::Entity::create(m_messageBus);
    entity->setPosition(transportPosition);
    entity->addCommandCategories(CommandCategory::TransportControl);
    entity->addComponent(rr);

    auto texture = &tr.get("assets/images/transport.png");

    auto playButton = makeTransportButton(m_messageBus);
    playButton->setName("play_button");
    playButton->getDrawable().setPosition(transportSize / 2.f);
    playButton->getDrawable().move(0.f, -120.f);
    playButton->getDrawable().setTexture(texture);
    playButton->getDrawable().setTextureRect({ 0, 0, 80, 80 });
    entity->addComponent(playButton);

    auto pauseButton = makeTransportButton(m_messageBus);
    pauseButton->setName("pause_button");
    pauseButton->getDrawable().setPosition(transportSize / 2.f);
    pauseButton->getDrawable().setTexture(texture);
    pauseButton->getDrawable().setTextureRect({ 0, 80, 80, 80 });
    entity->addComponent(pauseButton);

    auto rewindButton = makeTransportButton(m_messageBus);
    rewindButton->setName("rewind_button");
    rewindButton->getDrawable().setPosition(transportSize / 2.f);
    rewindButton->getDrawable().move(0.f, 120.f);
    rewindButton->getDrawable().setTexture(texture);
    rewindButton->getDrawable().setTextureRect({ 0, 160, 80, 80 });
    entity->addComponent(rewindButton);

    scene.addEntity(entity, xy::Scene::Layer::FrontFront);
    REPORT("Transport Status", "Stopped");
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
                auto bounds = ent.getWorldTransform().transformRect(ent.getComponent<xy::SfDrawableComponent<RoundedRectangle>>()->globalBounds());
                if (bounds.contains(mousePos))
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


            //check if we're over a transport button and raise a clicked event
            auto mousePos = m_mouseCursor->getPosition();
            cmd.category = CommandCategory::TransportControl;
            cmd.action = [mousePos, this](xy::Entity& entity, float dt)
            {
                auto position = entity.getWorldPosition();
                auto buttons = entity.getComponents<xy::SfDrawableComponent<sf::CircleShape>>();
                for (auto b : buttons)
                {
                    auto bounds = b->getDrawable().getGlobalBounds();
                    bounds.left += position.x;
                    bounds.top += position.y;
                    if (bounds.contains(mousePos))
                    {
                        auto msg = m_messageBus.post<TransportEvent>(MessageId::TransportMessage);
                        if (b->getName() == "play_button")
                        {
                            msg->button = TransportEvent::Play;
                        }
                        else if (b->getName() == "pause_button")
                        {
                            msg->button = TransportEvent::Pause;
                        }
                        else
                        {
                            msg->button = TransportEvent::Rewind;
                        }
                        break;
                    }
                }
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
        const auto& msgData = msg.getData<TrayIconEvent>();
        if (msgData.action == TrayIconEvent::Clicked)
            addInstructionBlock({ msgData.absX, msgData.absY }, { msgData.relX, msgData.relY }, msgData.instruction);
    }
    break;
    case MessageId::InputBoxMessage:
    {
        const auto& msgData = msg.getData<InputBoxEvent>();
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

std::vector<sf::Uint8> GameUI::getProgram() const
{
    std::vector<sf::Uint8> retVal;
    const auto& entities = m_instructionStack->getChildren();
    for (const auto& i : entities)
    {
        auto* logic = i->getComponent<InstructionBlockLogic>();
        retVal.push_back(sf::Uint8(logic->getInstruction()));
        retVal.push_back(logic->getValue());
    }

    return std::move(retVal);
}

void GameUI::setTransportStatus(TransportStatus ts)
{
    m_transportStatus = ts;

    static std::string target;
    switch (ts)
    {
    default: break;
    case TransportStatus::Playing:
        target = "play_button";
        REPORT("Transport Status", "Playing");
        break;
    case TransportStatus::Paused:
        target = "pause_button";
        REPORT("Transport Status", "Paused");
        break;
    case TransportStatus::Stopped:
        target = "";
        REPORT("Transport Status", "Rewind");
        break;
    }
    

    //highlight corresponding transport button
    xy::Command cmd;
    cmd.action = [](xy::Entity& entity, float)
    {
        auto buttons = entity.getComponents<xy::SfDrawableComponent<sf::CircleShape>>();
        for(auto button : buttons)
        {
            auto subRect = button->getDrawable().getTextureRect();
            if (button->getName() == target)
            {
                subRect.left = 80;
            }
            else
            {
                subRect.left = 0;
            }
            button->getDrawable().setTextureRect(subRect);
        }
    };

    cmd.category = CommandCategory::TransportControl;
    m_scene.sendCommand(cmd);
}

//private
void GameUI::addInstructionBlock(const sf::Vector2f& position, const sf::Vector2f& offset, Instruction instruction)
{
    auto entity = xy::Entity::create(m_messageBus);
    entity->setPosition(position);
    entity->addCommandCategories(CommandCategory::InstructionBlock);
    auto rr = makeButtonBackground(m_messageBus);
    rr->setShader(&m_shaderResource.get(Shader::Id::Crop));
    rr->setShaderActive(false);
    entity->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);

    auto text = std::make_unique<xy::SfDrawableComponent<sf::Text>>(m_messageBus);
    auto& td = text->getDrawable();
    td.setFont(m_fontResource.get("assets/fonts/Console.ttf"));
    td.setString(instructionLabels[instruction]);
    td.setFillColor(sf::Color::Black);
    xy::Util::Position::centreOrigin(td);
    text->setPosition(labelSize / 2.f);
    //text->move(0.f, textOffset);
    text->setShader(&m_shaderResource.get(Shader::Id::CropText));
    text->setShaderActive(false);
    entity->addComponent<xy::SfDrawableComponent<sf::Text>>(text);

    auto lc = std::make_unique<InstructionBlockLogic>(m_messageBus, instruction);
    lc->setTarget(position - offset);
    lc->setCursorOffset(offset);
    entity->addComponent<InstructionBlockLogic>(lc);

    if (instruction != Instruction::EngineOff && instruction != Instruction::EngineOn)
    {
        auto subEnt = xy::Entity::create(m_messageBus);
        subEnt->setPosition({ labelSize.x + inputBoxSpacing, 0.f });
        subEnt->addCommandCategories(CommandCategory::InputBox);
        rr = makeInputBackground(m_messageBus);
        rr->setShader(&m_shaderResource.get(Shader::Id::Crop));
        rr->setShaderActive(false);
        subEnt->addComponent<xy::SfDrawableComponent<RoundedRectangle>>(rr);
     
        text = std::make_unique<xy::SfDrawableComponent<sf::Text>>(m_messageBus);
        auto& td = text->getDrawable();
        td.setFont(m_fontResource.get("assets/fonts/Console.ttf"));
        td.setString("0");
        xy::Util::Position::centreOrigin(td);
        text->setPosition(inputSize / 2.f);
        //text->move(0.f, textOffset);
        text->setShader(&m_shaderResource.get(Shader::Id::CropText));
        text->setShaderActive(false);
        subEnt->addComponent<xy::SfDrawableComponent<sf::Text>>(text);
        
        entity->addChild(subEnt);
    }

    if (instruction == Instruction::Loop)
    {
        //add loop wire
        auto& child = entity->getChildren()[0];
        auto loop = std::make_unique<LoopHandle>(m_messageBus, m_textureResource.get("assets/images/loop_handle.png"), labelSize.y + 22.f); //TODO get the padding value from stack
        //loop->setEnabled(true);
        loop->setPosition(-(labelSize.x + inputBoxSpacing), 0.f);
        child->addComponent<LoopHandle>(loop);
    }

    m_instructionStack->addChild(entity);
}

void GameUI::showInputWindow(sf::Uint64 destId)
{
    auto entity = xy::Entity::create(m_messageBus);
    entity->setWorldPosition({ 960.f, 540.f });
    entity->addCommandCategories(CommandCategory::InputPopup);
    auto inputWindow = std::make_unique<InputWindow>(m_messageBus);
    inputWindow->setTargetId(destId);
    inputWindow->setFont(m_fontResource.get("assets/fonts/Console.ttf"));
    inputWindow->setCharacterSize(80u);
    entity->addComponent<InputWindow>(inputWindow);
    m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);

    m_inputWindowVisible = true;
}