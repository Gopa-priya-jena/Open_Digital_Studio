#!/usr/bin/bash
clear
cd build_linux/
echo -e "\n ENTERING BUILD DIR \n "
echo -e "\n========================= RUNNING NINJA ===================== \n"
if cmake --build .; then
  clear

  echo -e "COMPILATION SUCCEED \n"
  echo -e "RUNNING APP \n"

  ./Open_Digital_Studio
fi
