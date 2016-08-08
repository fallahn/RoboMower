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

//handes the in-game UI (not the standard xygine UI components)

#ifndef GAME_UI_HPP_
#define GAME_UI_HPP_

#include <SFML/System/Vector2.hpp>

#include <InstructionSet.hpp>
#include <TransportStatus.hpp>

#include <xygine/State.hpp>
#include <xygine/ShaderResource.hpp>

namespace sf
{
    class Event;
}

namespace xy
{
    class Scene;
    class Message;
    class Entity;
    class MessageBus;
    class App;
    class TextureResource;
    class FontResource;
}

class GameUI final
{
public:
    GameUI(xy::State::Context, xy::TextureResource&, xy::FontResource&, xy::Scene&);
    ~GameUI() = default;
    GameUI(const GameUI&) = delete;
    GameUI& operator = (const GameUI&) = delete;

    void update(float, const sf::Vector2f& mousePos);
    void handleEvent(const sf::Event&);
    void handleMessage(const xy::Message&);

    std::vector<sf::Uint8> getProgram() const;

    TransportStatus getTransportStatus() const { return m_transportStatus; }
    void setTransportStatus(TransportStatus ts) { m_transportStatus = ts; }

private:
    xy::ShaderResource m_shaderResource;
    xy::FontResource& m_fontResource;
    xy::TextureResource& m_textureResource;

    TransportStatus m_transportStatus;

    xy::State::Context m_stateContext;
    xy::Scene& m_scene;
    xy::MessageBus& m_messageBus;
    xy::Entity* m_mouseCursor;
    xy::Entity* m_instructionStack;
    bool m_inputWindowVisible;
    void addInstructionBlock(const sf::Vector2f& position, const sf::Vector2f& offset, Instruction);
    void showInputWindow(sf::Uint64 destId);
};

#endif //GAME_UI_HPP_