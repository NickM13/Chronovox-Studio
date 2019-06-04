--[  Resize Matrix Dialog  ]--

dialog.create("Resize Matrix", 360, 100);
dialog.addHeader("Resize Matrix");
dialog.addNumField("SIZE", "Size", false, 0, 256, 3, 1);

dialog.addOption("Update", 0);

dialog.open();
