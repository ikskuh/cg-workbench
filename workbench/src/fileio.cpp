#include "fileio.hpp"

#include <nfd.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#ifdef WIN32
#include <Windows.h>
#include "Shlwapi.h"
#else
#include <unistd.h>
#include <libgen.h>
#endif

namespace FileIO
{
    std::string GetWorkingDirectory()
    {
#ifdef WIN32
        static char cwd[MAX_PATH];
        ::GetCurrentDirectoryA(sizeof(cwd), cwd);
        return std::string(cwd);
#else
        static char cwd[PATH_MAX];
        return std::string(getcwd(cwd, sizeof(cwd)));
#endif
    }

    void SetWorkingDirectory(std::string const & text)
	{
#ifdef WIN32
        ::SetCurrentDirectory(text.c_str());
#else
		chdir(text.c_str());
#endif
	}

	std::string GetExecutableDirectory()
	{
#ifdef WIN32
		TCHAR path[MAX_PATH];
		assert(GetModuleFileName(NULL, path, sizeof(path)) > 0);

		assert(PathRemoveFileSpec(path) == TRUE);

		return std::string(path);
#else
		char path[PATH_MAX];
		assert(readlink("/proc/self/exe", path, sizeof(path)) > 0);
		return std::string(dirname(path));
#endif
	}

	std::string OpenDialog(char const * filter)
	{
		nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog(filter, FileIO::GetWorkingDirectory().c_str(), &outPath );
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
        nfdresult_t result = NFD_SaveDialog(filter, FileIO::GetWorkingDirectory().c_str(), &outPath );
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

	std::string RemoveLastPathComponent(std::string const & text)
	{
#ifdef WIN32
		TCHAR path[MAX_PATH];
		strcpy(path, text.c_str());
		assert(PathRemoveFileSpec(path) == TRUE);
		return std::string(path);
#else
		char path[PATH_MAX];
		strcpy(path, text.c_str());
		return std::string(dirname(path));
#endif
	}
}
