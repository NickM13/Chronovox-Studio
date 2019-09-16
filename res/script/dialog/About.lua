--[  About Dialog  ]--

dialog.create("About", 320, 170);
dialog.addHeader("About Chronovox Studio");
dialog.addTextFile("ABOUT", "res\\doc\\ABOUT");

dialog.addOption("Close", -1);
dialog.addOption("Website", 0);

dialog.open();
