//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

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
    ScrollHandle       = 0x20
};

#endif //COMMAND_CATEGORIES_HPP_
