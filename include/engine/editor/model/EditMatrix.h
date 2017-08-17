#pragma once

#include "..\command\DeleteCommand.h"
#include "..\command\DrawCommand.h"
#include "..\command\MoveCommand.h"
#include "..\command\NewCommand.h"
#include "..\command\ResizeCommand.h"

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector3.h"

struct EditMatrix
{
private:
	Sint16 m_matrixId;
	Matrix* m_matrix, *m_initMatrix;

	std::vector<Command*> m_recentCommands;
	Uint16 m_commandIndex;


	void saveChanges()
	{
		bool hasChanged = false;
		if(!(m_matrix->getSize() == m_initMatrix->getSize()))
		{
			addCommand(new ResizeCommand(m_matrix, m_initMatrix));
		}
		else if(!(m_matrix->getPos() == m_initMatrix->getPos()))
		{
			addCommand(new MoveCommand(m_matrix, m_initMatrix->getPos(), m_matrix->getPos()));
		}
		else
		{
			DrawCommand* _dcmd = new DrawCommand(m_matrix);
			for(Sint32 x = 0; x < m_initMatrix->getSize().x; x++)
			{
				for(Sint32 y = 0; y < m_initMatrix->getSize().y; y++)
				{
					for(Sint32 z = 0; z < m_initMatrix->getSize().z; z++)
					{
						if(!(m_matrix->getVoxel({x, y, z}) == getVoxel({x, y, z})))
						{
							_dcmd->add({x, y, z}, getVoxel({x, y, z}), m_matrix->getVoxel({x, y, z}));
							hasChanged = true;
						}
					}
				}
			}
			if(hasChanged)
				addCommand(_dcmd);
		}
	}
public:
	EditMatrix()
	{
		m_matrix = 0;
		m_initMatrix = 0;
		m_matrixId = -1;
	}
	~EditMatrix()
	{
		unloadMatrix(false);
		for(Uint16 i = 0; i < m_recentCommands.size(); i++)
			delete m_recentCommands[i];
		m_recentCommands.clear();
	}

	void addCommand(Command* p_com)
	{
		for(Sint16 i = Sint16(m_recentCommands.size()) - 1; i >= m_commandIndex; i--)
		{
			delete m_recentCommands[i];
			m_recentCommands.erase(m_recentCommands.begin() + i);
		}
		m_recentCommands.push_back(p_com);
		m_commandIndex = Sint16(m_recentCommands.size());
	}
	void undo()
	{
		if(m_commandIndex > 0)
		{
			Sint16 id = getId();
			reset();
			if(id != -1 && m_recentCommands[m_commandIndex - 1]->isStaticChange())
			{

			}
			else
				unloadMatrix();
			m_commandIndex--;
			m_recentCommands[m_commandIndex]->undo();
			if(id != -1 && m_recentCommands[m_commandIndex]->isStaticChange())
			{
				delete m_initMatrix;
				if(m_matrix)
					m_initMatrix = new Matrix(*m_matrix);
			}
		}
	}
	void redo()
	{
		if(m_commandIndex < m_recentCommands.size())
		{
			Sint16 id = getId();
			reset();
			if(m_commandIndex < m_recentCommands.size())
			{
				m_recentCommands[m_commandIndex]->redo();
				if(id != -1 && m_recentCommands[m_commandIndex]->isStaticChange())
				{
					delete m_initMatrix;
					m_initMatrix = new Matrix(*m_matrix);
				}
				m_commandIndex++;
			}
		}
	}

	void reset(bool p_saveChanges = true)
	{
		if(m_matrix)
		{
			if(p_saveChanges)
				saveChanges();

			delete m_initMatrix;
			m_initMatrix = new Matrix(*m_matrix);
		}
	}

	void unloadMatrix(bool p_saveChanges = true)
	{
		if(!m_matrix) return;

		if(p_saveChanges && m_matrixId != -1)
			saveChanges();

		delete m_initMatrix;
		m_matrixId = -1;
		m_matrix = 0;
		m_initMatrix = 0;
	}

	void setMatrix(Matrix* m, Sint32 id)
	{
		unloadMatrix();

		m_matrixId = id;
		m_matrix = m;
		m_initMatrix = new Matrix(*m);
	}
	void refreshPos()
	{
		if(m_matrix)
			m_initMatrix->setPosition(m_matrix->getPos());
	}

	Voxel getVoxel(Vector3<Sint32> p_pos)
	{
		if(m_initMatrix)
			return m_initMatrix->getVoxel(p_pos);
		return Voxel();
	}
	Sint16 &getId()
	{
		return m_matrixId;
	}
	bool isSet()
	{
		return (m_matrix);
	}
	Vector3<GLfloat> getPos()
	{
		if(m_initMatrix)
			return m_initMatrix->getPos();
		return {};
	}
	Vector3<Sint32> getSize()
	{
		if(m_initMatrix)
			return m_initMatrix->getSize();
		return {};
	}
	Matrix* getMatrix()
	{
		return m_matrix;
	}
	Matrix* getInitMatrix()
	{
		return m_initMatrix;
	}
};
