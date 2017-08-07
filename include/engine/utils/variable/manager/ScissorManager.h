#pragma once

#include "Manager.h"
#include "..\datatype\Rectangle.h"

class MScissor : public Singleton<MScissor>
{
protected:
	std::vector<Rect> m_unitList;
public:
	MScissor() {};
	MScissor(const MScissor&) {};
	MScissor operator=(const MScissor&) {};
	void toggle();
	Uint32 push(Rect& p_area, bool p_override = false);
	Rect& pop();
};
