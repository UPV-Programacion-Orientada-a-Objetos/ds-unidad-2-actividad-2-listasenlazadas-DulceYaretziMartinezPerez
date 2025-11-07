/**
 * @file PaqueteCaracter.cpp
 * @brief Implementación del paquete de carga de carácter
 * @author Tu Nombre
 * @date 2024
 */

#include "PaqueteCaracter.h"
#include <iostream>

PaqueteCaracter::PaqueteCaracter(char simbolo)
{
    caracterTransportado = simbolo;
}

void PaqueteCaracter::ejecutar(MensajeDecodificado* mensaje, DiscoRotatorio* disco)
{
    // Paso 1: Obtener el carácter decodificado usando el disco
    char caracterDecodificado = disco->obtenerCifrado(caracterTransportado);

    // Paso 2: Agregar al mensaje
    mensaje->agregarCaracter(caracterDecodificado);

    // Paso 3: Mostrar información de progreso
    std::cout << "Paquete recibido: [L," << caracterTransportado 
              << "] -> Procesando... -> Simbolo '" 
              << caracterTransportado << "' decodificado como '" 
              << caracterDecodificado << "'. Mensaje: ";
    
    mensaje->mostrarMensaje();
    std::cout << std::endl;
}
