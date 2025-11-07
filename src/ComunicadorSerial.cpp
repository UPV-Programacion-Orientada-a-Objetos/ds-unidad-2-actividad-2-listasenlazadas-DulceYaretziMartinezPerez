/**
 * @file ComunicadorSerial.cpp
 * @brief Implementación del comunicador serial
 * @author Tu Nombre
 * @date 2024
 */

#include "ComunicadorSerial.h"
#include <iostream>

ComunicadorSerial::ComunicadorSerial(const char* nombrePuerto)
{
    conexionActiva = false;

#ifdef _WIN32
    // Construir nombre completo del puerto (ej: \\.\COM3)
    char nombreCompleto[256];
    int indice = 0;
    
    // Copiar prefijo "\\.\"
    nombreCompleto[indice++] = '\\';
    nombreCompleto[indice++] = '\\';
    nombreCompleto[indice++] = '.';
    nombreCompleto[indice++] = '\\';
    
    // Copiar nombre del puerto
    int i = 0;
    while (nombrePuerto[i] != '\0' && indice < 255)
    {
        nombreCompleto[indice++] = nombrePuerto[i++];
    }
    nombreCompleto[indice] = '\0';

    // Intentar abrir el puerto
    manejadorPuerto = CreateFileA(
        nombreCompleto,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (manejadorPuerto == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error: No se pudo abrir " << nombrePuerto << std::endl;
        return;
    }

    // Configurar el puerto
    if (!configurarParametros())
    {
        CloseHandle(manejadorPuerto);
        return;
    }

    // Limpiar buffers residuales
    PurgeComm(manejadorPuerto, PURGE_RXCLEAR | PURGE_TXCLEAR);

    conexionActiva = true;
    std::cout << "Conexion establecida en " << nombrePuerto << std::endl;
#endif
}

ComunicadorSerial::~ComunicadorSerial()
{
#ifdef _WIN32
    if (conexionActiva)
    {
        CloseHandle(manejadorPuerto);
    }
#endif
}

bool ComunicadorSerial::configurarParametros()
{
#ifdef _WIN32
    // Obtener configuración actual
    DCB parametrosSerial = {0};
    parametrosSerial.DCBlength = sizeof(parametrosSerial);

    if (!GetCommState(manejadorPuerto, &parametrosSerial))
    {
        std::cout << "Error al obtener configuracion del puerto" << std::endl;
        return false;
    }

    // Establecer parámetros del protocolo PRT-7
    parametrosSerial.BaudRate = CBR_9600;     // 9600 baudios
    parametrosSerial.ByteSize = 8;            // 8 bits de datos
    parametrosSerial.StopBits = ONESTOPBIT;   // 1 bit de parada
    parametrosSerial.Parity = NOPARITY;       // Sin paridad

    if (!SetCommState(manejadorPuerto, &parametrosSerial))
    {
        std::cout << "Error al configurar parametros" << std::endl;
        return false;
    }

    // Configurar timeouts para lectura no bloqueante
    COMMTIMEOUTS tiempos = {0};
    tiempos.ReadIntervalTimeout = 50;
    tiempos.ReadTotalTimeoutConstant = 200;
    tiempos.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(manejadorPuerto, &tiempos))
    {
        std::cout << "Error al configurar timeouts" << std::endl;
        return false;
    }

    return true;
#else
    return false;
#endif
}

bool ComunicadorSerial::capturarLinea(char* buffer, int tamanioBuffer)
{
#ifdef _WIN32
    if (!conexionActiva)
        return false;

    DWORD bytesCapturados;
    int posicionActual = 0;
    bool lineaCompleta = false;

    // Inicializar buffer
    for (int i = 0; i < tamanioBuffer; i++)
    {
        buffer[i] = '\0';
    }

    // Leer carácter por carácter hasta encontrar fin de línea
    while (posicionActual < tamanioBuffer - 1 && !lineaCompleta)
    {
        char simboloLeido;
        DWORD cantidadLeida = 0;

        if (ReadFile(manejadorPuerto, &simboloLeido, 1, &cantidadLeida, NULL))
        {
            if (cantidadLeida > 0)
            {
                // Ignorar retorno de carro
                if (simboloLeido == '\r')
                {
                    continue;
                }

                // Detectar fin de línea
                if (simboloLeido == '\n')
                {
                    if (posicionActual > 0)
                    {
                        lineaCompleta = true;
                    }
                }
                else
                {
                    buffer[posicionActual++] = simboloLeido;
                }
            }
        }
        else
        {
            // Error de lectura
            break;
        }
    }

    buffer[posicionActual] = '\0';
    return (posicionActual > 0);
#else
    return false;
#endif
}

bool ComunicadorSerial::estaOperativo()
{
    return conexionActiva;
}
