cmake_minimum_required(VERSION 3.10)

project(stepper_driver_lib)

option(BUILD_STEPPER_DRIVER_LIB_EXAMPLES "Build examples" ON)

add_subdirectory(bbb_gpio)

add_library(${PROJECT_NAME} STATIC src/stepper_driver_uln2003.cpp src/stepper_driver_tb6600.cpp)
target_link_libraries(${PROJECT_NAME} bbb_gpio)
target_include_directories(${PROJECT_NAME} PUBLIC include)

if(BUILD_STEPPER_DRIVER_LIB_EXAMPLES)
    add_subdirectory(examples)
endif()



