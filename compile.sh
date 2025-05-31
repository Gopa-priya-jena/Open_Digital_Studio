#!/usr/bin/bash

clear
cd build_linux/
echo -e "\n \n \n  entering build dir \n \n \n "
echo -e "\n \n \n ========================= RUNNING NINJA ===================== \n \n \n"

if cmake --build .; then
  clear
  echo -e "\n \n \n COMPILATION SUCCEED \n \n \n"
  echo -e "\n \n \n RUNNING APP \n \n \n"

  ./Open_Digital_Studio
fi
