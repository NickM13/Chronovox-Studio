#include "engine\editor\model\tool\MTool.h"

std::vector<Tool*> MTool::m_tools;

void MTool::init() {
	addTool(new VTAdd());
	addTool(new VTReplace());
	addTool(new VTErase());
	addTool(new VTEyedrop());
	addTool(new MTSelect());
}

void MTool::terminate() {
	m_tools.clear();
}

void MTool::addTool(Tool* p_tool) {
	m_tools.push_back(p_tool);
}

Tool* MTool::getTool(std::string p_toolName) {
	for(Tool* tool : m_tools)
		if(tool->getName() == p_toolName)
			return tool;
	return 0;
}
Tool* MTool::getTool(Sint32 p_toolId) {
	return m_tools[p_toolId];
}
