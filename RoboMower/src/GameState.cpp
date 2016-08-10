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

#include <GameState.hpp>
#include <NetProtocol.hpp>
#include <Messages.hpp>
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
}

using namespace std::placeholders;

GameState::GameState(xy::StateStack& stateStack, Context context)
    : State             (stateStack, context),
    m_messageBus        (context.appInstance.getMessageBus()),
    m_scene             (m_messageBus),
    m_gameUI            (context, m_textureResource, m_fontResource, m_scene),
    m_programFinished   (true)
{
    launchLoadingScreen();

    //TODO handle failure to connect
    m_packetHandler = std::bind(&GameState::handlePacket, this, _1, _2, _3);
    m_connection.setPacketHandler(m_packetHandler);
    m_connection.setServerInfo({ "127.0.0.1" }, xy::Network::ServerPort);
    m_connection.connect();

    m_scene.setView(context.defaultView);
    auto pp = xy::PostProcess::create<xy::PostChromeAb>();
    m_scene.addPostProcess(pp);

    buildMap();

    context.appInstance.setMouseCursorVisible(false);
    quitLoadingScreen();
}

bool GameState::update(float dt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
    
    m_gameUI.update(dt, mousePos);
    m_scene.update(dt);
    m_connection.update(dt);

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
    switch (msg.id)
    {
    case MessageId::TransportMessage:
    {
        const auto& msgData = msg.getData<TransportEvent>();
        switch (msgData.button)
        {
        case TransportEvent::Pause:
            if (m_gameUI.getTransportStatus() == TransportStatus::Playing)
            {
                sf::Packet packet;
                packet << PacketIdent::TransportRequestChange << m_connection.getClientID() << TransportChange::Pause;
                m_connection.send(packet, true);
            }
            break;
        case TransportEvent::Play:
            if (m_gameUI.getTransportStatus() == TransportStatus::Stopped && m_programFinished)
            {
                //get the program and send if valid
                auto program = m_gameUI.getProgram();
                if (!program.empty())
                {
                    sf::Packet packet;
                    packet << PacketIdent::TransmitProgram;
                    packet << m_connection.getClientID();
                    packet << sf::Uint32(program.size());
                    for (auto data : program)
                    {
                        //LOG(std::to_string(data), xy::Logger::Type::Info);
                        packet << data;
                    }
                    m_connection.send(packet, true);
                }
            }
            else if(m_gameUI.getTransportStatus() == TransportStatus::Paused)
            {
                //send a play request
                sf::Packet packet;
                packet << PacketIdent::TransportRequestChange << m_connection.getClientID() << TransportChange::Play;
                m_connection.send(packet, true);
            }
                break;
        case TransportEvent::Rewind:
            if (m_gameUI.getTransportStatus() != TransportStatus::Playing)
            {
                sf::Packet packet;
                packet << PacketIdent::TransportRequestChange << m_connection.getClientID() << TransportChange::Rewind;
                m_connection.send(packet, true);
            }
            break;
        }
    }
    break;
    }
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

void GameState::handlePacket(xy::Network::PacketType type, sf::Packet& packet, xy::Network::ClientConnection* connection)
{
    switch (type)
    {
    case xy::Network::Connect:
    {
        sf::Packet newPacket;
        newPacket << PacketIdent::PlayerDetails;
        newPacket << m_connection.getClientID();
        newPacket << "Player One";
        connection->send(newPacket, true);
    }
        break;
    case PacketIdent::PositionUpdate:
        sf::Uint8 count;
        packet >> count;
        for (auto i = 0u; i < count; ++i)
        {
            xy::ClientID id;
            packet >> id;
            sf::Vector2f position;
            packet >> position.x >> position.y;

            XY_ASSERT(m_playerEntities.find(id) != m_playerEntities.end(), "Player ID does not exist");
            m_playerEntities[id]->getComponent<NetworkController>()->setDestination(position);
        }
        break;
    case PacketIdent::DirectionUpdate:
    {
        xy::ClientID id;
        Direction direction;
        packet >> id >> direction;

        XY_ASSERT(m_playerEntities.find(id) != m_playerEntities.end(), "Player ID does not exist");
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
    case PacketIdent::TransportStateChanged:
    {
        TransportStatus ts;
        packet >> ts;
        m_gameUI.setTransportStatus(ts);

        switch (ts)
        {
        default: break;
        case TransportStatus::Playing:
            m_programFinished = false;
            break;
        }
    }
        break;
    case PacketIdent::ProgramStatus:
    {
        ProgramState ps;
        packet >> ps;
        switch (ps)
        {
        default: break;
        case ProgramState::Finished:
            m_gameUI.setTransportStatus(TransportStatus::Stopped);
            LOG("Program Finished!", xy::Logger::Type::Info);
            break;
        case ProgramState::Rewound:
            m_programFinished = true;
            break;
        }
    }
        break;
    default: break;
    }
}