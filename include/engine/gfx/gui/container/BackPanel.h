#pragma once

#include "..\Component.h"

class BackPanel : public Component
{
public:
	BackPanel() {}

	void render()
	{
		glPushMatrix();
		{
			color.useColor();
			glScalef(GScreen::m_screenSize.x / 2, GScreen::m_screenSize.y / 2, 0);
			glBegin(GL_QUADS);
			{
				glVertex2f(-1, -1);
				glVertex2f(1, -1);
				glVertex2f(1, 1);
				glVertex2f(-1, 1);
			}
			glEnd();
		}
		glPopMatrix();
	}
private:
	const Color color = Color(0, 0, 0, 0.25f);
};
