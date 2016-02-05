//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <GameState.hpp>
#include <NetProtocol.hpp>
#include <components/Tilemap.hpp>
#include <components/PlayerDrawable.hpp>
#include <components/NetworkController.hpp>

#include <xygine/Reports.hpp>
#include <xygine/Entity.hpp>
#include <xygine/Command.hpp>
#include <xygine/PostChromeAb.hpp>
#include <xygine/ui/Label.hpp>

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

    xy::UI::Label::Ptr reportText;
}

using namespace std::placeholders;

GameState::GameState(xy::StateStack& stateStack, Context context)
    : State         (stateStack, context),
    m_messageBus    (context.appInstance.getMessageBus()),
    m_scene         (m_messageBus),
    m_gameUI        (context, m_textureResource, m_fontResource, m_scene),
    m_reportWindow  (context.renderWindow, m_fontResource.get(""), 500, 400)
{
    launchLoadingScreen();

    //TODO handle failure to connect
    m_packetHandler = std::bind(&GameState::handlePacket, this, _1, _2, _3);
    m_connection.setPacketHandler(m_packetHandler);
    m_connection.setServerInfo({ "127.0.0.1" }, Network::ServerPort);
    m_connection.connect();

    m_scene.setView(context.defaultView);
    //m_scene.drawDebug(true);
    auto pp = xy::PostProcess::create<xy::PostChromeAb>();
    m_scene.addPostProcess(pp);

    reportText = std::make_shared<xy::UI::Label>(m_fontResource.get("assets/fonts/Console.ttf"));
    reportText->move(10.f, 0.f);
    m_reportWindow.addControl(reportText);
    m_reportWindow.setPosition(1500.f, 30.f);

    buildMap();

    quitLoadingScreen();
}

bool GameState::update(float dt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
    
    m_gameUI.update(dt, mousePos);
    m_scene.update(dt);
    m_connection.update(dt);

    reportText->setString(xy::StatsReporter::reporter.getString());
    m_reportWindow.update(dt);

    return true;
}

void GameState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.draw(m_scene);
    rw.setView(getContext().defaultView);
    rw.draw(m_reportWindow);
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
    m_reportWindow.handleEvent(evt, xy::App::getMouseWorldPosition());
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

    //player - TODO move to 'create player' function
    auto playerDrawable = xy::Component::create<PlayerDrawable>(m_messageBus, m_textureResource.get("assets/images/tileset.png"), true);
    auto playerEnt = xy::Entity::create(m_messageBus);
    playerEnt->addComponent(playerDrawable);
    playerEnt->setPosition(224.f, 160.f);

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

    auto netController = xy::Component::create<NetworkController>(m_messageBus);
    playerEnt->addComponent(netController);

    //TODO add text for player name

    m_playerEntities[m_connection.getClientID()] = ent->addChild(playerEnt);
    
    m_scene.addEntity(ent, xy::Scene::Layer::BackRear);
}

void GameState::handlePacket(Network::PacketType type, sf::Packet& packet, Network::ClientConnection* connection)
{
    switch (type)
    {
    case Network::Connect:
    {
        sf::Packet newPacket;
        newPacket << PacketID(PacketIdent::PlayerDetails);
        newPacket << m_connection.getClientID();
        newPacket << "Player One";
        connection->send(newPacket); //TODO make sure this gets through
    }
        break;
    case PacketIdent::PositionUpdate:
        sf::Uint8 count;
        packet >> count;
        for (auto i = 0u; i < count; ++i)
        {
            ClientID id;
            packet >> id;
            sf::Vector2f position;
            packet >> position.x >> position.y;

            XY_ASSERT(m_playerEntities.find(id) != m_playerEntities.end(), "Player ID does not exist");
            m_playerEntities[id]->getComponent<NetworkController>()->setDestination(position);
        }
        break;
    case PacketIdent::DirectionUpdate:
    {
        ClientID id;
        sf::Uint8 dir;
        packet >> id >> dir;

        XY_ASSERT(m_playerEntities.find(id) != m_playerEntities.end(), "Player ID does not exist");
        Direction direction = static_cast<Direction>(dir);
        m_playerEntities[id]->getComponent<PlayerDrawable>()->setDirection(direction);

        auto particles = m_playerEntities[id]->getComponents<xy::ParticleSystem>();
        for (auto& ps : particles) ps->stop();
        switch (direction)
        {
        default: break;
        case Direction::Up:
            m_playerEntities[id]->getComponent<xy::ParticleSystem>("particle_up")->start();
            break;
        case Direction::Down:
            m_playerEntities[id]->getComponent<xy::ParticleSystem>("particle_down")->start();
            break;
        case Direction::Left:
            m_playerEntities[id]->getComponent<xy::ParticleSystem>("particle_left")->start();
            break;
        case Direction::Right:
            m_playerEntities[id]->getComponent<xy::ParticleSystem>("particle_right")->start();
            break;
        }
    }
        break;
    default: break;
    }
}