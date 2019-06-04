--[ OBJ Export Dialog ]--

dialog.create("OBJ Export", 360, 220);

dialog.addHeader("Object");
dialog.addNumField("SCALE", "Voxel Scale", true, 0, 1000, 3, 1);
dialog.addNumField("OFFSET", "Position Offset", true, -1000, 1000, 3, 0);
dialog.addHeader("Texture");
dialog.addNumField("TEXELSIZE", "Texel Size", false, 0, 100, 1, 1);
dialog.addTextField("TEXSUBFOLDER", "Texture Subfolder", "materials");

dialog.addOption("Export", 0);

dialog.open();
