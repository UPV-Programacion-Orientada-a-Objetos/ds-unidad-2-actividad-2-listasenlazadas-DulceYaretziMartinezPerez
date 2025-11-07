/**
 * @file ComunicadorSerial.h
 * @brief Interfaz de comunicación serial para Windows
 * @author Tu Nombre
 * @date 2024
 * 
 * Proporciona una abstracción para la comunicación con puertos
 * seriales COM, permitiendo leer datos del Arduino.
 */

#ifndef COMUNICADOR_SERIAL_H
#define COMUNICADOR_SERIAL_H

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @class ComunicadorSerial
 * @brief Manejador de comunicación serial para protocolo PRT-7
 * 
 * Encapsula la lógica de bajo nivel para abrir, configurar
 * y leer desde un puerto serial en Windows.
 */
class ComunicadorSerial
{
private:
#ifdef _WIN32
    HANDLE manejadorPuerto;  ///< Handle del puerto COM en Windows
#endif
    bool conexionActiva;     ///< Estado de la conexión
    
    /**
     * @brief Configura los parámetros del puerto serial
     * @return true si la configuración fue exitosa
     * 
     * Establece: 9600 baud, 8 bits, sin paridad, 1 bit de parada
     */
    bool configurarParametros();

public:
    /**
     * @brief Constructor que abre y configura el puerto
     * @param nombrePuerto Nombre del puerto (ej: "COM3")
     * 
     * Intenta abrir el puerto especificado y aplicar la
     * configuración estándar del protocolo PRT-7.
     */
    ComunicadorSerial(const char* nombrePuerto);

    /**
     * @brief Destructor que cierra el puerto
     */
    ~ComunicadorSerial();

    /**
     * @brief Lee una línea completa del puerto serial
     * @param buffer Array donde se almacenará la línea leída
     * @param tamanioBuffer Tamaño máximo del buffer
     * @return true si se leyó una línea completa, false en caso contrario
     * 
     * Lee caracteres hasta encontrar un salto de línea (\n),
     * eliminando caracteres de retorno de carro (\r).
     */
    bool capturarLinea(char* buffer, int tamanioBuffer);

    /**
     * @brief Verifica el estado de la conexión
     * @return true si el puerto está abierto y funcional
     */
    bool estaOperativo();
};

#endif // COMUNICADOR_SERIAL_H
