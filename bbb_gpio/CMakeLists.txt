cmake_minimum_required(VERSION 3.10)

project(bbb_gpio)

add_library(${PROJECT_NAME} src/BBBiolib.c src/BBBiolib_PWMSS.c src/BBBiolib_McSPI.c src/BBBiolib_ADCTSC.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)

