cmake_minimum_required(VERSION 3.10)

project(projector_test_datavis LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick Charts REQUIRED)

add_executable(${PROJECT_NAME}
    "main.cpp"
    "datareader.cpp"
    "datareader.h"
    "qml.qrc")

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Quick
    Qt5::Charts)

#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -O3)
