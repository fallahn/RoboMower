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

#include <PacketEnums.hpp>
#include <NetProtocol.hpp>

//---------------------------------------------------------
sf::Packet& operator << (sf::Packet& p, TransportStatus ts)
{
    return p << static_cast<sf::Uint8>(ts);
}

sf::Packet& operator >> (sf::Packet& p, TransportStatus& ts)
{
    sf::Uint8 its;
    p >> its;
    ts = static_cast<TransportStatus>(its);
    return p;
}
//---------------------------------------------------------
sf::Packet& operator << (sf::Packet& p, TransportChange tc)
{
    return p << sf::Uint8(tc);
}

sf::Packet& operator >> (sf::Packet& p, TransportChange& tc)
{
    sf::Uint8 a;
    p >> a;
    tc = static_cast<TransportChange>(a);
    return p;
}
//---------------------------------------------------------
sf::Packet& operator << (sf::Packet& p, Direction d)
{
    return p << sf::Uint8(d);
}

sf::Packet& operator >> (sf::Packet& p, Direction& d)
{
    sf::Uint8 pid;
    p >> pid;
    d = static_cast<Direction>(pid);
    return p;
}
//---------------------------------------------------------
sf::Packet& operator << (sf::Packet& p, ProgramState d)
{
    return p << sf::Uint8(d);
}

sf::Packet& operator >> (sf::Packet& p, ProgramState& d)
{
    sf::Uint8 pid;
    p >> pid;
    d = static_cast<ProgramState>(pid);
    return p;
}
//---------------------------------------------------------
sf::Packet& operator << (sf::Packet& p, PacketIdent id)
{
    return p << xy::PacketID(id);
}

sf::Packet& operator >> (sf::Packet& p, PacketIdent& id)
{
    xy::PacketID a;
    p >> a;
    id = static_cast<PacketIdent>(a);
    return p;
}
//---------------------------------------------------------