#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\LOpenGL.h"
#include <vector>
#include <string>

// Global class for Key events
class GKey {
public:
	struct KeyEvent {
		Sint32 keyCode, scanCode, action, mods;
	};
	struct KeyBind {
		Sint32 key, mods;
		KeyBind(Sint32 p_key = -1, Sint32 p_mods = -1)
			: key(p_key), mods(p_mods) {}
	};
private:
	struct KeyCommand {
		Sint32 state, mods;
	};
	enum KeyState {
		KEY_PRESS = 1,
		KEY_DOWN = 2,
		KEY_RELEASE = 4
	};
	static KeyCommand m_keyCommands[1024];
	static std::vector<KeyEvent> m_keyEvents;
public:
	static void keyCallback(GLFWwindow* p_window, Sint32 p_keyCode, Sint32 p_scanCode, Sint32 p_action, Sint32 p_mods);

	static bool keyPressed(Sint32 p_key, Sint32 p_mods = -1);
	static bool keyDown(Sint32 p_key, Sint32 p_mods = -1);
	static bool keyRelease(Sint32 p_key);
	static bool modDown(Sint32 p_mod);

	static std::string getBindAsText(KeyBind p_keyBind);

	static std::vector<KeyEvent> getKeyEvents();

	// Call reset before polling events
	static void reset();
};
