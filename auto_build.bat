"%VS90COMNTOOLS%/../IDE/devenv" "src/RegUtil.sln" /rebuild "Release"
@echo off
if not "%ERRORLEVEL%" == "0" goto ERROR
@echo on


goto	OK
:ERROR
@echo	"Build Error"
@exit /b 1
pause

:OK
@echo	"All Success"
pause