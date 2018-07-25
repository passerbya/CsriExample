# tcc -Isrc -lass src/libass_csri.c  src/posix/openfile.c -run src/test.c data/test.ass
gcc -Isrc -lass src/libass_csri.c  src/posix/openfile.c src/test.c -o test && ./test data/test.ass
