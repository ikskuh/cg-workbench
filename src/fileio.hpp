#pragma once

#include <string>

namespace FileIO
{
	std::string GetExecutableDirectory();

	std::string GetCurrentDirectory();

	void SetCurrentDirectory(std::string const & text);

	std::string RemoveLastPathComponent(std::string const & text);

	std::string OpenDialog(char const * filter);

	std::string SaveDialog(char const * filter);
};
