#pragma once
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Macros.h"

#include <string>

class Texture {
private:
	Sint32 m_texId;
	std::string m_name;
	Vector2<Sint32> m_size;

public:
	Texture(std::string p_name = "", Sint32 p_texId = -1, Vector2<Sint32> p_size = {}) {
		m_texId = p_texId;
		m_name = p_name;
		m_size = p_size;
	}
	Sint32 getId() const {
		return m_texId;
	}
	std::string getName() const {
		return m_name;
	}
	Vector2<Sint32> getSize() const {
		return m_size;
	}
	bool operator==(const Texture p_tex) const {
		return (m_name == p_tex.m_name);
	}
	bool operator<(const Texture p_tex) const {
		return (m_texId < p_tex.getId());
	}
};
