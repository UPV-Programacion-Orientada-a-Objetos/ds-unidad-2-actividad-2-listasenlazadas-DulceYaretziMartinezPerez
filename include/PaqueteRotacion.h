/**
 * @file PaqueteRotacion.h
 * @brief Paquete de tipo MAP para rotación del disco
 * @author Tu Nombre
 * @date 2024
 * 
 * Representa una trama que modifica el estado del disco rotatorio,
 * cambiando el mapeo de caracteres para paquetes subsecuentes.
 */

#ifndef PAQUETE_ROTACION_H
#define PAQUETE_ROTACION_H

#include "PaqueteBase.h"
#include "MensajeDecodificado.h"
#include "DiscoRotatorio.h"

/**
 * @class PaqueteRotacion
 * @brief Implementa un paquete de rotación (tipo M)
 * 
 * Este tipo de paquete no transporta datos, sino una instrucción
 * para modificar el estado del disco rotatorio. Cambia dinámicamente
 * el cifrado aplicado a los caracteres subsecuentes.
 */
class PaqueteRotacion : public PaqueteBase
{
private:
    int cantidadRotacion;  ///< Desplazamiento a aplicar (+ o -)

public:
    /**
     * @brief Constructor que inicializa el paquete con un desplazamiento
     * @param grados Cantidad de posiciones a rotar el disco
     *               Valores positivos: rotación horaria
     *               Valores negativos: rotación antihoraria
     */
    PaqueteRotacion(int grados);

    /**
     * @brief Ejecuta la rotación del disco de cifrado
     * @param mensaje Puntero al mensaje (no usado en rotación)
     * @param disco Puntero al disco que será rotado
     * 
     * Proceso:
     * 1. Aplica la rotación al disco
     * 2. Muestra información de depuración en consola
     */
    void ejecutar(MensajeDecodificado* mensaje, DiscoRotatorio* disco);
};

#endif // PAQUETE_ROTACION_H