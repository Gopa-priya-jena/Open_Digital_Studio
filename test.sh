#! /bin/bash

#!/bin/bash
clear
rm -rf test_build_linux/
mkdir test_build_linux && cd test_build_linux/
echo -e "\n \n \n created directory build and entering\n \n \n "
echo -e "\n \n \n ========================= RUNNING CMAKE ================================== \n \n \n"

if cmake -DCMAKE_BUILD_TYPE=DEBUG -DTEST=ON ..; then
  clear
  echo -e " \n \n \n CMAKE SUCCEED \n \n \n"

  mv compile_commands.json ..
  echo -e "\n \n \n ========================= RUNNING MAKE ===================== \n \n \n"

  if make -j 512; then
    clear
    echo -e "\n \n \n COMPILATION SUCCEED \n \n \n"
    echo -e "\n \n \n RUNNING APP \n \n \n"

    ./Open_Digital_Studio
  fi
else

  echo "CMAKE FAILED"
  exit 1
fi
