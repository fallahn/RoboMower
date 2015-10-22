//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//static functions for cross platform file handling

#ifndef FILE_SYS_HPP_
#define FILE_SYS_HPP_

#include <string>
#include <vector>

class FileSystem final
{
public:

    static std::vector<std::string> listFiles(std::string path);
    static std::string getFileExtension(const std::string& path);

private:

};

#endif //FILE_SYS_H_