cmake_minimum_required(VERSION 3.10)

project(Velox_tests)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the Qt libraries
# Qt5
find_package(Qt5 COMPONENTS Quick SerialPort Charts Widgets REQUIRED)
# Generate code from ui files
#qt5_wrap_ui(UI_HEADERS mainwindow.ui)

# Generate rules for building source files from the resources
qt5_add_resources(QRCS qml.qrc)

# Tell CMake to create the helloworld executable
add_executable(projector_test
    src/main.cpp
    src/motorstatescanner.cpp
    src/powermeterscanner.cpp
    src/datasource.cpp
    src/dummydatasource.cpp
#    src/powermeter.cpp
    include/projector_test/motorstatescanner.h
    include/projector_test/powermeterscanner.h
    include/projector_test/datasource.h
    include/projector_test/dummydatasource.h
#    include/utils/powermeter.h
#    mainwindow.cpp
#    ${UI_HEADERS}
    ${QRCS}
)
target_include_directories(projector_test PUBLIC include/)
# Add the Qt5 COMPONENTS for linking
target_link_libraries(projector_test
    Qt5::Quick
    Qt5::SerialPort
    Qt5::Widgets
    Qt5::Charts
    )
target_compile_features(projector_test PRIVATE cxx_std_17)
target_compile_options(projector_test PRIVATE -Wall -Wextra -Wpedantic -O3)
