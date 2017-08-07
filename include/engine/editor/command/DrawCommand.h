#pragma once

#include "Command.h"
#include "..\model\Matrix.h"

struct DrawCommand : public Command
{
public:
	DrawCommand(Matrix* m)
	{
		m_matrix = m;
	}
	void add(Vector3<Sint32> p_pos, Voxel p_voxelFrom, Voxel p_voxelTo)
	{
		m_voxelList.push_back(_Voxel(p_pos, p_voxelFrom, p_voxelTo));
	}
	void undo()
	{
		for(Uint16 i = 0; i < m_voxelList.size(); i++)
		{
			m_matrix->setVoxel(m_voxelList[i].pos, m_voxelList[i].from);
		}
	}
	void redo()
	{
		for(Uint16 i = 0; i < m_voxelList.size(); i++)
		{
			m_matrix->setVoxel(m_voxelList[i].pos, m_voxelList[i].to);
		}
	}

private:
	Matrix* m_matrix;

	struct _Voxel
	{
		Vector3<Sint32> pos;
		Voxel from;
		Voxel to;

		_Voxel(Vector3<Sint32> p_pos, Voxel p_from, Voxel p_to)
		{
			pos = p_pos;
			from = p_from;
			to = p_to;
		}
	};
	std::vector<_Voxel> m_voxelList;
};
