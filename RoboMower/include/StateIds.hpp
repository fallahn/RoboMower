//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef STATE_IDS_HPP_
#define STATE_IDS_HPP_

namespace States
{
    enum ID
    {
        None = 0,
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

#endif //STATE_IDS_HPP_