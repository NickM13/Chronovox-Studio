#include "engine\editor\model\command\MDrawCommand.h"

MDrawCommand::MDrawCommand(Matrix* m) {
	m_commandType = "MDrawCommand";
	m_matrix = m;
}
void MDrawCommand::terminate() {

}

void MDrawCommand::add(glm::ivec3 p_pos, Voxel p_voxelFrom, Voxel p_voxelTo) {
	m_voxelList.push_back(_Voxel(p_pos, p_voxelFrom, p_voxelTo));
}

void MDrawCommand::undo() {
	for (size_t i = 0; i < m_voxelList.size(); i++) {
		m_matrix->setVoxel(m_voxelList[i].pos, m_voxelList[i].from);
	}
}
void MDrawCommand::redo() {
	for (size_t i = 0; i < m_voxelList.size(); i++) {
		m_matrix->setVoxel(m_voxelList[i].pos, m_voxelList[i].to);
	}
}
