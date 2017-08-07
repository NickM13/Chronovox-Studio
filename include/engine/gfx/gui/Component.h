#pragma once

#include "..\LTexture.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\variable\manager\ScissorManager.h"

typedef void(*function)();
class Component
{
public:
	enum EventFlag
	{
		EVENT_MOUSEOVER = 1,
		EVENT_KEYPRESS = 2,
		EVENT_MOUSESCROLL = 4,
		EVENT_ALL = 7
	};
	enum Border
	{
		BORDER_NONE = 0,
		BORDER_TOP = 1,
		BORDER_RIGHT = 2,
		BORDER_BOTTOM = 4,
		BORDER_LEFT = 8,
		BORDER_ALL = 15
	};
	enum Theme
	{
		PRIMARY = 0,
		TOOLBAR = 1,
		INFO = 2,
		ACTION = 3
	};
	enum TextureStyle
	{
		NOTEX = -1,
		STRETCH = 0,
		WRAP = 1,
		SCALE = 2
	};

protected:
	function m_function = 0;
	struct ColorTheme
	{
		Color m_back;
		Color m_fore;
		Color m_active;
		Color m_text;
		Color m_textInfo;
		ColorTheme(Color p_back = {}, Color p_fore = {}, Color p_active = {}, Color p_text1 = {}, Color p_text2 = {})
		{
			m_back = p_back;
			m_fore = p_fore;
			m_active = p_active;
			m_text = p_text1;
			m_textInfo = p_text2;
		}
	};

	std::string m_compName, m_title;
	Vector2<Sint32> m_pos, m_size;
	Sint8 m_selected;
	ColorTheme m_colorTheme;

	Sint32 m_texture = -1;
	TextureStyle m_textureStyle = TextureStyle::STRETCH;

	Sint8 m_border = Border::BORDER_ALL;

	Sint32* m_numValue = 0;

	// Default color themes
	std::vector<ColorTheme> m_colorThemes;
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
	~Component();

	virtual Component* addComponent(Component* p_comp, Sint8 p_alignment = 0);
	virtual Component* findComponent(std::string p_compName);
	virtual std::string getItem(Uint16 p_index);
	virtual Texture getItemTexture(Uint16 p_index); // For Lists
	virtual Uint16 getItemTexId(Uint16 p_index); // For Lists
	virtual Component* addButton(std::string p_dir, std::string p_buttonName, std::string p_desc = "", function p_func = 0);
	virtual Component* addItem(std::string p_item);
	virtual Component* addItem(std::string p_item, Texture p_texture, Uint16 p_texId); // For Lists
	virtual void setList(std::vector<std::string> p_items);
	virtual void setItem(Uint16 p_index, std::string p_item);
	virtual void setItemTexId(Uint16 p_index, Uint16 p_texId); // For Lists
	virtual void setItemTexture(Uint16 p_index, Texture p_texture); // For Lists
	virtual void removeItem(Uint16 p_index);
	virtual Uint16 getItemCount();
	virtual Component* setFunction(function p_func);
	Component* callFunction() { if(m_function != 0) m_function(); return this; };
	virtual void setSelectedItem(Uint16 p_selectedItem);
	virtual void setSelectedButton(Uint16 p_selectedButton);
	virtual Uint8 isUpdated();
	virtual Sint16 getSelectedItem();
	virtual Sint16 getPrevSelectedItem();
	std::string getName();
	virtual std::string getTitle();
	void setTooltip(std::string p_tooltip);
	void addTooltip();
	void resetTooltip();

	virtual void setTitle(std::string p_title);
	void setPosition(Vector2<Sint32> p_pos);
	void setSize(Vector2<Sint32> p_size);
	Vector2<Sint32> getPosition();
	Vector2<Sint32> getSize();
	virtual Vector2<Sint32> getRealPosition();
	virtual Vector2<Sint32> getRealSize();

	virtual Component* setVisible(bool p_visible);
	bool isVisible();

	virtual void setState(Sint8 p_selected);

	virtual void input();
	virtual void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	virtual void update(GLfloat p_deltaUpdate);
	void renderBack();
	void renderFill(bool p_setColor = true);
	virtual void render();
	virtual Sint8 isSelected();
	virtual void setValue(Sint32 p_value);
	Sint32* getValue();
	Sint8 getPriorityLayer();
	void setPriorityLayer(Sint8 p_priority);
	virtual bool hasList();
	Color getColor();
};