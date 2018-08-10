#pragma once

#include "ModelObj.h"
#include <map>

// Model Object manager
class MModelObj {
private:
	static std::map<std::string, ModelObj*> m_modelObjs;

	static void loadModelObj(std::string p_texturePath);
public:
	static void init();
	static void terminate();
	static ModelObj* get(std::string p_texturePath);
};
