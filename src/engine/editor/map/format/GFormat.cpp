#include "engine\editor\model\format\GFormat.h"

float GFormat::m_loadPercent = 1;
bool GFormat::m_completed = false;
std::function<void()> GFormat::m_loadFunction = 0;

void GFormat::setLoadPercent(float p_loadPercent) {
	if (p_loadPercent >= 1 && m_loadPercent < 1 && m_loadFunction) {
		m_completed = true;
	}
	m_loadPercent = p_loadPercent;
}

void GFormat::checkLoadFunction() {
	if (m_completed) {
		m_loadFunction();
		m_loadFunction = 0;
		m_completed = false;
	}
}
