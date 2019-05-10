rmdir bin\Debug\res /Q /S
mkdir bin\Debug\res
xcopy res bin\Debug\res /d /E /Y
rmdir bin\Release\res /Q /S
mkdir bin\Release\res
xcopy res bin\Release\res /d /E /Y
rmdir bin\ReleaseConsole\res /Q /S
mkdir bin\ReleaseConsole\res
xcopy res bin\ReleaseConsole\res /d /E /Y