/**
 * @file MensajeDecodificado.cpp
 * @brief Implementación del contenedor de mensaje decodificado
 * @author Tu Nombre
 * @date 2024
 */

#include "MensajeDecodificado.h"
#include <iostream>

MensajeDecodificado::MensajeDecodificado()
{
    inicio = nullptr;
    final = nullptr;
    longitudTotal = 0;
}

MensajeDecodificado::~MensajeDecodificado()
{
    FragmentoMensaje* fragmentoActual = inicio;
    
    // Recorrer y eliminar cada nodo
    while (fragmentoActual != nullptr)
    {
        FragmentoMensaje* fragmentoTemporal = fragmentoActual;
        fragmentoActual = fragmentoActual->proximo;
        delete fragmentoTemporal;
    }
}

void MensajeDecodificado::agregarCaracter(char nuevoCaracter)
{
    // Crear nuevo fragmento
    FragmentoMensaje* nuevoFragmento = new FragmentoMensaje;
    nuevoFragmento->caracter = nuevoCaracter;
    nuevoFragmento->proximo = nullptr;
    nuevoFragmento->previo = final;

    // Enlazar con el final actual
    if (final != nullptr)
    {
        final->proximo = nuevoFragmento;
    }
    else
    {
        // Lista vacía: este es el primer elemento
        inicio = nuevoFragmento;
    }

    // Actualizar el puntero final
    final = nuevoFragmento;
    longitudTotal++;
}

void MensajeDecodificado::mostrarMensaje()
{
    FragmentoMensaje* fragmentoActual = inicio;
    
    while (fragmentoActual != nullptr)
    {
        std::cout << "[" << fragmentoActual->caracter << "]";
        fragmentoActual = fragmentoActual->proximo;
    }
}

int MensajeDecodificado::obtenerLongitud() const
{
    return longitudTotal;
}
