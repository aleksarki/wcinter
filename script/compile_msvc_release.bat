@cd ..\
@mkdir build\ 2>nul
@cd build\
@cmake ..\ -G "Visual Studio 17 2022" -A x64
@cmake --build .\ --config Release
@del ..\bin\*.* /Q
@robocopy ..\bin\Release ..\bin\ /MOVE
@echo All files are placed in "src\parser\bin\"
@cd ..\script\
@pause
