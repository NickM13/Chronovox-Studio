#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

class MDrawCommand : public Command {
public:
	MDrawCommand(Matrix* m) {
		m_commandType = "MDrawCommand";
		m_matrix = m;
	}
	void add(glm::ivec3 p_pos, Voxel p_voxelFrom, Voxel p_voxelTo) {
		m_voxelList.push_back(_Voxel(p_pos, p_voxelFrom, p_voxelTo));
	}
	void undo() {
		for(Uint16 i = 0; i < m_voxelList.size(); i++) {
			m_matrix->setVoxel(m_voxelList[i].pos, m_voxelList[i].from);
		}
	}
	void redo() {
		for(Uint16 i = 0; i < m_voxelList.size(); i++) {
			m_matrix->setVoxel(m_voxelList[i].pos, m_voxelList[i].to);
		}
	}

private:
	Matrix* m_matrix;

	struct _Voxel {
		glm::ivec3 pos;
		Voxel from;
		Voxel to;

		_Voxel(glm::ivec3 p_pos, Voxel p_from, Voxel p_to) {
			pos = p_pos;
			from = p_from;
			to = p_to;
		}
	};
	std::vector<_Voxel> m_voxelList;
};
