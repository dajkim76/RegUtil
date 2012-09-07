"%VS90COMNTOOLS%/../IDE/devenv" "src/EasyRegistry.sln" /rebuild "Release"
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