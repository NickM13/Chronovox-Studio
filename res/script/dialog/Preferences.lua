--[  Matrix Properties Dialog  ]--

dialog.create("Preferences", 300, 360);
dialog.addHeader("Preferences");

dialog.addNumField("FFPS", "Focused FPS", false, 10, 120, 1, 60);
dialog.addNumField("UFPS", "Unfocused FPS", false, 10, 60, 1, 20);
dialog.addDropDown("VIEW", "View Mode", {"Perspective", "Orthogonal", "Isometric"});
dialog.addDropDown("AA", "Anti-aliasing", {"None (1x)", "SSAA Low (2x)", "SSAA Medium (3x)", "SSAA High (4x)"});
dialog.addNumField("NGRID", "Grid Count", false, 1, 256, 1, 32);
dialog.addNumField("SGRID", "Grid Spacing", false, 1, 256, 1, 2);
dialog.addDropDown("AUTORESIZE", "Auto Resize", {"Off", "On"});

dialog.addOption("Update", 0);

dialog.open();
