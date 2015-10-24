//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//lists unique Ids of custom component types

#ifndef COMPONENT_IDS_HPP_
#define COMPONENT_IDS_HPP_

#include <xygine/Component.hpp>

namespace ComponentId
{
    enum
    {
        RoundedRectangle = xy::Component::UniqueId::Count //starting from here means any custom IDs are contiguous
    };
}

#endif //COMPONENT_IDS_HPP_