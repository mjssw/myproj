set ROOT_DIR=%cd%
FOR /F "delims=" %%i IN ('dir /B/AD') DO (
	cd %ROOT_DIR%\%%i
	del /F/Q libmysql.dll
	del /F/Q log4cxx.dll
	del /F/Q netconfig.xml
	rd /s/q serverconfig
	cd ..
)
pause

