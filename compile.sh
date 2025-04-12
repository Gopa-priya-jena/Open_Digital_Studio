#! /usr/bin/bash

clear

cd build/
rm Open_Digital_Studio

cmake ..
echo "===========================================running make ======================================="

make -j 512
mv compile_commands.json ..

./Open_Digital_Studio
