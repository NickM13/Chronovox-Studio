#include "engine\gfx\gui\component\field\TextFile.h"

CTextFile::CTextFile(std::string p_compName, std::string p_src, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, std::string p_fontColor)
	: Component(p_compName, "", p_pos, p_size) {
	m_align = p_align;
	m_color = p_fontColor;

	m_variableInputIndex = 0;
	m_lines.clear();
	std::ifstream file(p_src);
	if (file.good()) {
		std::string line("");
		while (!file.eof()) {
			std::getline(file, line);
			for (Sint32 i = 0; i < (Sint32)line.length() - 1; i++) {
				if (line.at(i) == '%') {
					switch (line.at(i + 1)) {
					case 's':
						m_variableInputLoc.push_back(Vector2<Sint32>(static_cast<Sint32>(m_lines.size()), i));
						line = line.substr(0, i) + line.substr(i + 2);
						break;
					}
				}
			}
			m_lines.push_back(line);
			line = "";
		}
	}
	file.close();
	Sint32 _max = 0;
	for (std::string line : m_lines) {
		_max = std::fmaxf(_max, Font::getMessageWidth(line).x);
	}
	m_sizeInit = m_size = Vector2<Sint32>(_max, m_lines.size() * Font::getSpacingHeight());
	updateTitle();
}

void CTextFile::updateTitle() {
	m_title = "";
	for (Sint32 i = 0; i < (Sint32)m_lines.size(); i++) {
		m_title += m_lines[i];
		if (i < (Sint32)m_lines.size() - 1) {
			m_title += "\n";
		}
	}
}

void CTextFile::addInputString(std::string p_str) {
	if (m_variableInputIndex < (Sint32)m_variableInputLoc.size()) {
		m_lines.at(m_variableInputLoc.at(m_variableInputIndex).x).insert(m_variableInputLoc.at(m_variableInputIndex).y, p_str);
		m_variableInputIndex++;
	}
	updateTitle();
}

std::string CTextFile::getTitle() {
	return m_title;
}

void CTextFile::render() {
	GBuffer::setColor(getElementColor(m_color));
	Font::setAlignment(m_align);
	Sint32 y = m_pos.y + Font::getSpacingHeight() / 2;
	for (Sint32 i = 0; i < (Sint32)m_lines.size(); i++) {
		switch (m_align) {
		case ALIGN_LEFT:
			Font::print(m_lines.at(i), m_pos.x, y);
			break;
		case ALIGN_CENTER:
			Font::print(m_lines.at(i), m_pos.x + m_size.x / 2, y);
			break;
		case ALIGN_RIGHT:
			Font::print(m_lines.at(i), m_pos.x + m_size.x, y);
			break;
		}
		y += Font::getSpacingHeight();
	}
}
