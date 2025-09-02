#include "engine\editor\map\EditChunk.h"

EditChunk::EditChunk() {
	m_chunk = 0;
	m_initChunk = 0;
	m_chunkId = -1;
	m_commandListIsOpen = true;
	m_commandChaining = false;
	m_changed = false;
}
EditChunk::~EditChunk() {
	clearChunk(false);
	clearCommands();
}

void EditChunk::setCommandListOpen(bool p_isOpen) {
	m_commandListIsOpen = p_isOpen;
}
void EditChunk::setCommandChaining(bool p_chaining) {
	if (p_chaining) {
		clearForwardCommands();
		m_recentCommands.push_back(std::vector<Command*>());
		m_commandIndex = Sint16(m_recentCommands.size());
	}
	else if (m_recentCommands.back().empty()) {
		m_recentCommands.pop_back();
		m_commandIndex--;
	}
	m_commandChaining = p_chaining;
}

void EditChunk::addCommand(Command* p_com) {
	if (!m_commandListIsOpen) return;
	clearForwardCommands();
	if (!m_commandChaining || m_recentCommands.empty()) {
		m_recentCommands.push_back(std::vector<Command*>());
	}
	m_recentCommands.back().push_back(p_com);
	m_commandIndex = Sint16(m_recentCommands.size());
	m_changed = true;
}
void EditChunk::undo() {
	if (m_commandIndex > 0) {
		Sint16 id = getId();
		clearChunk();
		//clearChunk(); // Was in an id==-1 || command.isStatic
		for (Sint32 i = static_cast<Sint32>(m_recentCommands[m_commandIndex - 1].size()) - 1; i >= 0; i--) {
			m_recentCommands[m_commandIndex - 1][i]->undo();
		}
		m_commandIndex--;
		m_changed = true;
		/*
		if (id != -1 && m_recentCommands[m_commandIndex]->isStaticChange()) {
			delete m_initChunk;
			if (m_chunk)
				m_initChunk = new Chunk(*m_chunk);
		}
		*/
	}
}
void EditChunk::redo() {
	if (m_commandIndex < m_recentCommands.size()) {
		Sint16 id = getId();
		clearChunk();
		if (m_commandIndex < m_recentCommands.size()) {
			for (Command* c : m_recentCommands[m_commandIndex]) {
				c->redo();
			}
			m_commandIndex++;
			m_changed = true;
		}
	}
}
void EditChunk::clearCommands() {
	for (Uint16 i = 0; i < m_recentCommands.size(); i++) {
		for (Uint16 j = 0; j < m_recentCommands[i].size(); j++) {
			m_recentCommands[i][j]->terminate();
			delete m_recentCommands[i][j];
		}
		m_recentCommands[i].clear();
	}
	m_recentCommands.clear();
	m_commandIndex = 0;
}
void EditChunk::clearForwardCommands() {
	for (Sint16 i = Sint16(m_recentCommands.size()) - 1; i >= m_commandIndex; i--) {
		for (Uint16 j = 0; j < m_recentCommands[i].size(); j++) {
			m_recentCommands[i][j]->terminate();
			delete m_recentCommands[i][j];
		}
		m_recentCommands.erase(m_recentCommands.begin() + i);
	}
}

void EditChunk::reset(bool p_saveChanges) {
	if (m_chunk) {
		if (p_saveChanges) {
			saveChanges();
		}
		delete m_initChunk;
		m_initChunk = new Chunk(*m_chunk);
	}
}

void EditChunk::setChunk(Chunk* m, Sint32 id) {
	clearChunk();

	m_chunkId = id;
	m_chunk = m;
	m_initChunk = new Chunk(*m);
}
void EditChunk::clearChunk(bool p_saveChanges) {
	if (!m_chunk) return;

	if (p_saveChanges && m_chunkId != -1) {
		saveChanges();
	}

	delete m_initChunk;
	m_chunkId = -1;
	m_chunk = 0;
	m_initChunk = 0;
}
void EditChunk::saveChanges() {
	std::vector<Command*> cmdList;

	if (m_chunk->getName() != m_initChunk->getName()) {
		cmdList.push_back(new MRenameCommand(m_chunk, m_initChunk->getName(), m_chunk->getName()));
	}
	if (m_chunk->getPos() != m_initChunk->getPos()) {
		cmdList.push_back(new MMoveCommand(m_chunk, m_initChunk->getPos(), m_chunk->getPos()));
	}
	if (m_chunk->getSize() != m_initChunk->getSize()) {
		cmdList.push_back(new MResizeCommand(m_chunk, m_initChunk));
	}
	else {
		bool hasChanged = false;
		MDrawCommand* _dcmd = new MDrawCommand(m_chunk);
		for (Sint32 x = 0; x < m_initChunk->getSize().x; x++) {
			for (Sint32 y = 0; y < m_initChunk->getSize().y; y++) {
				for (Sint32 z = 0; z < m_initChunk->getSize().z; z++) {
					if (!(m_chunk->getVoxel({ x, y, z }) == getVoxel({ x, y, z }))) {
						_dcmd->add({ x, y, z }, getVoxel({ x, y, z }), m_chunk->getVoxel({ x, y, z }));
						hasChanged = true;
					}
				}
			}
		}
		if (hasChanged) {
			cmdList.push_back(_dcmd);
		}
	}

	if (!cmdList.empty()) {
		setCommandChaining(true);
		for (Command* c : cmdList) addCommand(c);
		setCommandChaining(false);
	}
}
void EditChunk::refreshPos() {
	if (m_chunk) {
		m_initChunk->setPosition(m_chunk->getPos());
	}
}

Voxel EditChunk::getVoxel(glm::ivec3 p_pos) {
	if (m_initChunk) {
		return m_initChunk->getVoxel(p_pos);
	}
	return Voxel();
}
Sint16& EditChunk::getId() {
	return m_chunkId;
}
bool EditChunk::isSet() {
	return (m_chunk);
}
glm::vec3 EditChunk::getPos() {
	if (m_initChunk) {
		return m_initChunk->getPos();
	}
	return {};
}
glm::ivec3 EditChunk::getSize() {
	if (m_initChunk) {
		return m_initChunk->getSize();
	}
	return {};
}
Chunk* EditChunk::getChunk() {
	return m_chunk;
}
Chunk* EditChunk::getInitChunk() {
	return m_initChunk;
}
