export MACOSX_DEPLOYMENT_TARGET=10.9
export CFLAGS="-mmacosx-version-min=10.9 -target x86_64-apple-macos10.9"
export LDFLAGS="-mmacosx-version-min=10.9"
export CPPFLAGS="-mmacosx-version-min=10.9"

mkdir out_csri_unix
cd out_csri_unix
gcc -c -fPIC -I../src -I../src/posix ../src/*.c ../src/posix/*.c
gcc -shared -fPIC *.o -o libcsri.so
mv libcsri.so ../bin
cd ..
rm -rf out_csri_unix