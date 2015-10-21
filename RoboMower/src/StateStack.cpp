//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <StateStack.hpp>
#include <App.hpp>
#include <Log.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <cassert>

StateStack::StateStack(State::Context context)
    : m_context (context)
{
    updateView();
}

//public
void StateStack::update(float dt)
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); ++i)
    {
        if (!(*i)->update(dt)) break;
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    for (auto& s : m_stack) s->draw();
}

void StateStack::handleEvent(const sf::Event& evt)
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); ++i)
    {
        if (!(*i)->handleEvent(evt)) break;
    }
    applyPendingChanges();
}

void StateStack::handleMessage(const Message& msg)
{
    if (msg.type == Message::Type::UI)
    {
        switch (msg.ui.type)
        {
        case Message::UIEvent::RequestState:
            pushState(msg.ui.stateId);
            break;
        default: break;
        }
    }
    
    for (auto& s : m_stack) s->handleMessage(msg);
}

void StateStack::pushState(States::ID id)
{
    if (empty() || m_stack.back()->stateID() != id)
    {
        m_pendingChanges.emplace_back(Action::Push, id);
    }
}

void StateStack::popState()
{
    m_pendingChanges.emplace_back(Action::Pop);
}

void StateStack::clearStates()
{
    m_pendingChanges.emplace_back(Action::Clear);
}

bool StateStack::empty() const
{
    return m_stack.empty();
}

sf::View StateStack::updateView()
{
    //calculate the correct view size / ratio for window size
    m_context.defaultView.setSize(1920.f, 1080.f);
    m_context.defaultView.setCenter(960.f, 540.f);

    auto vModeWidth = static_cast<float>(m_context.appInstance.getVideoSettings().VideoMode.width);
    auto vModeHeight = static_cast<float>(m_context.appInstance.getVideoSettings().VideoMode.height);

    auto winSize = sf::Vector2f(vModeWidth, vModeHeight);
    float windowRatio = winSize.x / winSize.y;
    float viewRatio = 16.f / 9.f;

    float sizeY = windowRatio / viewRatio;
    float top = (1.f - sizeY) / 2.f;

    m_context.defaultView.setViewport({ { 0.f, top }, { 1.f, sizeY } });

    for (auto& state : m_stack)
        state->setContext(m_context);

    return m_context.defaultView;
}

//private
State::Ptr StateStack::createState(States::ID id)
{
    auto result = m_factories.find(id);
    assert(result != m_factories.end());

    return result->second();
}

void StateStack::applyPendingChanges()
{
    for (auto& change : m_pendingChanges)
    {
        switch (change.action)
        {
        case Action::Push:
            m_stack.emplace_back(createState(change.id));
            break;
        case Action::Pop:
            m_stack.pop_back();
            break;
        case Action::Clear:
            m_stack.clear();
            break;
        default: break;
        }
    }
    m_pendingChanges.clear();
}

//---------------------------------------

StateStack::Pendingchange::Pendingchange(Action a, States::ID i)
    : action    (a),
    id          (i)
{

}