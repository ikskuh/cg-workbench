#pragma once

#include <string>

namespace FileIO
{
    std::string GetExecutableDirectory();

    std::string __cdecl GetWorkingDirectory();

    void SetWorkingDirectory(std::string const & text);

	std::string RemoveLastPathComponent(std::string const & text);

	std::string OpenDialog(char const * filter);

	std::string SaveDialog(char const * filter);
};
