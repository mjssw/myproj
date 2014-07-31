copy /Y .\netconfig.xml .\publish\
mkdir .\publish\serverconfig
xcopy /Y .\serverconfig .\publish\serverconfig /s
mkdir .\publish\Scripts
xcopy /Y .\Scripts .\publish\Scripts /s

xcopy /Y .\x64\Release .\publish /s

copy /Y .\run_all.bat .\publish\
copy /Y .\run_only_login.bat .\publish\
copy /Y .\run_only_login_group.bat .\publish\