if exist EasyRegistrySetup.exe del EasyRegistrySetup.exe

:: version.ini ����

".\NSIS_UNICODE\makensis.exe" EasyRegistry.nsi
if not "%ERRORLEVEL%" == "0" goto ERROR


goto	OK
:ERROR
echo	"_make_setup.bat failed"
exit /b 1

:OK
