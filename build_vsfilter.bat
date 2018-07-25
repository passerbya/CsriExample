set PATH=bin;%PATH%;
if exist test.bmp del test.bmp
cl /nologo  /w /EHsc /I. src\test.c /Fe:test.exe lib\VSFilter.lib
test.exe data\test.ass
del test.exp test.lib test.obj
REM tcc -I. -Lbin -lVSFilter -run src\test.c data\test.ass
