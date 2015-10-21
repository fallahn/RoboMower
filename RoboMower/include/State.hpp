//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//abstract base class for game states

#ifndef STATE_HPP_
#define STATE_HPP_

#include <StateIds.hpp>

#include <SFML/Graphics/View.hpp>

#include <memory>

namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack;
class App;
class Message;
class State
{
public:
    using Ptr = std::unique_ptr<State>;

    struct Context
    {
        Context(sf::RenderWindow& renderWindow, App& app);
        sf::RenderWindow& renderWindow;
        App& appInstance;
        sf::View defaultView;
    };

    State(StateStack& stateStack, Context context);
    virtual ~State() = default;
    State(const State&) = delete;
    const State& operator =(const State&) = delete;

    virtual bool handleEvent(const sf::Event& evt) = 0;
    virtual void handleMessage(const Message&) = 0;
    virtual bool update(float dt) = 0;
    virtual void draw() = 0;
    virtual States::ID stateID() const = 0;

    void setContext(Context);

protected:
    void requestStackPush(States::ID id);
    void requestStackPop();
    void requestStackClear();

    Context getContext() const;

    //TODO will we need loading screen?

private:
    StateStack& m_stateStack;
    Context m_context;
};

#endif //STATE_HPP_