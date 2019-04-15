#pragma once
#include "..\EditMatrix.h"
#include "..\Matrix.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\mesh\MMesh.h"
#include "engine\utils\math\ModelMath.h"
#include <functional>

class Tool {
public:
	enum class ToolType {
		VOXEL,
		MATRIX
	};
protected:
	static EditMatrix* m_editMatrix;
	static glm::vec4 m_colorSelect, m_colorError;

	ToolType m_toolType = ToolType::VOXEL; // VOXEL or MATRIX
	std::string m_parent = ""; // For Radio sublists
	std::string m_toolName = "", m_toolDesc = "";
	Texture* m_toolIcon = 0;
	GKey::KeyBind m_keyBind = {};
public:
	static void init(EditMatrix* p_editMatrix);

	virtual void enable();
	virtual void disable();

	std::string getParent() const				{ return m_parent; }
	std::string getName() const					{ return m_toolName; }
	std::string getDescription() const			{ return m_toolDesc; }
	ToolType getType() const					{ return m_toolType; }
	Texture* getIcon() const					{ return m_toolIcon; }
	GKey::KeyBind getKeyBind() const			{ return m_keyBind; }
};
