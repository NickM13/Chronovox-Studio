rd /S /Q bin\Debug\res
mkdir bin\Debug\res
xcopy res bin\Debug\res /E
rd /S /Q bin\Release\res
mkdir bin\Release\res
xcopy res bin\Release\res /E
rd /S /Q bin\ReleaseConsole\res
mkdir bin\ReleaseConsole\res
xcopy res bin\ReleaseConsole\res /E