--[  About Dialog  ]--

dialog.create("About", 260, 170);
dialog.addHeader("About Chronovox Studio");
dialog.addTextFile("ABOUT", "res\\doc\\ABOUT");

dialog.addOption("Close", -1);

dialog.open();
