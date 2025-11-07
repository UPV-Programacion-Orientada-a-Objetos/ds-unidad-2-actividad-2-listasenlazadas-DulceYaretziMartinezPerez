/**
 * @file main.cpp
 * @brief Programa principal del decodificador de protocolo PRT-7
 * @author Tu Nombre
 * @date 2024
 * 
 * Sistema de decodificación de mensajes cifrados transmitidos
 * mediante un protocolo de ensamblaje dinámico desde Arduino.
 */

#include "PaqueteBase.h"
#include "PaqueteCaracter.h"
#include "PaqueteRotacion.h"
#include "DiscoRotatorio.h"
#include "MensajeDecodificado.h"
#include "ComunicadorSerial.h"
#include <iostream>
#include <cstdlib>

// =====================================================
// FUNCIONES AUXILIARES DE PARSEO
// =====================================================

/**
 * @brief Elimina espacios en blanco al inicio de una cadena
 * @param texto Cadena a procesar
 * @return Índice del primer carácter no blanco
 */
int eliminarEspaciosIniciales(const char* texto)
{
    int indice = 0;
    while (texto[indice] == ' ' || texto[indice] == '\t')
    {
        indice++;
    }
    return indice;
}

/**
 * @brief Elimina espacios y saltos de línea al final de una cadena
 * @param texto Cadena a modificar (se modifica in-place)
 */
void eliminarEspaciosFinales(char* texto)
{
    int longitud = 0;
    while (texto[longitud] != '\0')
    {
        longitud++;
    }
    
    if (longitud == 0)
        return;
    
    longitud--;
    while (longitud >= 0 && 
           (texto[longitud] == ' ' || texto[longitud] == '\t' ||
            texto[longitud] == '\r' || texto[longitud] == '\n'))
    {
        texto[longitud] = '\0';
        longitud--;
    }
}

/**
 * @brief Busca la posición de un carácter en una cadena
 * @param texto Cadena donde buscar
 * @param objetivo Carácter a buscar
 * @return Índice del carácter, o -1 si no se encuentra
 */
int buscarCaracter(const char* texto, char objetivo)
{
    int indice = 0;
    while (texto[indice] != '\0')
    {
        if (texto[indice] == objetivo)
        {
            return indice;
        }
        indice++;
    }
    return -1;
}

/**
 * @brief Convierte una cadena a número entero (similar a atoi)
 * @param texto Cadena numérica a convertir
 * @return Valor entero
 */
int convertirAEntero(const char* texto)
{
    int resultado = 0;
    int signo = 1;
    int indice = 0;
    
    // Manejar signo negativo
    if (texto[0] == '-')
    {
        signo = -1;
        indice = 1;
    }
    else if (texto[0] == '+')
    {
        indice = 1;
    }
    
    // Convertir dígitos
    while (texto[indice] >= '0' && texto[indice] <= '9')
    {
        resultado = resultado * 10 + (texto[indice] - '0');
        indice++;
    }
    
    return resultado * signo;
}

/**
 * @brief Analiza y crea un paquete desde una línea del protocolo
 * @param lineaTexto Línea recibida (formato: "L,X" o "M,N")
 * @return Puntero al paquete creado, o nullptr si hay error
 * 
 * Formatos válidos:
 * - "L,A" -> Paquete de carácter con 'A'
 * - "M,5" -> Paquete de rotación +5
 * - "M,-3" -> Paquete de rotación -3
 */
PaqueteBase* analizarPaquete(const char* lineaTexto)
{
    // Validar entrada
    if (lineaTexto == nullptr || lineaTexto[0] == '\0')
    {
        return nullptr;
    }

    // Copiar a buffer de trabajo
    char bufferTrabajo[128];
    int indice = 0;
    while (lineaTexto[indice] != '\0' && indice < 127)
    {
        bufferTrabajo[indice] = lineaTexto[indice];
        indice++;
    }
    bufferTrabajo[indice] = '\0';

    // Verificar longitud mínima
    if (indice < 3)
    {
        return nullptr;
    }

    // Extraer tipo de paquete
    char tipoPaquete = bufferTrabajo[0];
    
    // Buscar separador
    int posicionSeparador = buscarCaracter(bufferTrabajo, ',');
    
    if (posicionSeparador == -1 || posicionSeparador >= indice - 1)
    {
        return nullptr;
    }

    // Extraer contenido después del separador
    const char* contenido = &bufferTrabajo[posicionSeparador + 1];

    // Crear paquete según tipo
    if (tipoPaquete == 'L' || tipoPaquete == 'l')
    {
        // Paquete de carácter
        if (contenido[0] != '\0')
        {
            return new PaqueteCaracter(contenido[0]);
        }
    }
    else if (tipoPaquete == 'M' || tipoPaquete == 'm')
    {
        // Paquete de rotación
        int valorRotacion = convertirAEntero(contenido);
        return new PaqueteRotacion(valorRotacion);
    }

    return nullptr;
}

