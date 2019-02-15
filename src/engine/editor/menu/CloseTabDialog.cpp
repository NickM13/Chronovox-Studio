#include "engine\editor\menu\CloseTabDialog.h"

CloseTabDialog::CloseTabDialog() {
	CText* desc = new CText("DESC", "Save changes made to file?", { 12, -4 }, {}, Alignment::ALIGN_LEFT, Color());
	m_dialog = new CDialog("DIALOG_CLOSE_TAB", "Nick's Voxel Editor", {}, { desc->getSize().x + 80, desc->getSize().y + 40 });
	m_dialog->addComponent(desc, Component::Anchor::MIDDLE_LEFT);
	m_dialog->addOption("Cancel", "", [&]() {
		m_dialog->setActive(false);
	});
	m_dialog->addOption("Don't Save", "", [&]() {
		m_dialog->setActive(false);
	});
	m_dialog->addOption("Save", "", [&]() {
		m_dialog->setActive(false);
	});
}
CDialog* CloseTabDialog::getDialog() {
	return m_dialog;
}
