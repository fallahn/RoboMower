//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef STATE_IDS_HPP_
#define STATE_IDS_HPP_

namespace States
{
    enum class ID
    {
        None,
        Game,
        MenuBackground,
        MenuMain,
        MenuLobby,
        MenuJoin,
        MenuOptions,
        MenuPaused,
        Score,
        Stats,
        Title
    };
}

//TODO there should be a more intuitive place to put these
enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

#endif //STATE_IDS_HPP_