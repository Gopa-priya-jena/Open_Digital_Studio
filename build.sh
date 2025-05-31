#!/bin/bash
clear
if cd build_linux/; then

    echo -e "\n ENTERING   build_linux  \n "
else
    mkdir build_linux
    cd build_linux/
    echo -e "\n CREATED DIRECTORY BUILD AND ENTERING \n "
fi
echo -e "\n ========================= RUNNING CMAKE ================================== \n "

if cmake .. -DCMAKE_BUILD_TYPE=DEBUG; then
    clear
    echo -e " \n CMAKE SUCCEED \n"

    mv compile_commands.json ..
    echo -e "\n========================= RUNNING MAKE ===================== \n"

    if cmake --build . --clean-first -j256; then
        clear
        echo -e "\n COMPILATION SUCCEED \n"
        echo -e "\n RUNNING APP \n"
        ./Open_Digital_Studio
    fi
else

    echo "CMAKE FAILED"
    exit 1
fi