/**
 * @brief Detecta si una línea es un patrón de finalización
 * @param lineaTexto Línea a verificar
 * @return true si es patrón de fin (M con valor negativo)
 */
bool esIndicadorFinalizacion(const char* lineaTexto)
{
    if (lineaTexto == nullptr || lineaTexto[0] == '\0')
        return false;

    // Debe ser tipo M
    if (lineaTexto[0] != 'M' && lineaTexto[0] != 'm')
        return false;

    // Buscar coma
    int posicionComa = buscarCaracter(lineaTexto, ',');
    if (posicionComa == -1)
        return false;

    // Verificar signo negativo después de la coma
    int indice = posicionComa + 1;
    while (lineaTexto[indice] == ' ' || lineaTexto[indice] == '\t')
    {
        indice++;
    }

    return (lineaTexto[indice] == '-');
}

// =====================================================
// FUNCIÓN PRINCIPAL
// =====================================================

/**
 * @brief Punto de entrada del programa
 * @return 0 si la ejecución fue exitosa, 1 en caso de error
 */
int main()
{
    // Encabezado del sistema
    std::cout << "========================================" << std::endl;
    std::cout << "  Sistema Decodificador PRT-7 v1.0" << std::endl;
    std::cout << "  Protocolo de Transmision Rotatorio" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Solicitar puerto de comunicación
    char identificadorPuerto[32];
    std::cout << "Ingrese el identificador del puerto (ejemplo: COM3): ";
    std::cin >> identificadorPuerto;

    std::cout << std::endl << "Iniciando sistema. Estableciendo conexion con " 
              << identificadorPuerto << "..." << std::endl;

    // Establecer conexión serial
    ComunicadorSerial comunicador(identificadorPuerto);

    if (!comunicador.estaOperativo())
    {
        std::cout << std::endl << "ERROR: Imposible establecer conexion." << std::endl;
        std::cout << "Verificaciones necesarias:" << std::endl;
        std::cout << "  1. Arduino conectado fisicamente" << std::endl;
        std::cout << "  2. Puerto correcto seleccionado" << std::endl;
        std::cout << "  3. Puerto disponible (no usado por otro programa)" << std::endl;
        std::cout << "  4. Drivers USB instalados" << std::endl;
        return 1;
    }

    std::cout << "Conexion exitosa. Esperando transmision de paquetes..." 
              << std::endl << std::endl;

    // Inicializar estructuras de datos
    MensajeDecodificado mensajeFinal;
    DiscoRotatorio discoCifrado;

    // Variables de control
    char bufferLectura[128];
    bool transmisionCompleta = false;
    int paquetesRecibidos = 0;
    const int MINIMO_PAQUETES = 8;  // Mínimo para considerar mensaje válido

    // Bucle principal de procesamiento
    while (!transmisionCompleta)
    {
        if (comunicador.capturarLinea(bufferLectura, 128))
        {
            // Limpiar espacios
            int inicio = eliminarEspaciosIniciales(bufferLectura);
            eliminarEspaciosFinales(&bufferLectura[inicio]);

            // Ignorar líneas vacías
            if (bufferLectura[inicio] == '\0')
                continue;

            // Analizar y crear paquete
            PaqueteBase* paqueteActual = analizarPaquete(&bufferLectura[inicio]);

            if (paqueteActual != nullptr)
            {
                // Ejecutar paquete (polimorfismo)
                paqueteActual->ejecutar(&mensajeFinal, &discoCifrado);
                paquetesRecibidos++;

                // Verificar condición de finalización
                if (esIndicadorFinalizacion(&bufferLectura[inicio]) && 
                    paquetesRecibidos >= MINIMO_PAQUETES)
                {
                    std::cout << std::endl 
                              << ">>> Indicador de finalizacion detectado. <<<" 
                              << std::endl;
                    transmisionCompleta = true;
                }

                // Liberar memoria del paquete
                delete paqueteActual;
            }
            else
            {
                // Reportar solo errores de paquetes aparentemente válidos
                char primerCaracter = bufferLectura[inicio];
                if (primerCaracter == 'L' || primerCaracter == 'l' ||
                    primerCaracter == 'M' || primerCaracter == 'm')
                {
                    std::cout << "Paquete malformado detectado: [" 
                              << &bufferLectura[inicio] << "]" << std::endl;
                }
            }
        }
    }

    // Presentar resultados
    std::cout << std::endl << "---" << std::endl;
    std::cout << "Transmision finalizada." << std::endl;
    std::cout << "Total de paquetes procesados: " << paquetesRecibidos << std::endl;
    std::cout << "Longitud del mensaje: " << mensajeFinal.obtenerLongitud() 
              << " caracteres" << std::endl;
    std::cout << std::endl << "MENSAJE SECRETO DECODIFICADO:" << std::endl;
    std::cout << ">>> ";
    mensajeFinal.mostrarMensaje();
    std::cout << " <<<" << std::endl;
    std::cout << "---" << std::endl << std::endl;
    std::cout << "Liberando recursos... Sistema terminado correctamente." << std::endl;

    return 0;
}
