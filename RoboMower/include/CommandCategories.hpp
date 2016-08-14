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

//categories in which entities can be placed so that they may be targeted by commands

#ifndef COMMAND_CATEGORIES_HPP_
#define COMMAND_CATEGORIES_HPP_

enum CommandCategory
{
    TrayIcon           = 0x1,
    Cursor             = 0x2,
    InstructionBlock   = 0x4,
    ParticleController = 0x8,
    InstructionList    = 0x10,
    ScrollHandle       = 0x20,
    InputBox           = 0x40,
    InputPopup         = 0x80,
    TransportControl   = 0x100,
    LoopingHandle      = 0x200
};

#endif //COMMAND_CATEGORIES_HPP_
