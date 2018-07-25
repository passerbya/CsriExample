mkdir out_csri_unix
cd out_csri_unix
gcc -c -fPIC -I../src -I../src/posix ../src/*.c ../src/posix/*.c
gcc -shared -fPIC *.o -o libcsri.so
mv libcsri.so ../bin
cd ..
rm -rf out_csri_unix