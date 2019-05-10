#include "engine\editor\menu\CloseEditorDialog.h"

CloseEditorDialog::CloseEditorDialog() {
	CText* desc = new CText("DESC", "You have unsaved changes.\nAre you sure you want to leave?", { 12, 20 }, {}, Alignment::ALIGN_LEFT, "textLight");
	m_dialog = new CDialog("DIALOG_CLOSE_EDITOR", "Exitting...", {}, { desc->getSize().x + 80, desc->getSize().y + 40 });
	m_dialog->addComponent(desc, Component::Anchor::TOP_LEFT);
	m_dialog->addOption("Cancel", "", [&]() {
		m_dialog->setActive(false);
	});
	m_dialog->addOption("Exit Without Saving", "", [&]() {
		m_dialog->setActive(false);
	});
}
CDialog* CloseEditorDialog::getDialog() {
	return m_dialog;
}
