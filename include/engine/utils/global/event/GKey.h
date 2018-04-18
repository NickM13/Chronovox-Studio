#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\LOpenGL.h"
#include <vector>

// Global class for Key events
class GKey {
public:
	struct KeyEvent {
		Sint32 keyCode, scanCode, action, mods;
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

	static std::vector<KeyEvent> getKeyEvents();

	// Call update just before polling events
	static void reset();
};
