/**
 * @file PaqueteBase.h
 * @brief Interfaz abstracta para paquetes del protocolo PRT-7
 * @author Tu Nombre
 * @date 2024
 * 
 * Define la estructura base para todos los tipos de paquetes
 * que pueden ser recibidos a través del puerto serial.
 */

#ifndef PAQUETE_BASE_H
#define PAQUETE_BASE_H

// Forward declarations para evitar dependencias circulares
class MensajeDecodificado;
class DiscoRotatorio;

/**
 * @class PaqueteBase
 * @brief Clase abstracta que define la interfaz para paquetes PRT-7
 * 
 * Esta clase utiliza el patrón Template Method para definir el
 * comportamiento común de todos los paquetes del protocolo.
 * Las clases derivadas deben implementar el método ejecutar().
 */
class PaqueteBase
{
public:
    /**
     * @brief Método virtual puro para ejecutar la acción del paquete
     * @param mensaje Puntero a la estructura donde se almacena el mensaje
     * @param disco Puntero al disco rotatorio de cifrado
     * 
     * Este método debe ser implementado por cada tipo de paquete
     * específico (carga de carácter o rotación).
     */
    virtual void ejecutar(MensajeDecodificado* mensaje, DiscoRotatorio* disco) = 0;

    /**
     * @brief Destructor virtual para permitir polimorfismo correcto
     * 
     * CRÍTICO: Este destructor DEBE ser virtual para evitar fugas
     * de memoria cuando se elimina un objeto derivado a través de
     * un puntero a la clase base.
     */
    virtual ~PaqueteBase() {}
};

#endif // PAQUETE_BASE_H
