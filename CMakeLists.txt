cmake_minimum_required(VERSION 3.16)
project(internetcontrol)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTORCC ON)

# Rutas a los encabezados de Veyon
include_directories("/home/madrid/veyon-src/core/include")
include_directories("/home/madrid/veyon-src/core/src")

# ====================================================================
# FORZAMOS Qt 6 (Nada de Qt 5 aquí)
# ====================================================================
find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

message(STATUS "Compilando EXCLUSIVAMENTE con Qt 6")

# Buscamos la librería core de Veyon
find_library(VEYON_CORE_LIB NAMES veyon-core PATHS /usr/lib/x86_64-linux-gnu/veyon)

add_library(internetcontrol MODULE 
    src/InternetControlPlugin.cpp
    src/InternetControlPlugin.h
    src/resources.qrc
)

# Quitamos el prefijo 'lib' para que se llame internetcontrol.so
set_target_properties(internetcontrol PROPERTIES PREFIX "")

target_compile_definitions(internetcontrol PRIVATE -DVEYON_BUILD_LIBRARY)

# Enlazamos con Qt 6
target_link_libraries(internetcontrol PRIVATE 
    Qt6::Widgets 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Network
    ${VEYON_CORE_LIB}
)