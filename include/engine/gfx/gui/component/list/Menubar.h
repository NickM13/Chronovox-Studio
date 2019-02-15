#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "..\container\Panel.h"

class CMenubar : public Component {
private:
	struct SubList {
		std::string m_name, m_desc;
		GKey::KeyBind m_keyBind;
		std::function<bool()> m_isFunc;
		bool m_subList;
		std::vector<SubList> m_buttons;
		Uint16 m_width;
		function m_func = 0;
		SubList() {};
		SubList(std::string p_name, GKey::KeyBind p_keyBind, function p_func = 0, std::function<bool()> p_isFunc = 0) {
			m_name = p_name;
			m_keyBind = p_keyBind;
			m_desc = "";
			if (m_keyBind.key != -1) {
				if (m_keyBind.mods & GLFW_MOD_SUPER)	{ m_desc += "Super+"; }
				if (m_keyBind.mods & GLFW_MOD_CONTROL)	{ m_desc += "Ctrl+"; }
				if (m_keyBind.mods & GLFW_MOD_ALT)		{ m_desc += "Alt+"; }
				if (m_keyBind.mods & GLFW_MOD_SHIFT)	{ m_desc += "Shift+"; }
				switch (m_keyBind.key) {
				case GLFW_KEY_F1: m_desc += "F1"; break;
				case GLFW_KEY_F2: m_desc += "F2"; break;
				case GLFW_KEY_F3: m_desc += "F3"; break;
				case GLFW_KEY_F4: m_desc += "F4"; break;
				case GLFW_KEY_F5: m_desc += "F5"; break;
				case GLFW_KEY_F6: m_desc += "F6"; break;
				case GLFW_KEY_F7: m_desc += "F7"; break;
				case GLFW_KEY_F8: m_desc += "F8"; break;
				case GLFW_KEY_F9: m_desc += "F9"; break;
				case GLFW_KEY_F10: m_desc += "F10"; break;
				case GLFW_KEY_F11: m_desc += "F11"; break;
				case GLFW_KEY_F12: m_desc += "F12"; break;
				case GLFW_KEY_SPACE: m_desc += "Space"; break;
				default: m_desc += (char)m_keyBind.key; break;
				}
			}
			m_subList = false;
			m_width = 0;
			m_func = p_func;
			m_isFunc = p_isFunc;
		}

		void setFunction(function p_func) {
			m_func = p_func;
		}

		function getFunction() {
			return m_func;
		}

		void callPressFunction() {
			m_func();
		}

		SubList* find(std::string p_name) {
			if (m_subList) {
				for (Uint16 i = 0; i < m_buttons.size(); i++) {
					if (m_buttons[i].m_name == p_name) {
						return &m_buttons[i];
					}
				}
			}
			return 0;
		}

		void addButton(std::string p_name, GKey::KeyBind p_keyBind, function p_func, std::function<bool()> p_isFunc) {
			m_buttons.push_back(SubList(p_name, p_keyBind, p_func, p_isFunc));
			m_subList = true;
			if (p_name.length() > m_width) {
				m_width = Uint16(p_name.length());
			}
		}
	};
	std::vector<std::pair<GKey::KeyBind, function>> m_keyBinds;

	Panel* m_panelMain, *m_panelSub;
	SubList m_buttonsMain;
	Texture* m_texHasSubmenu;

	std::string m_currDir;
	std::string m_selected; // Directory to button being hovered over
public:
	CMenubar() {};
	CMenubar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);
	~CMenubar();

	//Directory splits with /'s or \\'s 
	CMenubar* addButton(std::string p_dir, std::string p_buttonName, GKey::KeyBind p_keyBind = { -1 }, function p_func = 0, std::function<bool()> p_isFunc = 0);
	void setSize(Vector2<Sint32> p_pos);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
