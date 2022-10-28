export MACOSX_DEPLOYMENT_TARGET=10.9
export CFLAGS="-mmacosx-version-min=10.9 -target x86_64-apple-macos10.9"
export LDFLAGS="-mmacosx-version-min=10.9"
export CPPFLAGS="-mmacosx-version-min=10.9"

# tcc -Isrc -lass src/libass_csri.c  src/posix/openfile.c -run src/test.c data/test.ass
# gcc -Isrc -lass src/libass_csri.c  src/posix/openfile.c src/test.c -o test && ./test data/test.ass
mkdir out_unix
cd out_unix
gcc -c -fPIC -I../src -I../src/libass -I../src/win32 ../src/posix/openfile.c ../src/libass/*.c
gcc -lass -fPIC -shared *.o -o libass_csri.so
mv libass_csri.so ../csri/
cd ..
rm -rf out_unix