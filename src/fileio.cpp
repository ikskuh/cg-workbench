#include "fileio.hpp"

#include <nfd.h>
#include <limits.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace FileIO
{
    static char const * GetCurrentDir()
    {
#ifdef WIN32
        static char cwd[MAX_PATH];
        GetCurrentDirectoryA(sizeof(cwd), cwd);
        return cwd;
#else
        static char cwd[PATH_MAX];
        return getcwd(cwd, sizeof(cwd));
#endif
    }

	std::string OpenDialog(char const * filter)
	{
		nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog(filter, GetCurrentDir(), &outPath );
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
        nfdresult_t result = NFD_SaveDialog(filter, GetCurrentDir(), &outPath );
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
