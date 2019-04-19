@REM Build using the Visual Studio compiler. Run vcvarsall.bat to setup paths and env variables for building programs from command line
@echo off
IF NOT EXIST build mkdir build
cl /nologo /Zi /MD /I libs\glfw\include *.cpp /Febuild/spfcurve.exe /Fobuild/ /link /LIBPATH:libs\glfw\lib-vc2015-32 glfw3.lib opengl32.lib gdi32.lib shell32.lib user32.lib