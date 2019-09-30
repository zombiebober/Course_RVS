cmake_minimum_required(VERSION 2.8)
project(02_issue)
add_definitions("-std=c++11")

#    message(STATUS conan_check_compiler())

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(${PROJECT_NAME} main.cpp NumberControllerAPI.cpp NumberControllerAPI.h)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})