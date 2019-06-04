#pragma once

#include "..\logger\Logger.h"
#include <thread>

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
	static std::thread* scriptThread;

public:
	static void init();
	static void terminate();

	static lua_State* getLuaState() { return L; }
	static Namespace getGlobal() { return getGlobalNamespace(L); }

	// Load file (from "res/script")
	static bool loadScriptFile(std::string p_scriptFile);
	static bool loadScriptString(std::string p_script);
};
