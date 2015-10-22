//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <MenuOptionState.hpp>

#include <xygine/Util.hpp>
#include <xygine/App.hpp>
#include <xygine/Log.hpp>

#include <xygine/ui/Slider.hpp>
#include <xygine/ui/CheckBox.hpp>
#include <xygine/ui/Selection.hpp>
#include <xygine/ui/Button.hpp>
#include <xygine/ui/TextBox.hpp>

#include <SFML/Window/Event.hpp>

namespace
{
    
}

using namespace xy;

MenuOptionState::MenuOptionState(StateStack& stateStack, Context context)
    : State     (stateStack, context),
    m_messageBus(context.appInstance.getMessageBus())
{
    //m_menuSprite.setTexture(context.appInstance.getTexture("assets/images/main_menu.png"));
    //m_menuSprite.setPosition(context.defaultView.getCenter());
    //Util::Position::centreOrigin(m_menuSprite);
    //m_menuSprite.move(0.f, -40.f);

    m_cursorSprite.setTexture(context.appInstance.getTexture("assets/images/ui/cursor.png"));
    m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
    
    const auto& font = context.appInstance.getFont("assets/fonts/N_E_B.ttf");
    buildMenu(font);

    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.type = Message::UIEvent::MenuOpened;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuOptions;
    m_messageBus.post(msg);
}

//public
bool MenuOptionState::update(float dt)
{
    m_uiContainer.update(dt);
    return true;
}

void MenuOptionState::draw()
{
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().defaultView);
    //rw.draw(m_menuSprite);

    rw.draw(m_uiContainer);

    for (const auto& t : m_texts)
    {
        rw.draw(t);
    }

    rw.draw(m_cursorSprite);
}

bool MenuOptionState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyReleased)
    {
        switch (evt.key.code)
        {
        case sf::Keyboard::Tab:

            return false;
        case sf::Keyboard::Space:

            return false;
        default: break;
        }
    }
    else if (evt.type == sf::Event::JoystickButtonReleased)
    {
        switch (evt.joystickButton.button)
        {
        case 7: //start on xbox

            return false;
        default: break;
        }
    }
    
    //pass remaining events to menu
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainer.handleEvent(evt, mousePos);
    m_cursorSprite.setPosition(mousePos);

    return false; //consume events
}

void MenuOptionState::handleMessage(const Message& msg)
{
    switch (msg.type)
    {
    case Message::Type::UI:
        switch (msg.ui.type)
        {
        case Message::UIEvent::MenuClosed:
            /*if (msg.ui.stateId == States::ID::Help)
            {
                const auto& rw = getContext().renderWindow;
                auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
                m_cursorSprite.setPosition(mousePos);
            }*/
            break;
        default: break;
        }
        break;
    default: break;
    }
}

