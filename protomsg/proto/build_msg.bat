FOR /F "delims=" %%i IN ('dir /B .\\*.proto') DO ..\..\protobuf-2.5.0\lib\x64\release\protoc.exe --cpp_out=..\msg %%~i

pause