#! /bin/bash
clear
cd test_build/
# rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt LOG.txt Makefile test

echo "created directory build and entering "

echo "================================================================================running cmake========================================================================="

if cmake -DTEST=ON ..; then
  # mv compile_commands.json ..
  make -j 512 &&
    echo "================================================================================make=========================================================================" && ./test
else
  exit 1
fi
