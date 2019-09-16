#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

class MDrawCommand : public Command {
public:
	MDrawCommand(Matrix* m);
	void terminate();

	void add(glm::ivec3 p_pos, Voxel p_voxelFrom, Voxel p_voxelTo);

	void undo();
	void redo();

private:
	Sint32 m_matrix;

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
