//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//command class for targetting entities within a scene graph

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <SFML/Config.hpp>

#include <functional>
#include <queue>

class Entity;
struct Command final
{
    std::function<void(Entity&, float)> action;
    sf::Uint64 entityID = 0u;
    enum
    {
        None      = 0,
        Player    = 0x1,
        Alien     = 0x2,
        Particle  = 0x4,
        GameRules = 0x8,
        Human     = 0x10,
        All = Player | Alien | Particle | GameRules | Human
    }category = None;
};

class CommandQueue final
{
public:
    CommandQueue() = default;
    ~CommandQueue() = default;
    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator = (const CommandQueue&) = delete;

    void push(const Command&);
    Command pop();
    bool empty() const;

private:
    std::queue<Command> m_queue;
};

#endif //COMMAND_HPP_