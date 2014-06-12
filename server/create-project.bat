@echo off
:: This script is used to create an win vs2012 project.
:: You should modify PROJECT_NAME to work under your environment.
:: Don't change it until you know what you do.

setlocal

if not exist "%cd%\create-project.bat" echo Error!!! You should run it under base root & pause & exit 2	

if not exist "%cd%\tools\sed\sed.exe" echo Error!!! Not found sed.exe & pause & exit 2	

set PROJECT_NAME=MyTestServer

:: create project
echo Create config file:%PROJECT_NAME%.xml
copy /Y "%cd%\serverconfig\Template.xml" "%cd%\serverconfig\%PROJECT_NAME%.xml"

echo Create project:%PROJECT_NAME%
set ROOT_DIR=%cd%
mkdir %PROJECT_NAME%
copy /Y %ROOT_DIR%\Template\Main.cpp %ROOT_DIR%\%PROJECT_NAME%\Main.cpp.tp
copy /Y %ROOT_DIR%\Template\Template.vcxproj %ROOT_DIR%\%PROJECT_NAME%\%PROJECT_NAME%.vcxproj.tp
copy /Y %ROOT_DIR%\Template\Template.vcxproj.filters %ROOT_DIR%\%PROJECT_NAME%\%PROJECT_NAME%.vcxproj.filters.tp
copy /Y %ROOT_DIR%\Template\TemplateClient.cpp %ROOT_DIR%\%PROJECT_NAME%\%PROJECT_NAME%Client.cpp.tp
copy /Y %ROOT_DIR%\Template\TemplateClient.h %ROOT_DIR%\%PROJECT_NAME%\%PROJECT_NAME%Client.h.tp
copy /Y %ROOT_DIR%\Template\TemplateRpcClient.cpp %ROOT_DIR%\%PROJECT_NAME%\%PROJECT_NAME%RpcClient.cpp.tp
copy /Y %ROOT_DIR%\Template\TemplateRpcClient.h %ROOT_DIR%\%PROJECT_NAME%\%PROJECT_NAME%RpcClient.h.tp
copy /Y %ROOT_DIR%\Template\makefile %ROOT_DIR%\%PROJECT_NAME%\makefile.tp

echo Configure all files
cd tools/sed
FOR /F "delims=" %%i IN ('dir /B %ROOT_DIR%\%PROJECT_NAME%\*.tp') DO (
	sed -r "s/Template/%PROJECT_NAME%/g" %ROOT_DIR%\%PROJECT_NAME%\%%~i > %ROOT_DIR%\%PROJECT_NAME%\%%~ni
	del /F /Q %ROOT_DIR%\%PROJECT_NAME%\%%~i
)

echo Done!
pause
