#pragma once

#include "..\logger\Logger.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <LuaBridge.h>

using namespace luabridge;

class GLua {
private:
	static lua_State* L;
public:
	static void init();
	static void terminate();

	static lua_State* getLuaState() { return L; }

	static bool loadScriptFile(std::string p_scriptFile);
	static bool loadScriptString(std::string p_script);
};
