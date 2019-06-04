--[ STL Export Dialog ]--

dialog.create("STL Export", 360, 120);

dialog.addHeader("Object");
dialog.addNumField("SCALE", "Voxel Scale", true, 0, 1000, 3, 1);
dialog.addNumField("OFFSET", "Position Offset", true, -1000, 1000, 3, 0);

dialog.addOption("Export", 0);

dialog.open();
