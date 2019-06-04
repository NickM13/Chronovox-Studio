#pragma once

#include <functional>

class GFormat {
private:
	// Static variable to keep multiple save/loads 
	// or other actions from occuring, and gives an
	// approximate percent to the loading bar
	static float m_loadPercent;
	static bool m_completed;
	static std::function<void()> m_loadFunction;

public:
	static void setLoadPercent(float p_loadPercent);
	static float getLoadPercent() { return m_loadPercent; }

	static void setLoadFunction(std::function<void()> p_func) { m_loadFunction = p_func; }
	static void checkLoadFunction();
};
