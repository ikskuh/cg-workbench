#pragma once

#include <string>

namespace FileIO
{
	std::string OpenDialog(char const * filter);

	std::string SaveDialog(char const * filter);
};
