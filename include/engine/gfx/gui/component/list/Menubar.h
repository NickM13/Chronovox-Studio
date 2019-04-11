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
		virtual std::string getDirectoryNoSlash() const {
			if (parent) {
				return parent->getDirectory();
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
		std::string getDirectoryNoSlash() const {
			if (parent) {
				return parent->getDirectory() + name;
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

			desc = GKey::getBindAsText(p_keyBind);
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
	std::vector<std::pair<GKey::KeyBind, function>> m_keyBinds;
	const Sint32 m_slBuffer = 2;

	Panel* m_panelMain = {};
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
