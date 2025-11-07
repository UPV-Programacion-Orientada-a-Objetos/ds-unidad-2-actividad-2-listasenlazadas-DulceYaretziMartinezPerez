/**
 * @file DiscoRotatorio.cpp
 * @brief Implementación del disco de cifrado rotatorio
 * @author Tu Nombre
 * @date 2024
 */

#include "DiscoRotatorio.h"

DiscoRotatorio::DiscoRotatorio()
{
    posicionCero = nullptr;
    tamanoAlfabeto = 26;
    construirDisco();
}

DiscoRotatorio::~DiscoRotatorio()
{
    if (posicionCero == nullptr)
        return;

    // Paso 1: Romper el círculo para evitar bucle infinito
    ElementoDisco* ultimoElemento = posicionCero->atras;
    if (ultimoElemento != nullptr)
    {
        ultimoElemento->adelante = nullptr;
    }

    // Paso 2: Eliminar cada nodo secuencialmente
    ElementoDisco* nodoActual = posicionCero;
    while (nodoActual != nullptr)
    {
        ElementoDisco* siguienteNodo = nodoActual->adelante;
        delete nodoActual;
        nodoActual = siguienteNodo;
    }
}

void DiscoRotatorio::construirDisco()
{
    ElementoDisco* primerElemento = nullptr;
    ElementoDisco* elementoAnterior = nullptr;

    // Crear los 26 elementos (A-Z)
    for (int indice = 0; indice < tamanoAlfabeto; indice++)
    {
        ElementoDisco* nuevoElemento = new ElementoDisco;
        nuevoElemento->simbolo = 'A' + indice;
        nuevoElemento->adelante = nullptr;
        nuevoElemento->atras = elementoAnterior;

        // Enlazar con el elemento anterior
        if (elementoAnterior != nullptr)
        {
            elementoAnterior->adelante = nuevoElemento;
        }
        else
        {
            // Es el primer elemento
            primerElemento = nuevoElemento;
        }

        elementoAnterior = nuevoElemento;
    }

    // Cerrar el círculo: conectar el último con el primero
    if (primerElemento != nullptr && elementoAnterior != nullptr)
    {
        primerElemento->atras = elementoAnterior;
        elementoAnterior->adelante = primerElemento;
        posicionCero = primerElemento;  // Iniciar en 'A'
    }
}

void DiscoRotatorio::girar(int desplazamiento)
{
    if (posicionCero == nullptr)
        return;

    // Normalizar el desplazamiento al rango [0, tamanoAlfabeto)
    desplazamiento = desplazamiento % tamanoAlfabeto;

    // Convertir desplazamiento negativo a su equivalente positivo
    if (desplazamiento < 0)
    {
        desplazamiento = tamanoAlfabeto + desplazamiento;
    }

    // Mover el puntero posicionCero
    for (int paso = 0; paso < desplazamiento; paso++)
    {
        posicionCero = posicionCero->adelante;
    }
}

char DiscoRotatorio::obtenerCifrado(char caracterOriginal)
{
    // Caracteres no alfabéticos se retornan sin cambios
    if ((caracterOriginal < 'A' || caracterOriginal > 'Z') && 
        (caracterOriginal < 'a' || caracterOriginal > 'z'))
    {
        return caracterOriginal;
    }

    // Manejar minúsculas
    bool eraMinuscula = false;
    if (caracterOriginal >= 'a' && caracterOriginal <= 'z')
    {
        caracterOriginal = caracterOriginal - 'a' + 'A';  // Convertir a mayúscula
        eraMinuscula = true;
    }

    // Calcular la posición del carácter en el alfabeto (0-25)
    int posicionCaracter = caracterOriginal - 'A';

    // Navegar hasta el elemento correspondiente
    ElementoDisco* elementoBuscado = posicionCero;
    for (int contador = 0; contador < posicionCaracter; contador++)
    {
        elementoBuscado = elementoBuscado->adelante;
    }

    // El carácter cifrado es el símbolo en ese elemento
    char resultado = elementoBuscado->simbolo;

    // Restaurar minúscula si era necesario
    if (eraMinuscula)
    {
        resultado = resultado - 'A' + 'a';
    }

    return resultado;
}
