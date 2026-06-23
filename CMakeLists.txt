cmake_minimum_required(VERSION 3.20)

project(ZiPlusDownload)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
)

add_executable(
        ZiPlusDownload
        main.cpp
        MainWindow.cpp
        MainWindow.h
)

target_link_libraries(
        ZiPlusDownload
        Qt6::Core
        Qt6::Widgets
)