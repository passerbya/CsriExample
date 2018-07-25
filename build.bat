REM tcc -I. -L. -lass libass_csri.c  win32/openfile.c -run test.c test.ass
REM cl /I. /c libass_csri.c win32/openfile.c test.c
set PATH=bin;%PATH%;
mkdir out
if exist test.bmp del test.bmp
cl /nologo  /w /EHsc /Isrc src\*.c src\win32\*.c /Fo.\out\ /Fe:test.exe lib\libass.lib
test.exe data\test.ass
del test.exp test.lib
rd /s/q out