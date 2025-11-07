/**
 * @file PaqueteCaracter.h
 * @brief Paquete de tipo LOAD para carga de caracteres
 * @author Tu Nombre
 * @date 2024
 * 
 * Representa una trama del protocolo que transporta un carácter
 * que debe ser decodificado y almacenado en el mensaje.
 */

#ifndef PAQUETE_CARACTER_H
#define PAQUETE_CARACTER_H

#include "PaqueteBase.h"
#include "MensajeDecodificado.h"
#include "DiscoRotatorio.h"

/**
 * @class PaqueteCaracter
 * @brief Implementa un paquete de carga de carácter (tipo L)
 * 
 * Este tipo de paquete transporta un carácter que debe ser
 * decodificado usando el disco rotatorio actual y luego
 * agregado al mensaje final.
 */
class PaqueteCaracter : public PaqueteBase
{
private:
    char caracterTransportado;  ///< Carácter en formato cifrado

public:
    /**
     * @brief Constructor que inicializa el paquete con un carácter
     * @param simbolo Carácter recibido del puerto serial
     */
    PaqueteCaracter(char simbolo);

    /**
     * @brief Ejecuta la decodificación y almacenamiento del carácter
     * @param mensaje Puntero al mensaje donde se agregará el carácter
     * @param disco Puntero al disco que realizará la decodificación
     * 
     * Proceso:
     * 1. Obtiene el carácter decodificado usando el disco actual
     * 2. Agrega el carácter al mensaje
     * 3. Muestra información de depuración en consola
     */
    void ejecutar(MensajeDecodificado* mensaje, DiscoRotatorio* disco);
};

#endif // PAQUETE_CARACTER_H
