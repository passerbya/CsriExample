@echo off
set PATH=bin;%PATH%;
if exist test.bmp del test.bmp
mkdir out_libass
cl /DEBUG /Zi /nologo  /c /w /EHsc /Isrc /Isrc\libass /Isrc\win32 src\win32\openfile.c src\libass\*.c /Fo:.\out_libass\
link /DEBUG /nologo /dll /def:lib\libass_csri.def out_libass\*.obj /out:libass_csri.dll /PDB:"csri\libass_csri.pdb" lib\libass.lib
move libass_csri.dll csri
move libass_csri.lib lib
del *.exp *.ilk
tcc -Isrc -Lbin -lcsri -run test\test.c data\test.ass
REM tcc -I. -Lbin -lVSFilter -run src\test.c data\test.ass
REM rd /s/q out_libass