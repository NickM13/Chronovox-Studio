--[  Create Matrix Dialog  ]--

dialog.create("Create Matrix", 360, 130);
dialog.addHeader("Create Matrix");
dialog.addTextField("MATRIXNAME", "Matrix Name", "Matrix");
dialog.addNumField("SIZE", "Matrix Size", false, 0, 256, 3, 10);

dialog.addOption("Create", 0);

dialog.open();
