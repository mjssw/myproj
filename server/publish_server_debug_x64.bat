copy /Y .\netconfig.xml .\publish\
mkdir .\publish\serverconfig
xcopy /Y .\serverconfig .\publish\serverconfig /s
mkdir .\publish\Scripts
xcopy /Y .\Scripts .\publish\Scripts /s

mkdir .\publish\server
copy /Y ..\third_part\log4cxx\lib\Debug\log4cxx.dll .\publish\server\
xcopy /Y .\x64\Debug .\publish\server /s
