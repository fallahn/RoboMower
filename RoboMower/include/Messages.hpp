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

//extends the xygine message types

#ifndef MESSAGES_HPP_
#define MESSAGES_HPP_

#include <xygine/MessageBus.hpp>

#include <InstructionSet.hpp>
#include <NetProtocol.hpp>
#include <PacketEnums.hpp>

class InstructionBlockLogic;

enum MessageId
{
    TrayIconMessage = xy::Message::Count,
    InstructionBlockMessage,
    ScrollbarMessage,
    InputBoxMessage,
    PlayerMessage,
    TransportMessage,
    DirectionMessage
};

struct TrayIconEvent
{
    enum
    {
        Clicked
    }action;
    Instruction instruction = NOP;
    float absX, absY;
    float relX, relY;
};

struct InstructionBlockEvent
{
    enum
    {
        Moved,
        Dropped,
        PickedUp,
        Destroyed
    }action;
    InstructionBlockLogic* component = nullptr;
    sf::Vector2f position;
};

struct ScrollbarEvent
{
    float position = 0.f;
};

struct InputBoxEvent
{
    enum
    {
        Clicked,
        WindowClosed
    }action;
    sf::Uint64 entityId = 0;
    sf::Uint32 value = 0;
};

struct PlayerEvent
{
    enum
    {
        FinishedCurrentCommand,
        FinishedProgram
    }action;
    xy::ClientID id = -1;
};

struct TransportEvent
{
    enum
    {
        Play,
        Pause,
        Rewind
    }button;
};

struct DirectionEvent
{
    Direction direction;
    xy::ClientID id;
};

#endif //MESSAGE_HPP_