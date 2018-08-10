#include "engine\gfx\model\MModelObj.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\directory\LDirectory.h"

std::map<std::string, ModelObj*> MModelObj::m_modelObjs;

void MModelObj::loadModelObj(std::string p_filePath) {
	std::cout << p_filePath << std::endl;
	std::string path = LDirectory::getProjectPath().append("res\\obj\\").append(p_filePath);
	ModelObj* modelObj = new ModelObj(p_filePath);
	if(modelObj->loadObjFile(path))
		m_modelObjs.insert({p_filePath, modelObj});
	else
		std::cout << "Error::ModelObj::Could not find file \"" << path << "\"" << std::endl;
}

void MModelObj::init() {
	m_modelObjs.insert({"NULL", new ModelObj("NULL")});
	size_t rootLen = std::string(LDirectory::getProjectPath() + "res\\obj\\").length();
	for(std::string path : LDirectory::getFilesInDirectory(LDirectory::getProjectPath() + "res\\obj\\", ".obj"))
		loadModelObj(path.substr(rootLen));
}

void MModelObj::terminate() {
	for(std::pair<std::string, ModelObj*> t : m_modelObjs)
		delete t.second;
	m_modelObjs.clear();
}

ModelObj* MModelObj::get(std::string p_filePath) {
	if(m_modelObjs.find(p_filePath) == m_modelObjs.end()) {
		return m_modelObjs.at("NULL");
	}
	return m_modelObjs.at(p_filePath);
}
