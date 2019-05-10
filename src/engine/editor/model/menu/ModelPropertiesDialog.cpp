#include "engine\editor\model\menu\ModelPropertiesDialog.h"

ModelPropertiesDialog::ModelPropertiesDialog() {
	m_dialog = new CDialog("DIALOG_PROPERTIES", "Matrix Properties", {}, { 250, 160 });

	m_dialog->addComponent(new TextField("MATRIXNAME", "Matrix Name", { 0, 8 }, { 200, 1 }, 0), Component::Anchor::TOP_CENTER);
	m_dialog->addComponent(new TextField("PARENTNAME", "Parent Matrix", { 0, 32 }, { 200, 1 }, 0), Component::Anchor::TOP_CENTER);
	m_dialog->addComponent(new CText("TEXT_OFFSET", "Position", { -58, 66 }, { 0, 0 }, Alignment::ALIGN_CENTER, "textLight"),
		Component::Anchor::TOP_CENTER);
	m_dialog->addComponent(new NumberField("OFFX", "x: ", { -58, 80 }, { 30, 1 }, { -64, 64 }, NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("OFFY")->setState(1); });
	m_dialog->addComponent(new NumberField("OFFY", "y: ", { -58, 104 }, { 30, 1 }, { -64, 64 }, NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("OFFZ")->setState(1); });
	m_dialog->addComponent(new NumberField("OFFZ", "z: ", { -58, 128 }, { 30, 1 }, { -64, 64 }, NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("WIDTH")->setState(1); });
	m_dialog->addComponent(new CText("TEXT_SIZE", "Size", { 58, 66 }, { 0, 0 }, Alignment::ALIGN_CENTER, "textLight"),
		Component::Anchor::TOP_CENTER);
	m_dialog->addComponent(new NumberField("WIDTH", "x: ", { 58, 80 }, { 30, 1 }, { 1, 128 }, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("HEIGHT")->setState(1); });
	m_dialog->addComponent(new NumberField("HEIGHT", "y: ", { 58, 104 }, { 30, 1 }, { 1, 128 }, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([&]() { m_dialog->findComponent("DEPTH")->setState(1); });
	m_dialog->addComponent(new NumberField("DEPTH", "z: ", { 58, 128 }, { 30, 1 }, { 1, 128 }, NumberField::NumType::INT), Component::Anchor::TOP_CENTER);

	m_dialog->addOption("Cancel", "", [&]() {
		m_dialog->setActive(false);
	});
	m_dialog->addOption("Update", "", [&]() {
		m_dialog->setActive(false);
	});
}
CDialog* ModelPropertiesDialog::getDialog() {
	return m_dialog;
}
