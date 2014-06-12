FOR /F "delims=" %%i IN ('dir /B .\\*.proto') DO ..\..\third_part\protobuf\lib\protoc.exe --cpp_out=..\msg %%~i

pause