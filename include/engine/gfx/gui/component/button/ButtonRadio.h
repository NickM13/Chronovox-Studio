#pragma once

#include "ButtonToggle.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "engine\sfx\Sound.h"

class CButtonRadio : public Component {
public:
	struct RadioBase {
		enum class Type {
			BUTTON, SUBLIST
		} type = Type::BUTTON;

		bool hovered = false;
		GLfloat hoverTimer = 0;

		virtual Texture* getTexture() {
			return 0;
		}
		virtual std::string getName() {
			return "";
		}
		virtual std::string getDescription() {
			return "";
		}
		virtual GKey::KeyBind getKeyBind() {
			return {};
		}
		virtual std::string getKeyBindText() {
			return "";
		}
		Type getType() {
			return type;
		}
	};
	struct RadioButton : public RadioBase {
		Texture* tex = 0;
		std::string name = "";
		std::string desc = "";
		GKey::KeyBind keyBind = {};
		std::string keyBindText = "";

		Sint8 state = 0;
		bool hovered = false;
		GLfloat hoverTimer = 0;
		RadioButton(Texture* p_tex, std::string p_name, std::string p_desc, GKey::KeyBind p_keyBind) {
			tex = p_tex;
			name = p_name;
			desc = p_desc;
			keyBind = p_keyBind;
			keyBindText = GKey::getBindAsText(p_keyBind);
			type = Type::BUTTON;
		}

		Texture* getTexture() {
			return tex;
		}
		std::string getName() {
			return name;
		}
		std::string getDescription() {
			return desc;
		}
		GKey::KeyBind getKeyBind() {
			return keyBind;
		}
		std::string getKeyBindText() {
			return keyBindText;
		}
	};
	struct RadioSublist : public RadioBase {
		std::vector<RadioButton*> rblist;
		
		std::string name = "";
		Sint32 selected = 0;
		Sint32 nameWidth = 0, bindWidth = 0; // bindWidth is the width of the keybind text

		RadioSublist(std::string p_name) {
			name = p_name;
			type = Type::SUBLIST;
		}

		RadioSublist* addButton(RadioButton* p_button) {
			rblist.push_back(p_button);
			nameWidth = static_cast<Sint32>(std::fmaxf(nameWidth, Font::getMessageWidth(p_button->getName() + "  ").x));
			bindWidth = static_cast<Sint32>(std::fmaxf(bindWidth, Font::getMessageWidth(GKey::getBindAsText(p_button->getKeyBind())).x));
			return this;
		}
		std::vector<RadioButton*> getRBList() {
			return rblist;
		}
		Sint32 getNameWidth() const {
			return nameWidth;
		}
		Sint32 getBindWidth() const {
			return bindWidth;
		}
		Sint32 getTotalWidth() {
			return nameWidth + bindWidth;
		}

		Texture* getTexture() {
			return rblist[selected]->getTexture();
		}
		std::string getName() {
			return name;
		}
		std::string getDescription() {
			return rblist[selected]->getDescription();
		}
		GKey::KeyBind getKeyBind() {
			return rblist[selected]->getKeyBind();
		}
		std::string getKeyBindText() {
			return rblist[selected]->getKeyBindText();
		}
	};

private:
	Vector2<Sint32> m_buttonSize = {}, m_buttonStep = {};
	Uint16 m_selectedButton = 0, m_prevSelectedButton = 0;
	Sint32 m_selectedHover = -1;
	Sint32 m_iconWidth = 0;

	// Texture to be rendered ontop of icons if they contain
	// more than one options
	Texture* m_texSublist = 0;
	std::vector<RadioSublist*> m_buttonList;
public:
	CButtonRadio(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_buttonSize, Vector2<Sint32> p_buttonStep);
	~CButtonRadio();

	Component* addButton(Texture* p_tex, std::string p_parent, std::string p_name, std::string p_desc, GKey::KeyBind p_keyBind);
	void setSize(Vector2<Sint32> p_size) {}

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();

	void setSelectedItem(Uint16 p_selectedButton);
	std::string getSelectedRadio();

	Sint16 getSelectedItem()	 { return m_selectedButton; }
	Sint16 getPrevSelectedItem() { return m_prevSelectedButton; }
	Sint32 getButtonCount()		 { return m_buttonList.size(); }
};
