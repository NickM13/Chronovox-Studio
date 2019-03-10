#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "..\container\Panel.h"

class CMenubar : public Component {
public:
	enum class ElementType {
		BASE, SUBMENU, BUTTON, CHECKBOX, DIVIDER
	};

	class MenuElement {
	protected:
		ElementType etype = ElementType::BASE;
		Sint32 height = 0;
		Sint32 width = 0;
		std::function<bool()> isVisibleFunc = []() -> bool { return true; };
		std::string name = "", desc = "";
		MenuElement* parent = 0;
		GLfloat hoverTimer = 0;
		GLfloat priority = 0;
		bool hoverPop = false;

	public:
		MenuElement() {}

		Sint32 getHeight() { return height; }
		Sint32 getWidth() const { return width; }
		std::string getName() const { return name; }
		std::string getDesc() const { return desc; }

		MenuElement* setParent(MenuElement* p_parent) {
			parent = p_parent;
			return this;
		}
		std::string getPath() {
			if (parent) {
				return parent->getDirectory() + name;
			}
			return "";
		}
		virtual std::string getDirectory() const {
			if (parent) {
				return parent->getDirectory() + "\\";
			}
			return "";
		}

		ElementType getType() const {
			return etype;
		}

		MenuElement* setVisibleFunction(std::function<bool()> p_isVisibleFunc) {
			isVisibleFunc = p_isVisibleFunc;
			return this;
		}
		bool isVisible() const {
			return (isVisibleFunc() && (!parent || parent->isVisible()));
		}

		virtual function getFunction() {
			return 0;
		}
		virtual void callFunction() {}

		void setPriority(GLfloat p_priority) {
			priority = p_priority;
		}
		GLfloat getPriority() const {
			return priority;
		}

		void addHoverTimer(GLfloat t) {
			if ((t > 0 && hoverTimer < 1.f)
				|| (t < 0 && hoverTimer > 0.f)) {
				hoverTimer += t;
				if (hoverTimer > 1.f) {
					hoverTimer = 1.f;
					hoverPop = true;
				}
				else if (hoverTimer < 0.f) {
					hoverTimer = 0.f;
				}
			}
		}
		GLfloat getHoverTimer() const {
			return hoverTimer;
		}
		bool isHoverPopped() const {
			return hoverPop;
		}

		virtual void update() {
			hoverPop = false;
		}
	};

	// Yo dawg I heard you like submenus, so now you can put submenus in your submenus!
	class Submenu : public MenuElement {
	private:
		std::vector<MenuElement*> elements = {};
		std::vector<MenuElement*> visElements = {};
		Sint32 subHeight = 0;

	public:
		Submenu(std::string p_name = "") {
			name = p_name;
			etype = ElementType::SUBMENU;
		}

		GLfloat getSubHeight() {
			return subHeight;
		}

		Submenu* addElement(MenuElement* p_element) {
			p_element->setParent(this);
			elements.push_back(p_element);

			width = 0;
			for (MenuElement* e : elements) {
				switch (e->getType()) {
				case ElementType::SUBMENU:
				case ElementType::BUTTON:
				case ElementType::CHECKBOX:
					width = (Sint32)std::fmax(width, Font::getMessageWidth(e->getName()).x);
					break;
				default: break;
				}
				e->getWidth();
			}

			return this;
		}
		std::vector<MenuElement*> getElements() {
			return elements;
		}
		void updateElements() {
			std::vector<MenuElement*> prevElements = elements;
			elements.clear();

			bool placed;
			for (size_t i = 0; i < prevElements.size(); i++) {
				placed = false;
				for (size_t j = 0; j < elements.size(); j++) {
					if (prevElements[i]->getPriority() < elements[j]->getPriority()) {
						elements.insert(elements.begin() + j, prevElements[i]);
						placed = true;
						break;
					}
				}
				if (!placed) {
					elements.push_back(prevElements[i]);
				}
			}

			visElements.clear();
			subHeight = 0;
			for (MenuElement* e : elements) {
				if (e->isVisible()) {
					visElements.push_back(e);
					if (e->getType() == ElementType::DIVIDER) {
						subHeight += e->getHeight();
					}
					else {
						subHeight += Font::getSpacingHeight();
					}
				}
			}
		}
		std::vector<MenuElement*> getVisibleElements() {
			return visElements;
		}

		Submenu* find(std::string p_name) {
			for (Uint16 i = 0; i < elements.size(); i++) {
				if (elements[i]->getName() == p_name) {
					return static_cast<Submenu*>(elements[i]);
				}
			}
			return 0;
		}

		std::string getDirectory() const {
			if (parent) {
				return parent->getDirectory() + name + "\\";
			}
			return (name != "" ? name + "\\" : "");
		}

		void update() {
			MenuElement::update();
			height = Font::getSpacingHeight();
		}
	};

