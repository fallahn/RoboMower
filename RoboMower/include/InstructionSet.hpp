//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//defines the instruction set / vm which powers the mower logic

#ifndef INSTRUCTION_SET_HPP_
#define INSTRUCTION_SET_HPP_

enum Instruction
{
    NOP,
    EngineOn = 0x1,
    EngineOff,
    Forward,
    Right,
    Left,
    Loop,
    Value
};

#endif //INSTRUCTION_SET_HPP_