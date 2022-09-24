
find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(${PROJECT_NAME}_TB6600 main.cpp)
target_include_directories(${PROJECT_NAME}_TB6600 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_TB6600 ${PROJECT_NAME} ${Boost_LIBRARIES})