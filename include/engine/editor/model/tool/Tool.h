#pragma once
#include "..\EditMatrix.h"
#include "..\Matrix.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\mesh\MMesh.h"
#include "engine\utils\math\ModelMath.h"

class Tool {
public:
	enum class ToolType {
		VOXEL,
		MATRIX
	};
protected:
	static EditMatrix* m_editMatrix;
	static Sint32* m_subTool;
	static glm::vec4 m_colorSelect, m_colorError;

	ToolType m_toolType;
	std::string m_toolName, m_toolDesc;
	Texture* m_toolIcon;
	bool m_hasSubmenu = true;
public:
	static void init(EditMatrix* p_editMatrix, Sint32* p_subTool);

	virtual void enable();
	virtual void disable();

	std::string getName() const			{ return m_toolName; }
	std::string getDescription() const	{ return m_toolDesc; }
	ToolType getType() const			{ return m_toolType; }
	Texture* getIcon() const			{ return m_toolIcon; }
	bool hasSubmenu() const				{ return m_hasSubmenu; }
};
