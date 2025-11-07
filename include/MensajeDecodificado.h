/**
 * @file MensajeDecodificado.h
 * @brief Lista doblemente enlazada para almacenar el mensaje descifrado
 * @author Tu Nombre
 * @date 2024
 * 
 * Implementa una estructura de datos secuencial para mantener
 * el orden correcto de los caracteres decodificados.
 */

#ifndef MENSAJE_DECODIFICADO_H
#define MENSAJE_DECODIFICADO_H

/**
 * @struct FragmentoMensaje
 * @brief Nodo de la lista doblemente enlazada
 * 
 * Cada fragmento almacena un carácter del mensaje decodificado
 * y mantiene enlaces bidireccionales para navegación eficiente.
 */
struct FragmentoMensaje
{
    char caracter;              ///< Carácter decodificado almacenado
    FragmentoMensaje* proximo;  ///< Puntero al siguiente fragmento
    FragmentoMensaje* previo;   ///< Puntero al fragmento anterior
};

/**
 * @class MensajeDecodificado
 * @brief Contenedor secuencial para el mensaje descifrado
 * 
 * Implementa una lista doblemente enlazada que mantiene el orden
 * de llegada de los caracteres decodificados. Permite inserción
 * eficiente al final y recorrido completo para visualización.
 */
class MensajeDecodificado
{
private:
    FragmentoMensaje* inicio;    ///< Primer fragmento del mensaje
    FragmentoMensaje* final;     ///< Último fragmento del mensaje
    int longitudTotal;           ///< Cantidad de caracteres almacenados

public:
    /**
     * @brief Constructor que inicializa un mensaje vacío
     */
    MensajeDecodificado();

    /**
     * @brief Destructor que libera toda la memoria utilizada
     * 
     * Recorre la lista completa eliminando cada nodo para
     * evitar fugas de memoria.
     */
    ~MensajeDecodificado();

    /**
     * @brief Agrega un carácter al final del mensaje
     * @param nuevoCaracter Carácter a agregar
     * 
     * Crea un nuevo nodo y lo enlaza al final de la lista,
     * manteniendo la integridad de los enlaces bidireccionales.
     */
    void agregarCaracter(char nuevoCaracter);

    /**
     * @brief Muestra el mensaje completo en la salida estándar
     * 
     * Recorre la lista desde el inicio hasta el final,
     * imprimiendo cada carácter entre corchetes.
     */
    void mostrarMensaje();

    /**
     * @brief Obtiene la longitud actual del mensaje
     * @return Número de caracteres almacenados
     */
    int obtenerLongitud() const;
};

#endif // MENSAJE_DECODIFICADO_H
