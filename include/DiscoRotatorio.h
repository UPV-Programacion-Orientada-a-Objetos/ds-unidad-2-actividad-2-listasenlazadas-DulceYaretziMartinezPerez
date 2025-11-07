/**
 * @file DiscoRotatorio.h
 * @brief Implementación de disco de cifrado rotatorio usando lista circular
 * @author Tu Nombre
 * @date 2024
 * 
 * Implementa un cifrado César dinámico mediante una lista circular
 * doblemente enlazada que puede rotar en ambas direcciones.
 */

#ifndef DISCO_ROTATORIO_H
#define DISCO_ROTATORIO_H

/**
 * @struct ElementoDisco
 * @brief Nodo de la lista circular para el disco de cifrado
 * 
 * Estructura que representa un elemento individual del disco.
 * Cada elemento almacena un carácter y tiene enlaces bidireccionales.
 */
struct ElementoDisco
{
    char simbolo;              ///< Carácter almacenado en este elemento
    ElementoDisco* adelante;   ///< Enlace al siguiente elemento (sentido horario)
    ElementoDisco* atras;      ///< Enlace al elemento anterior (sentido antihorario)
};

/**
 * @class DiscoRotatorio
 * @brief Disco de cifrado que implementa rotación César variable
 * 
 * Esta clase implementa una lista circular doblemente enlazada que
 * contiene el alfabeto A-Z. La rotación del disco cambia el mapeo
 * entre caracteres, simulando un cifrado César con offset dinámico.
 */
class DiscoRotatorio
{
private:
    ElementoDisco* posicionCero;  ///< Puntero a la posición de referencia actual
    int tamanoAlfabeto;           ///< Tamaño del alfabeto (26 letras)

public:
    /**
     * @brief Constructor que inicializa el disco con el alfabeto A-Z
     * 
     * Crea una lista circular con 26 nodos (A-Z) y establece
     * la posición cero inicial en 'A'.
     */
    DiscoRotatorio();

    /**
     * @brief Destructor que libera toda la memoria del disco
     * 
     * Rompe el círculo y elimina todos los nodos uno por uno.
     */
    ~DiscoRotatorio();

    /**
     * @brief Rota el disco un número específico de posiciones
     * @param desplazamiento Cantidad de posiciones a rotar
     *                       (positivo: sentido horario, negativo: antihorario)
     * 
     * Mueve el puntero posicionCero, cambiando así el mapeo
     * de todos los caracteres subsecuentes.
     */
    void girar(int desplazamiento);

    /**
     * @brief Obtiene el carácter cifrado según la rotación actual
     * @param caracterOriginal Carácter a cifrar
     * @return Carácter cifrado según la posición actual del disco
     * 
     * Encuentra la posición relativa del carácter en el disco
     * y devuelve el carácter mapeado según la rotación actual.
     */
    char obtenerCifrado(char caracterOriginal);

private:
    /**
     * @brief Construye la lista circular inicial
     * 
     * Método auxiliar privado que crea y enlaza los 26 nodos.
     */
    void construirDisco();
};

#endif // DISCO_ROTATORIO_H
