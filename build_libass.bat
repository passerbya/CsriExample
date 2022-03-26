@echo off
set PATH=bin;%PATH%;
if exist test.bmp del test.bmp
mkdir out_libass
cl /nologo  /c /w /EHsc /Isrc /Isrc\libass /Isrc\win32 src\win32\openfile.c src\libass\*.c /Fo:.\out_libass\
link /nologo /dll /def:lib\libass_csri.def out_libass\*.obj /out:libass_csri.dll lib\libass.lib
move libass_csri.dll csri
move libass_csri.lib lib
del *.exp
tcc -Isrc -Lbin -lcsri -run test\test.c data\test.ass
REM tcc -I. -Lbin -lVSFilter -run src\test.c data\test.ass
REM rd /s/q out_libass