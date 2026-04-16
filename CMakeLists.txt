cmake_minimum_required(VERSION 3.16)
project(internetcontrol)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTORCC ON)

# Rutas absolutas a los encabezados
include_directories("/home/madrid/veyon-src/core/include")
include_directories("/home/madrid/veyon-src/core/src")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Rastreador automático para la librería binaria de Veyon en Ubuntu
find_library(VEYON_CORE_LIB 
    NAMES veyon-core
    PATHS 
    /usr/lib/x86_64-linux-gnu/veyon
    /usr/lib/veyon
    /usr/lib/x86_64-linux-gnu
    /usr/lib
)

if(NOT VEYON_CORE_LIB)
    message(FATAL_ERROR "No se encontró libveyon-core.so en el sistema. Asegúrate de que Veyon está instalado.")
endif()

add_library(internetcontrol MODULE 
    src/InternetControlPlugin.cpp
    src/InternetControlPlugin.h
    src/resources.qrc
)

target_compile_definitions(internetcontrol PRIVATE -DVEYON_BUILD_LIBRARY)
# Enlazamos usando la ruta exacta encontrada por el rastreador
target_link_libraries(internetcontrol PRIVATE Qt5::Widgets ${VEYON_CORE_LIB})