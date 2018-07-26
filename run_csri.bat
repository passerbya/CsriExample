@echo off

if "%1"=="direct" goto direct

set PATH=bin;%PATH%;
REM if exist test.bmp del test.bmp
echo "Run CSRI Wrap"
tcc -Isrc -Lbin -lcsri -run test\test.c data\test.ass
REM tcc -DUSE_RWIMG -Isrc -Isrc\rwimg -Lbin -lcsri -lrwimg -run test\test.c data\test.ass
goto :eof

:direct
set PATH=csri;%PATH%;
echo "Run CSRI Direct"
tcc -Isrc -Lcsri -lass_csri -run test\test.c data\test.ass
REM tcc -Isrc -Lcsri -lVSFilter -run test\test.c data\test.ass
REM tcc -Isrc -Lcsri -lVSFilterMod -run test\test.c data\test.ass
goto :eof

:eof
REM del test.bmp