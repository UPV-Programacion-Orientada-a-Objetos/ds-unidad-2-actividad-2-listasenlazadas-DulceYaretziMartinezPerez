/**
 * @file PaqueteRotacion.cpp
 * @brief Implementación del paquete de rotación
 * @author Tu Nombre
 * @date 2024
 */

#include "PaqueteRotacion.h"
#include <iostream>

PaqueteRotacion::PaqueteRotacion(int grados)
{
    cantidadRotacion = grados;
}

void PaqueteRotacion::ejecutar(MensajeDecodificado* mensaje, DiscoRotatorio* disco)
{
    // Aplicar la rotación al disco
    disco->girar(cantidadRotacion);

    // Mostrar información de depuración
    std::cout << "Paquete recibido: [M," << cantidadRotacion 
              << "] -> Procesando... -> GIRANDO DISCO "
              << (cantidadRotacion >= 0 ? "+" : "") << cantidadRotacion 
              << "." << std::endl << std::endl;
    
    // Nota: El parámetro 'mensaje' no se usa en rotaciones,
    // pero está presente por la interfaz PaqueteBase
    (void)mensaje;  // Evitar warning de parámetro no usado
}
