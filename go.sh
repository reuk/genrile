mkdir -p build
cd build
cmake .. && make && ctest -V
