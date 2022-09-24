find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(${PROJECT_NAME}_ULN2003 main.cpp)
target_include_directories(${PROJECT_NAME}_ULN2003 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_ULN2003 ${PROJECT_NAME} ${Boost_LIBRARIES})