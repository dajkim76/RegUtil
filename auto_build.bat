set VS141COMNTOOLS=c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools

"%VS141COMNTOOLS%/../IDE/devenv" "src/EasyRegistry.sln" /rebuild "Release"
if not "%ERRORLEVEL%" == "0" goto ERROR

cd bin
call _make_setup.bat
if not "%ERRORLEVEL%" == "0" goto ERROR

goto	OK
:ERROR
@echo	"Build Error"
@exit /b 1
pause

:OK
@echo	"All Success"
pause