cmake_minimum_required(VERSION 3.16)
project(AuxDesk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Detecta Qt6 instalado via apt (Ubuntu/Debian)
set(QT_PREFIX "/usr/lib/x86_64-linux-gnu/cmake")

set(Qt6_DIR "${QT_PREFIX}/Qt6")
list(APPEND CMAKE_PREFIX_PATH "${QT_PREFIX}")
find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
# executável precisa dos includes
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# executável também precisa linkar Qt
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        AuxDeskLib
        Qt6::Widgets
)