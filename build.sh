#! /bin/bash

clear

rm -rf build/
mkdir build && cd build/
echo "created directory build and entering "

echo "================================================================================running cmake========================================================================="

if cmake -DCMAKE_BUILD_TYPE=DEBUG ..; then
    mv compile_commands.json ..
    make -j 512

    echo "================================================================================make=========================================================================" && ./Open_Digital_Studio
else
    exit 1
fi
