cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(DecodificadorPRT7 VERSION 1.0 LANGUAGES CXX)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opciones de compilación
if(MSVC)
    # Opciones para Visual Studio
    add_compile_options(/W4)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    # Opciones para GCC/Clang
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Incluir directorios de headers
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/ComunicadorSerial.cpp
    src/MensajeDecodificado.cpp
    src/DiscoRotatorio.cpp
    src/PaqueteCaracter.cpp
    src/PaqueteRotacion.cpp
)

# Archivos de cabecera
set(HEADERS
    include/PaqueteBase.h
    include/PaqueteCaracter.h
    include/PaqueteRotacion.h
    include/DiscoRotatorio.h
    include/MensajeDecodificado.h
    include/ComunicadorSerial.h
)

# Crear el ejecutable
add_executable(decodificador ${SOURCES} ${HEADERS})

# Configuración específica para Windows
if(WIN32)
    target_compile_definitions(decodificador PRIVATE WINDOWS_BUILD)
endif()

# Información de compilación
message(STATUS "===========================================")
message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Estándar C++: ${CMAKE_CXX_STANDARD}")
message(STATUS "===========================================")

# Instalación (opcional)
install(TARGETS decodificador
    RUNTIME DESTINATION bin
)

# Documentación con Doxygen (si está disponible)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generando documentación con Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen no encontrado - documentación no disponible")
endif()
