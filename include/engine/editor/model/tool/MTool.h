#pragma once
#include "engine\editor\model\tool\LTool.h"
#include <vector>

class MTool {
private:
	static std::vector<Tool*> m_tools;

	static void addTool(Tool* p_tool);
public:
	static void init();
	static void terminate();
	static Tool* getTool(std::string p_toolName);
	static Tool* getTool(Sint32 p_toolId);
	static std::vector<Tool*> getToolList() { return m_tools; }
};