//private
void MenuOptionState::buildMenu(const sf::Font& font)
{
    auto soundSlider = std::make_shared<ui::Slider>(font, getContext().appInstance.getTexture("assets/images/ui/slider_handle.png"), 375.f);
    soundSlider->setPosition(600.f, 470.f);
    soundSlider->setText("Volume");
    soundSlider->setMaxValue(1.f);
    soundSlider->setCallback([this](const ui::Slider* slider)
    {
        //send volume setting command
        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = Message::UIEvent::RequestVolumeChange;
        msg.ui.value = slider->getValue();
        m_messageBus.post(msg);

    }, ui::Slider::Event::ValueChanged);
    soundSlider->setValue(getContext().appInstance.getAudioSettings().volume); //set this *after* callback is set
    m_uiContainer.addControl(soundSlider);

    auto muteCheckbox = std::make_shared<ui::CheckBox>(font, getContext().appInstance.getTexture("assets/images/ui/checkbox.png"));
    muteCheckbox->setPosition(1070.f, 430.f);
    muteCheckbox->setText("Mute");
    muteCheckbox->setCallback([this](const ui::CheckBox* checkBox)
    {
        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = (checkBox->checked()) ? Message::UIEvent::RequestAudioMute : Message::UIEvent::RequestAudioUnmute;
        m_messageBus.post(msg);
    }, ui::CheckBox::Event::CheckChanged);
    muteCheckbox->check(getContext().appInstance.getAudioSettings().muted);
    m_uiContainer.addControl(muteCheckbox);


    auto resolutionBox = std::make_shared<ui::Selection>(font, getContext().appInstance.getTexture("assets/images/ui/scroll_arrow.png"), 375.f);
    resolutionBox->setPosition(600.f, 510.f);

    const auto& modes = getContext().appInstance.getVideoSettings().AvailableVideoModes;
    auto i = 0u;
    auto j = 0u;
    for (const auto& m : modes)
    {
        std::string name = std::to_string(m.width) + " x " + std::to_string(m.height);
        sf::Int32 val = (m.width << 16) | m.height;
        resolutionBox->addItem(name, val);
        //select currently active mode
        if (getContext().appInstance.getVideoSettings().VideoMode != m)
            i++;
        else
            j = i;
    }
    if (i < modes.size()) resolutionBox->setSelectedIndex(j);

    m_uiContainer.addControl(resolutionBox);

    auto fullscreenCheckbox = std::make_shared<ui::CheckBox>(font, getContext().appInstance.getTexture("assets/images/ui/checkbox.png"));
    fullscreenCheckbox->setPosition(1070.f, 510.f);
    fullscreenCheckbox->setText("Full Screen");
    fullscreenCheckbox->setCallback([this](const ui::CheckBox*)
    {

    }, ui::CheckBox::Event::CheckChanged);
    fullscreenCheckbox->check((getContext().appInstance.getVideoSettings().WindowStyle & sf::Style::Fullscreen) != 0);
    m_uiContainer.addControl(fullscreenCheckbox);

    auto difficultySelection = std::make_shared<ui::Selection>(font, getContext().appInstance.getTexture("assets/images/ui/scroll_arrow.png"), 375.f);
    difficultySelection->setPosition(600.f, 590.f);
    difficultySelection->addItem("Easy", static_cast<int>(Difficulty::Easy));
    difficultySelection->addItem("Medium", static_cast<int>(Difficulty::Medium));
    difficultySelection->addItem("Hard", static_cast<int>(Difficulty::Hard));
    difficultySelection->selectItem(0);
    difficultySelection->setCallback([this](const ui::Selection* s)
    {
        //send message with new difficulty
        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = Message::UIEvent::RequestDifficultyChange;
        msg.ui.difficulty = static_cast<Difficulty>(s->getSelectedValue());
        m_messageBus.post(msg);
    });
    difficultySelection->selectItem(static_cast<int>(getContext().appInstance.getGameSettings().difficulty));
    m_uiContainer.addControl(difficultySelection);

    auto controllerCheckbox = std::make_shared<ui::CheckBox>(font, getContext().appInstance.getTexture("assets/images/ui/checkbox.png"));
    controllerCheckbox->setPosition(1070.f, 590.f);
    controllerCheckbox->setText("Enable Controller");
    controllerCheckbox->setCallback([this](const ui::CheckBox* checkBox)
    {
        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = (checkBox->checked()) ? Message::UIEvent::RequestControllerEnable : Message::UIEvent::RequestControllerDisable;
        m_messageBus.post(msg);
    }, ui::CheckBox::Event::CheckChanged);
    controllerCheckbox->check(getContext().appInstance.getGameSettings().controllerEnabled);
    m_uiContainer.addControl(controllerCheckbox);

    auto applyButton = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/button.png"));
    applyButton->setText("Apply");
    applyButton->setAlignment(ui::Alignment::Centre);
    applyButton->setPosition(840.f, 770.f);
    applyButton->setCallback([fullscreenCheckbox, resolutionBox, this]()
    {
        auto res = resolutionBox->getSelectedValue();

        App::VideoSettings settings;
        settings.VideoMode.width = res >> 16;
        settings.VideoMode.height = res & 0xFFFF;
        settings.WindowStyle = (fullscreenCheckbox->checked()) ? sf::Style::Fullscreen : sf::Style::Close;
        getContext().appInstance.applyVideoSettings(settings);

        Message msg;
        msg.type = Message::Type::UI;
        msg.ui.type = Message::UIEvent::ResizedWindow;
        m_messageBus.post(msg);

    });
    m_uiContainer.addControl(applyButton);

    auto backButton = std::make_shared<ui::Button>(font, getContext().appInstance.getTexture("assets/images/ui/button.png"));
    backButton->setText("Back");
    backButton->setAlignment(ui::Alignment::Centre);
    backButton->setPosition(1080.f, 770.f);
    backButton->setCallback([this]()
    {
        close();
        requestStackPush((getContext().appInstance.connected()) ? States::ID::MenuPaused : States::ID::MenuMain);
    });
    m_uiContainer.addControl(backButton);
}

void MenuOptionState::close()
{
    requestStackPop();

    Message msg;
    msg.type = Message::Type::UI;
    msg.ui.type = Message::UIEvent::MenuClosed;
    msg.ui.value = 0.f;
    msg.ui.stateId = States::ID::MenuOptions;
    m_messageBus.post(msg);
}