mkdir out
set PATH=bin;%PATH%;
REM cl /nologo /c /w /EHsc /Isrc src\*.c src\win32\*.c /Fo.\out\
REM link /nologo /dll /def:lib\libass_csri.def out\*.obj lib\libass.lib
REM tcc -Isrc -Lbin -lass_csri -run src\test.c data\test.ass
cl /Isrc lib/libass_csri.lib test.c
if exist test.bmp del test.bmp
test.exe data\test.ass
del test.exp test.lib
rd /s/q out