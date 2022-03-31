pushd .
otool -L csri/libass_csri.so
install_name_tool -change /usr/local/lib/libass.9.dylib @executable_path/../Frameworks/libass.9.dylib csri/libass_csri.so
otool -L csri/libass_csri.so
popd