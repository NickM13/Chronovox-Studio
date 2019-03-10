#include "engine\editor\model\menu\NewModelDialog.h"
#include "engine\editor\camera\Camera.h"

NewModelDialog::NewModelDialog() {
	m_dialog = new CDialog("DIALOG_NEW_MATRIX", "Create New Matrix", {}, { 250, 120 });
	
	m_dialog->addComponent(new TextField("MATRIXNAME", "Matrix Name", { 0, 12 }, { 200, 1 }, 0), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("WIDTH")->setState(1); });
	m_dialog->addComponent(new NumberField("WIDTH", "Width: ", { 50, 40 }, { 100, 1 }, { 0, 128 }, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("HEIGHT")->setState(1); });
	m_dialog->addComponent(new NumberField("HEIGHT", "Height: ", { 50, 64 }, { 100, 1 }, { 0, 128 }, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("DEPTH")->setState(1); });
	m_dialog->addComponent(new NumberField("DEPTH", "Depth: ", { 50, 88 }, { 100, 1 }, { 0, 128 }, NumberField::NumType::INT), Component::Anchor::TOP_CENTER);

	m_dialog->addOption("Back", "", [&]() {
		m_dialog->setActive(false);
	});
	m_dialog->addOption("Create", "", [&]() {
		m_dialog->setActive(false);
	});
}
CDialog* NewModelDialog::getDialog() {
	return m_dialog;
}
