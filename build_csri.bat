@echo off
mkdir out
set PATH=bin;%PATH%;
cl /nologo /c /w /EHsc /Isrc -Isrc/win32 src\*.c src\win32\*.c /Fo.\out\
link /nologo /dll /def:lib\libcsri.def out\*.obj /out:libcsri.dll
move libcsri.dll bin
move libcsri.lib lib
del *.exp
tcc -Isrc -Lbin -lcsri -run test\test.c data\test.ass
REM tcc -Isrc -Lbin -lass_csri src\test.c
REM test.exe data\test.ass
REM cl /Isrc lib/libass_csri.lib test.c
REM if exist test.bmp del test.bmp
REM test.exe data\test.ass
REM del test.exp test.lib
REM rd /s/q out