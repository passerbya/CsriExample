## Linux
# CSRI_PATH=$(pwd)/csri tcc -Isrc -Lbin -lcsri -lass -run test/test.c data/test.ass
# tcc -Isrc -Lcsri -lass_csri -lass -run test/test.c data/test.ass

## MacOS
gcc -Isrc -Lcsri -lass_csri -lass test/test.c -o test_ass
DYLD_FALLBACK_LIBRARY_PATH=csri ./test_ass data/test.ass

rm test.bmp

gcc -Isrc -Lbin -lcsri -lass test/test.c -o test_csri
DYLD_FALLBACK_LIBRARY_PATH=bin CSRI_PATH=$(pwd)/csri ./test_csri data/test.ass