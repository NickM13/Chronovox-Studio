#pragma once

#include "..\LTexture.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\variable\manager\ScissorManager.h"

enum Anchor {
	NOANCHOR = 0,
	CENTER = 1,
	LEFT = 2,
	RIGHT = 3,
	TOP = 4,
	BOTTOM = 5,
	TOP_LEFT = 6,
	TOP_RIGHT = 7,
	BOTTOM_LEFT = 8,
	BOTTOM_RIGHT = 9
};

class Component
{
protected:
	typedef void(*function)();
public:
	enum EventFlag {
		EVENT_MOUSEOVER = 1,
		EVENT_KEYPRESS = 2,
		EVENT_MOUSESCROLL = 4,
		EVENT_ALL = 7
	};
	enum Border {
		BORDER_NONE = 0,
		BORDER_TOP = 1,
		BORDER_RIGHT = 2,
		BORDER_BOTTOM = 4,
		BORDER_LEFT = 8,
		BORDER_ALL = 15
	};
	enum Theme {
		PRIMARY = 0,
		MENUBAR = 1,
		INFO = 2,
		ACTION = 3,
		ACTION_LIGHT = 4
	};
	enum TextureStyle {
		NOTEX = -1,
		STRETCH = 0,
		WRAP = 1,
		SCALE = 2
	};

protected:
	function m_pressFunction = 0, m_holdFunction = 0, m_releaseFunction = 0;
	struct ColorTheme
	{
		Color m_border, m_primary, m_select, m_hover, m_text, m_textLight;
		ColorTheme(Color p_border = {}, Color p_primary = {}, Color p_select = {}, Color p_hover = {}, Color p_text1 = {}, Color p_text2 = {})
		{
			m_border = p_border;
			m_primary = p_primary;
			m_select = p_select;
			m_hover = p_hover;
			m_text = p_text1;
			m_textLight = p_text2;
		}
	};

	std::string m_compName, m_title;
	Vector2<Sint32> m_posInit, m_pos, m_sizeInit, m_size;
	Sint8 m_selected;
	bool m_hovered;
	ColorTheme m_colorTheme;

	Sint32 m_texture = -1;
	TextureStyle m_textureStyle = TextureStyle::STRETCH;

	Sint8 m_border = Border::BORDER_ALL;

	Sint32* m_numValue = 0;

	// Default color themes
	static std::vector<ColorTheme> m_colorThemes;
	bool m_visible = true;
	Sint8 m_moveToFront = 0;
	Sint8 m_priority = 0;

	std::string m_tooltip;
	GLfloat m_tooltipTime = 0;
	bool m_tooltipCounted;
	Vector2<Sint32> m_tooltipMouse;
public:
	Component();
	Component(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme = 0);
	virtual ~Component();

	virtual Component* addComponent(Component* p_comp, Anchor p_posAnchor = Anchor::NOANCHOR, Anchor p_sizeAnchor = Anchor::NOANCHOR);
	virtual Component* findComponent(std::string p_compName);

	virtual Component* addButton(std::string p_dir, std::string p_buttonName, std::string p_desc = "", function p_func = 0);
	virtual Component* addItem(std::string p_item);
	virtual Uint16 getItemCount();
	virtual void setList(std::vector<std::string> p_items);
	virtual bool hasList();

	Component* setPressFunction(function p_func);
	Component* setHoldFunction(function p_func);
	Component* setReleaseFunction(function p_func);
	Component* callPressFunction() { if(m_pressFunction != 0) m_pressFunction(); return this; };
	Component* callHoldFunction() { if(m_holdFunction != 0) m_holdFunction(); return this; };
	Component* callReleaseFunction() { if(m_releaseFunction != 0) m_releaseFunction(); return this; };

	virtual void setSelectedItem(Uint16 p_selectedItem);
	virtual void setSelectedButton(Uint16 p_selectedButton);
	virtual Sint16 getSelectedItem();
	virtual Sint16 getPrevSelectedItem();

	std::string getName();
	virtual std::string getTitle();

	void setTooltip(std::string p_tooltip);
	void addTooltip();
	void resetTooltip();

	virtual void setTitle(std::string p_title);
	Component* setTheme(Theme p_theme) { m_colorTheme = m_colorThemes[p_theme]; return this; }
	Component* setBorder(Sint8 p_border) { m_border = p_border; return this; } // Use flags from enum Component::Border
	virtual void resize();
	virtual void setPosition(Vector2<Sint32> p_pos);
	virtual void setSize(Vector2<Sint32> p_size);
	Vector2<Sint32> getInitialPosition();
	Vector2<Sint32> getInitialSize();
	Vector2<Sint32> getPosition();
	Vector2<Sint32> getSize();
	virtual Vector2<Sint32> getRealPosition();
	virtual Vector2<Sint32> getRealSize();

	virtual Component* setVisible(bool p_visible);
	bool isVisible();

	void renderBack();
	void renderFill(bool p_setColor = true);
	void renderBorder();
	virtual void render();
	virtual Sint8 isSelected();
	virtual void setValue(Sint32 p_value);
	Sint32* getValue();

	Component* setPriorityLayer(Sint8 p_priority);
	Sint8 getPriorityLayer();

	virtual void setState(Sint8 p_selected);
	virtual Uint8 isUpdated();

	virtual void input();
	virtual void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	virtual void update(GLfloat p_deltaUpdate);
};
