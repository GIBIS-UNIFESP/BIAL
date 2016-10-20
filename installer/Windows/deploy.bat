SET PATH=%PATH%;C:\Qt\5.5\mingw492_32\bin\;C:\Program Files\7-Zip;C:\Program Files (x86)\NSIS\
if not exist Build mkdir Build
cd Build
qmake.exe ..\..\..\
mingw32-make.exe -j4
cd ..
"windeployqt.exe" "Build\release\bialgui.exe"
"makensis.exe" bial.nsi
7z.exe x bial_32.exe -o"%cd%\bial_32"
7z.exe a -tzip bial_portable.zip "%cd%\bial_32"
RMDIR /S /Q "%cd%\bial_32"
pause
