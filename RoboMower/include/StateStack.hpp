//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//stack of game states

#ifndef STATESTACK_HPP_
#define STATESTACK_HPP_

#include <State.hpp>

#include <map>
#include <functional>
#include <vector>

namespace sf
{
    class RenderWindow;
    class Event;
}

class StateStack final
{
public:
    enum class Action
    {
        Push,
        Pop,
        Clear
    };

    explicit StateStack(State::Context context);
    ~StateStack() = default;
    StateStack(const StateStack&) = delete;
    const StateStack& operator = (const StateStack&) = delete;

    template <typename T>
    void registerState(States::ID id)
    {
        m_factories[id] = [this]()
        {
            return std::make_unique<T>(*this, m_context);
        };
    }

    void update(float dt);
    void draw();
    void handleEvent(const sf::Event& evt);
    void handleMessage(const Message&);

    void pushState(States::ID id);
    void popState();
    void clearStates();

    bool empty() const;

    sf::View updateView();

    void applyPendingChanges();

private:

    struct Pendingchange
    {
        explicit Pendingchange(Action, States::ID id = States::ID::None);
        Action action;
        States::ID id;
    };

    std::vector<State::Ptr> m_stack;
    std::vector<Pendingchange> m_pendingChanges;
    State::Context m_context;
    std::map<States::ID, std::function<State::Ptr()>> m_factories;
    State::Ptr createState(States::ID id);

};

#endif //STATESTACK_HPP_