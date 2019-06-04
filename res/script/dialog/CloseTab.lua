--[  Close Tab Dialog  ]--

dialog.create("Close Tab", 300, 100);
dialog.addText("Save changes to the file before closing?");

dialog.addOption("Cancel", -1);
dialog.addOption("Don't Save", 0);
dialog.addOption("Save", 1);

dialog.open();
