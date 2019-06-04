--[  Close Editor Dialog  ]--

dialog.create("Close Editor", 300, 100);
dialog.addText("Exit editor without saving files?");

dialog.addOption("Cancel", -1);
dialog.addOption("Don't Save", 0);
--dialog.addOption("Save", 1);

dialog.open();
