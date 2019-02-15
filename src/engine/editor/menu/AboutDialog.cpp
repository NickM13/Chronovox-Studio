#include "engine\editor\menu\AboutDialog.h"

AboutDialog::AboutDialog() {
	CTextFile* aboutFile = new CTextFile("TEXTFILE_ABOUT", LDirectory::getProjectPath() + "res\\doc\\ABOUT", { 20, 0 }, { 0, 0 }, Alignment::ALIGN_LEFT, Color(1, 1, 1));
	aboutFile->addInputString(GScreen::m_appVersion);
	m_dialog = new CDialog("DIALOG_ABOUT", "About Nick's Voxel Editor", {}, { aboutFile->getSize().x + 80, aboutFile->getSize().y + 40 });
	m_dialog->addComponent(aboutFile, Component::Anchor::MIDDLE_LEFT);
	m_dialog->addOption("Close", "", [&]() {
		m_dialog->setActive(false);
	});
}
CDialog* AboutDialog::getDialog() {
	return m_dialog;
}
