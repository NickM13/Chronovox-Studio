#include "engine\editor\model\tool\MTool.h"

#include <set>

std::vector<Tool*> MTool::m_tools;

void MTool::init() {
	addTool(new MTSelect());

	addTool(new MTMove());
	addTool(new MTResize());

	addTool(new VTAdd());

	addTool(new VTErase());

	addTool(new VTReplace());

	addTool(new VSTEyedrop());

	addTool(new VFTAdd());
	addTool(new VFTErase());
	addTool(new VFTReplace());
}

void MTool::terminate() {
	for(Tool* tool : m_tools)
		delete tool;
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
Sint32 MTool::getToolId(std::string p_toolName) {
	for (size_t i = 0; i < m_tools.size(); i++) {
		if (m_tools.at(i)->getName() == p_toolName) {
			return static_cast<Sint32>(i);
		}
	}
	return 0;
}

Sint32 MTool::getParentCount() {
	std::set<std::string> parents;
	for (Tool* t : m_tools) {
		parents.insert(t->getParent());
	}
	return static_cast<Sint32>(parents.size());
}
