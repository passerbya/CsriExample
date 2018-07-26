# tcc -Isrc -lass src/libass_csri.c  src/posix/openfile.c -run src/test.c data/test.ass
# gcc -Isrc -lass src/libass_csri.c  src/posix/openfile.c src/test.c -o test && ./test data/test.ass
mkdir out_unix
cd out_unix
gcc -c -fPIC -I../src -I../src/libass -I../src/win32 ../src/posix/openfile.c ../src/libass/*.c
gcc -lass -fPIC -shared *.o -o libass_csri.so
mv libass_csri.so ../csri/
cd ..
rm -rf out_unix