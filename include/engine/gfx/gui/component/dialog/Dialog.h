#pragma once
#include "..\container\Container.h"
#include "..\button\Button.h"

class CDialog : public Container {
private:
	Sint32 m_buffer = 0, m_spacing = 0, m_currWidth = 0;
	Sint32 m_optionBorder = 0;

	bool m_active = false;
public:
	CDialog() {};
	CDialog(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);

	void setActive(bool p_active);
	bool isActive();

	CDialog* addOption(std::string p_title, std::string p_description, function p_func);
	CDialog* setOptionFunc(std::string p_title, function p_func);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_updateTime);
	void render();
};
