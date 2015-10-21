//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//read / write score data

#ifndef SCORES_HPP_
#define SCORES_HPP_

#include <StateIds.hpp>

#include <vector>
#include <string>

class Scores final
{
public:
    struct Chunk
    {
        unsigned int offset;
        unsigned int size;
    };

    struct Header
    {
        int ident;
        int version;
        Chunk chunks[3];
    };

    struct Item
    {
        char name[21];
        float score;
        std::size_t hash;
    };

    Scores() = default;
    ~Scores() = default;
    Scores(const Scores&) = delete;
    const Scores& operator = (const Scores&) = delete;

    void load();
    void save();
    int add(const std::string&, float, Difficulty);

    const std::vector<Item>& getScores(Difficulty) const;

private:
    std::vector<Item> m_easyScores;
    std::vector<Item> m_mediumScores;
    std::vector<Item> m_hardScores;
};

#endif //SCORES_HPP_