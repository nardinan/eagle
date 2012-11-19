oalinst.exe

mingw\bin\mingw-get.exe update
mingw\bin\mingw-get.exe install mingw g++ msys

move "openal\openal32.lib" "mingw\lib\openal32.lib"
move "openal\al" "mingw\include"
rd "openal"

move "freeglut\bin\freeglut.dll" "mingw\bin\"
move "freeglut\include\GL\*" "mingw\include\GL\"
move "freeglut\lib\*" "mingw\lib\"
rd "freeglut\bin"
rd "freeglut\include\GL"
rd "freeglut\include"
rd "freeglut\lib"
rd "freeglut"

@echo off
ver | find "6.1" > nul
if %ERRORLEVEL%==0 goto winseven

ver | find "5.1" > nul
if %ERRORLEVEL%==0 goto winxp

:winseven
del "%PUBLIC%\Desktop\EagleSDK post install.lnk"
goto exit

:winxp
del "%allusersprofile%\Dekstop\EagleSDK post install.lnk"
goto exit

:exit