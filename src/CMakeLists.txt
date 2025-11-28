cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UDEV REQUIRED libudev)

set(SOURCES
    MainWindow.cpp
    Serial.cpp
    Display.cpp
)

set(HEADERS
    MainWindow.h
    Serial.hpp
    Display.hpp
)

add_library(AuxDeskLib ${SOURCES} ${HEADERS})

set_target_properties(AuxDeskLib PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_link_libraries(AuxDeskLib PRIVATE Qt6::Widgets ${UDEV_LIBRARIES})
target_include_directories(AuxDeskLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${UDEV_LIBRARIES})