	// Endpoint buttons on submenus
	class MenuButton : public MenuElement {
	protected:
		GKey::KeyBind keyBind = 0;
		function func = 0;

	public:
		MenuButton(std::string p_name, GKey::KeyBind p_keyBind, function p_func) {
			name = p_name;
			keyBind = p_keyBind;
			func = p_func;
			etype = ElementType::BUTTON;

			desc = "";
			if (keyBind.key != -1) {
				if (keyBind.mods & GLFW_MOD_SUPER) { desc += "Super+"; }
				if (keyBind.mods & GLFW_MOD_CONTROL) { desc += "Ctrl+"; }
				if (keyBind.mods & GLFW_MOD_ALT) { desc += "Alt+"; }
				if (keyBind.mods & GLFW_MOD_SHIFT) { desc += "Shift+"; }
				switch (keyBind.key) {
				case GLFW_KEY_F1: desc += "F1"; break;
				case GLFW_KEY_F2: desc += "F2"; break;
				case GLFW_KEY_F3: desc += "F3"; break;
				case GLFW_KEY_F4: desc += "F4"; break;
				case GLFW_KEY_F5: desc += "F5"; break;
				case GLFW_KEY_F6: desc += "F6"; break;
				case GLFW_KEY_F7: desc += "F7"; break;
				case GLFW_KEY_F8: desc += "F8"; break;
				case GLFW_KEY_F9: desc += "F9"; break;
				case GLFW_KEY_F10: desc += "F10"; break;
				case GLFW_KEY_F11: desc += "F11"; break;
				case GLFW_KEY_F12: desc += "F12"; break;
				case GLFW_KEY_SPACE: desc += "Space"; break;
				default: desc += (char)keyBind.key; break;
				}
			}
		}

		GKey::KeyBind getKeyBind() const {
			return keyBind;
		}

		function getFunction() {
			return ([&]() { if (isVisible()) func(); });
		}
		void callFunction() {
			func();
		}

		void update() {
			MenuElement::update();
			height = Font::getSpacingHeight();
		}
	};

	// Checkbox buttons on submenus
	class MenuCheckbox : public MenuButton {
	private:
		std::function<bool()> isCheckedFunc = 0;

	public:
		MenuCheckbox(std::string p_name, GKey::KeyBind p_keyBind, function p_func, std::function<bool()> p_isCheckedFunc)
		: MenuButton(p_name, p_keyBind, p_func) {
			isCheckedFunc = p_isCheckedFunc;
			etype = ElementType::CHECKBOX;
		}

		bool isChecked() {
			if (isCheckedFunc != 0) {
				return isCheckedFunc();
			}
			return false;
		}
	};

	// ---------
	class MenuDivider : public MenuElement {
	private:

	public:
		MenuDivider() {
			name = "Divider";
			height = 11;
			etype = ElementType::DIVIDER;
		}
	};

private:
	/*
	struct SubList {
		std::function<bool()> m_visibleFunc = {};
		std::string m_directory = "";
		std::string m_name = "", m_desc = "";
		GLfloat hoverTimer = 0;
		GKey::KeyBind m_keyBind = {};
		std::function<bool()> m_isCheckboxFunc = {};
		bool m_subList = false;
		std::vector<SubList> m_buttons = {};
		Uint16 m_width = 0;
		function m_func = 0;
		SubList() {};
		SubList(std::string p_directory, std::string p_name, GKey::KeyBind p_keyBind, function p_func = 0, std::function<bool()> p_isCheckboxFunc = 0) {
			m_directory = p_directory;
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
			m_isCheckboxFunc = p_isCheckboxFunc;
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

		void addButton(std::string m_directory, std::string p_name, GKey::KeyBind p_keyBind, function p_func, std::function<bool()> p_isCheckboxFunc) {
			m_buttons.push_back(SubList(m_directory, p_name, p_keyBind, p_func, p_isCheckboxFunc));
			m_subList = true;
			if (p_name.length() > m_width) {
				m_width = Uint16(p_name.length());
			}
		}
	};
	*/
	//SubList m_buttonsMain = {};

	std::vector<std::pair<GKey::KeyBind, function>> m_keyBinds;
	const Sint32 m_slBuffer = 2;

	Panel* m_panelMain = {}, * m_panelSub = 0;
	Submenu m_submenu;

	// Texture for submenu arrow
	Texture* m_texHasSubmenu = 0;

	std::string m_currDir = "";
	std::string m_selected = ""; // Directory to button being hovered over
public:
	CMenubar() {};
	CMenubar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);
	~CMenubar();

	//Directory splits with /'s or \\'s 
	MenuElement* addElement(std::string p_dir, MenuElement* p_element);
	MenuElement* getElement(std::string p_dir);
	void setSize(Vector2<Sint32> p_pos);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
