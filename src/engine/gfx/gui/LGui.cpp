#include "engine\gfx\gui\LGui.h"
#include "engine\utils\global\GLua.h"

Container* Gui::m_mainContainer = 0;
std::vector<CDialog*> Gui::m_dialogList;
std::vector<Component*> Gui::m_compBuildList;
Sint32 Gui::m_cHeight;
Sint32 Gui::m_actionWidth;
Sint32 Gui::m_actionSpacing = 8;

void Gui::init() {
	m_mainContainer = new Container("GUI", { 0, 0 }, GScreen::getScreenSize(), []() { return true; });

	auto nDialog = GLua::getGlobal().beginNamespace("dialog");
	nDialog.addFunction("create",		&create);
	nDialog.addFunction("addHeader",	&addHeader);
	nDialog.addFunction("addText",		&addText);
	nDialog.addFunction("addTextField", &addTextField);
	nDialog.addFunction("addTextArea",	&addTextArea);
	nDialog.addFunction("addNumField",	&addNumField);
	nDialog.addFunction("addTextFile",	&addTextFile);
	nDialog.addFunction("addOption",	&addOption);
	nDialog.addFunction("open",			&open);
}

Container* Gui::getContainer() {
	return m_mainContainer;
}

CDialog* Gui::openDialog(std::string p_filename) {
	GLua::loadScriptFile("dialog\\" + p_filename);
	if (!m_dialogList.empty())	return m_dialogList.back();
	else						return 0;
}
void Gui::closeTopDialog() {
	delete m_dialogList.back();
	m_dialogList.pop_back();
	if (m_dialogList.empty()) {
		m_mainContainer->closeDialog();
	}
	else {
		m_mainContainer->openDialog(m_dialogList.back());
	}
}

void Gui::create(std::string p_title, Sint32 width, Sint32 height) {
	m_dialogList.push_back(new CDialog(p_title, p_title, {}, { width, height }));
	m_compBuildList.clear();
	m_actionWidth = width * 2 / 3;
	m_cHeight = 0;
}
void Gui::addHeader(std::string p_text) {
	m_dialogList.back()->addComponent(new CText(p_text, p_text, { 0, m_cHeight }, {  }, Alignment::ALIGN_LEFT, "dialogText2", "Header"));
	m_cHeight += Font::getFont("Header")->m_spacing + 10;
}
void Gui::addText(std::string p_text) {
	m_dialogList.back()->addComponent(new CText(p_text, p_text, { 0, m_cHeight }, {  }, Alignment::ALIGN_LEFT, "dialogText1", "Body"));
	m_cHeight += Font::getFont("Body")->m_spacing + 8;
}
void Gui::addLineBreak() {

}
void Gui::addTextField(std::string p_compName, std::string p_title, std::string p_default) {
	auto textfield = new TextField(p_compName, p_title, { 0, m_cHeight }, { m_actionWidth - m_actionSpacing, 1 });
	textfield->setTitle(p_default);
	m_dialogList.back()->addComponent(new CText(p_compName + "_TEXT", p_title, { 0, m_cHeight }, {  }, Alignment::ALIGN_LEFT, "dialogText1"));
	m_dialogList.back()->addComponent(textfield, Component::Anchor::TOP_RIGHT);
	m_cHeight += Font::getFont("Body")->m_spacing + 8;
	m_compBuildList.push_back(textfield);
}
void Gui::addTextArea(std::string p_compName, Sint32 p_height) {
	auto textfield = new TextField(p_compName, "", { 0, m_cHeight }, { m_dialogList.back()->getSize().x, p_height }, 0, false);
	textfield->setTitle("");
	m_dialogList.back()->addComponent(textfield, Component::Anchor::TOP_CENTER);
	m_cHeight += Font::getFont("Body")->m_spacing * p_height + 8;
	m_compBuildList.push_back(textfield);
}
void Gui::addNumField(std::string p_compName, std::string p_title, bool p_isFloat, Sint32 p_lowerBound, Sint32 p_upperBound, Sint32 p_count, Sint32 p_default) {
	m_dialogList.back()->addComponent(new CText(p_compName + "_TEXT", p_title, { 0, m_cHeight }, {  }, Alignment::ALIGN_LEFT, "dialogText1"));
	for (Sint32 i = p_count - 1; i >= 0; i--) {
		auto numfield = new NumberField(p_compName + (p_count > 1 ? Util::numToStringInt(i) : ""), "",
			{ -i * (m_actionWidth / p_count), m_cHeight },
			{ m_actionWidth / p_count - m_actionSpacing, 1 },
			Vector2<GLfloat>(p_lowerBound, p_upperBound),
			p_isFloat ? NumberField::NumType::FLOAT : NumberField::NumType::INT);
		numfield->setValue(p_default);
		m_dialogList.back()->addComponent(numfield, Component::Anchor::TOP_RIGHT);
		m_compBuildList.push_back(numfield);
	}
	m_cHeight += Font::getFont("Body")->m_spacing + 8;
}
void Gui::addTextFile(std::string p_compName, std::string p_filename) {
	m_dialogList.back()->addComponent(new CTextFile(p_compName, p_filename, { 0, m_cHeight }, {}, Alignment::ALIGN_LEFT, "dialogText1"));
}
void Gui::addOption(std::string p_option, Sint32 p_functionIndex) {
	m_dialogList.back()->addOption(p_option, "", p_functionIndex);
}
void Gui::open() {
	for (size_t i = 0; i < m_compBuildList.size(); i++) {
		if (i != 0)
			m_compBuildList.at(i)->setPrevComponent(m_compBuildList.at(i - 1));
		if (i != m_compBuildList.size() - 1)
			m_compBuildList.at(i)->setNextComponent(m_compBuildList.at(i + 1));
	}
	m_mainContainer->openDialog(m_dialogList.back());
}

void Gui::setLoading(GLfloat p_loadPercent) {
	m_mainContainer->setLoading(p_loadPercent);
}

void Gui::resize() {
	m_mainContainer->setSize(GScreen::getScreenSize());
}

void Gui::input(Sint8& p_iFlags) {
	m_mainContainer->input(p_iFlags);
}

void Gui::update(GLfloat p_deltaTime) {
	m_mainContainer->update(p_deltaTime);
	GGui::update();
	if (!m_dialogList.empty() && !m_dialogList.back()->isActive()) {
		closeTopDialog();
	}
}

void Gui::render() {
	m_mainContainer->render();

	if (!GScreen::isMaximized()) {
		Sint32 sx = m_mainContainer->getSize().x, sy = m_mainContainer->getSize().y;

		GBuffer::setTexture(0);
		GBuffer::setColor(Component::getElementColor(GScreen::isFocused() ? "windowBorderFocused" : "windowBorderUnfocused"));

		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(1, 0);
		GBuffer::addVertexQuad(1, sy);
		GBuffer::addVertexQuad(0, sy);

		GBuffer::addVertexQuad(sx, 0);
		GBuffer::addVertexQuad(sx - 1, 0);
		GBuffer::addVertexQuad(sx - 1, sy);
		GBuffer::addVertexQuad(sx, sy);

		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(sx, 0);
		GBuffer::addVertexQuad(sx, 1);
		GBuffer::addVertexQuad(0, 1);

		GBuffer::addVertexQuad(0, sy);
		GBuffer::addVertexQuad(sx, sy);
		GBuffer::addVertexQuad(sx, sy - 1);
		GBuffer::addVertexQuad(0, sy - 1);
	}
}
