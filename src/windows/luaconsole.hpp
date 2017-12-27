#ifndef LUACONSOLE_HPP
#define LUACONSOLE_HPP

#include "../window.hpp"

#include <vector>
#include <string>

extern "C"
{
#include <lua.h>
}

class LuaConsole :
	public Window
{
private:
	struct logentry
	{
		bool isError;
		std::string message;
	};

	static const int MaxInputLength = 512;
	std::vector<logentry> log;
	bool scrollToEnd;
	lua_State * lua;
	char inputBuffer[MaxInputLength];
private:
	static int LuaPrint(lua_State * lua);
protected:
	void OnUpdate() override;
public:
	LuaConsole();
	LuaConsole(LuaConsole const &) = delete;
	LuaConsole(LuaConsole &&) = delete;
	virtual ~LuaConsole();

	void Execute(char const * str);

	void Print(char const * str, bool err = false);
};

#endif // LUACONSOLE_HPP
