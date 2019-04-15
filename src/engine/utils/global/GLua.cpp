#include "engine\utils\global\GLua.h"
#include "engine\utils\global\GScreen.h"

lua_State* GLua::L = 0;

void GLua::init() {
	if (L) return;
	L = luaL_newstate();
	luaL_openlibs(L);

	Namespace nGlob = getGlobalNamespace(L);
	nGlob.addFunction("logger", &Logger::logLua);
}

void GLua::terminate() {
	if (L) {
		lua_close(L);
	}
}

bool GLua::loadScriptFile(std::string p_scriptFile) {
	int r = luaL_dofile(L, p_scriptFile.c_str());
	if (r == LUABRIDGE_LUA_OK) {
		return true;
	} else {
		Logger::logError("Lua script could not be run \"%s\"\n%s", p_scriptFile, std::string(lua_tostring(L, -1)));
		return false;
	}
}
bool GLua::loadScriptString(std::string p_script) {
	int r = luaL_dostring(L, p_script.c_str());
	if (r == LUABRIDGE_LUA_OK) {
		return true;
	} else {
		Logger::logError("Lua string could not be run \n%s", std::string(lua_tostring(L, -1)));
		return false;
	}
}
