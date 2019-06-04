--[  Matrix Properties Dialog  ]--

dialog.create("Properties", 360, 180);
dialog.addHeader("Matrix Properties");
dialog.addTextField("NAME", "Name", "");
dialog.addTextField("PARENT", "Parent", "");

dialog.addNumField("POS", "Position", true, -256, 256, 3, 0);
dialog.addNumField("SIZE", "Size", false, 0, 256, 3, 1);

dialog.addOption("Cancel", -1);
dialog.addOption("Update", 0);

dialog.open();
