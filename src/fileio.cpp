#include "fileio.hpp"

#include <nfd.h>
#include <unistd.h>
#include <limits.h>

namespace FileIO
{
	static char cwd[PATH_MAX];

	std::string OpenDialog(char const * filter)
	{
		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_OpenDialog(filter, getcwd(cwd, sizeof(cwd)), &outPath );
		if ( result == NFD_OKAY )
		{
 			std::string path(outPath);
			free(outPath);
			return path;
		}
		else if ( result == NFD_CANCEL )
			; // Silently ignore cancel
		else
		{
			printf("Error: %s\n", NFD_GetError() );
		}
		return "";
	}

	std::string SaveDialog(char const * filter)
	{
		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(filter, getcwd(cwd, sizeof(cwd)), &outPath );
		if ( result == NFD_OKAY )
		{
 			std::string path(outPath);
			free(outPath);
			return path;
		}
		else if ( result == NFD_CANCEL )
			; // Silently ignore cancel
		else
		{
			printf("Error: %s\n", NFD_GetError() );
		}
		return "";
	}
}
