//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//main entry point for the app

#ifndef APP_HPP_
#define APP_HPP_

#include <StateStack.hpp>
#include <Resource.hpp>
#include <Score.hpp>
#include <MessageBus.hpp>
#include <Server.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>

#include <functional>

class App final
{
public:

    struct VideoSettings final
    {
        sf::Int32 WindowStyle;
        sf::VideoMode VideoMode;
        bool VSync;
        std::vector<sf::VideoMode> AvailableVideoModes;

        VideoSettings()
            : WindowStyle(sf::Style::Close /*sf::Style::Fullscreen*/),
            VideoMode(1024, 768),
            VSync(true){}

        bool operator == (const VideoSettings& vs)
        {
            if (&vs == this) return true;
            return
                (vs.VideoMode == this->VideoMode
                && vs.VSync == this->VSync
                && vs.WindowStyle == this->WindowStyle);
        }
    };

    struct AudioSettings final
    {
        bool muted = false;
        float volume = 1.f;
    };

    struct GameSettings final
    {
        Difficulty difficulty = Difficulty::Easy;
        bool controllerEnabled = true;
        std::array<char, 3u> playerInitials;
        GameSettings() : playerInitials({ { '-', '-', '-' } }){}
    };

    App();
    ~App() = default;
    App(const App&) = delete;
    const App& operator = (const App&) = delete;

    void run();
    void pause();
    void resume();

    const AudioSettings& getAudioSettings() const;

    const VideoSettings& getVideoSettings() const;
    void applyVideoSettings(const VideoSettings&);

    const GameSettings& getGameSettings() const;

    sf::Font& getFont(const std::string& path);
    sf::Texture& getTexture(const std::string& path);
    MessageBus& getMessageBus();
    sf::TcpSocket& getSocket();

    bool connected() const;
    sf::Int16 getClientID() const;
    bool hosting() const;
    void setDestinationIP(const std::string&);

    void addScore(const std::string& name, float value);
    const std::vector<Scores::Item>& getScores() const;
    int getLastScoreIndex() const;

    void setPlayerInitials(const std::string&);

private:

    struct SettingsFile final
    {
        int ident;
        int version;

        sf::VideoMode videoMode;
        sf::Int32 windowStyle;
        AudioSettings audioSettings;
        GameSettings gameSettings;
    };

    GameSettings m_gameSettings;
    AudioSettings m_audioSettings;

    VideoSettings m_videoSettings;
    sf::RenderWindow m_renderWindow;

    StateStack m_stateStack;

    FontResource m_fontResource;
    TextureResource m_textureResource;

    MessageBus m_messageBus;

    Scores m_scores;
    Difficulty m_pendingDifficulty;

    std::unique_ptr<GameServer> m_server;
    sf::TcpSocket m_socket;
    bool m_connected;
    sf::Int16 m_clientID;
    std::string m_destinationIP;

    bool connect(const sf::IpAddress&, sf::Uint16);
    void disconnect();

    void handleEvents();
    void handleMessages();
    std::function<void(float)> update;
    void updateApp(float dt);
    void pauseApp(float dt);
    void draw();

    void registerStates();

    void loadSettings();
    void saveSettings();

    void saveScreenshot();
};

#endif //APP_HPP_