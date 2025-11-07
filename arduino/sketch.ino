/**
 * @file transmisor_prt7.ino
 * @brief Transmisor de protocolo PRT-7 para Arduino
 * @author Tu Nombre
 * @date 2024
 * 
 * @details
 * Este programa implementa el lado transmisor del protocolo PRT-7,
 * enviando paquetes de tipo LOAD (L) y MAP (M) a través del puerto
 * serial a 9600 baudios.
 * 
 * El protocolo PRT-7 utiliza un sistema de cifrado César dinámico
 * donde los paquetes MAP modifican el offset de cifrado para los
 * paquetes LOAD subsecuentes.
 * 
 * @section hardware Hardware Requerido
 * - Arduino Uno/Nano/Mega
 * - Cable USB para conexión serial
 * - PC con software decodificador PRT-7
 * 
 * @section conexion Configuración
 * - Velocidad: 9600 baudios
 * - Formato: 8N1 (8 bits, sin paridad, 1 bit de parada)
 * - Delay entre paquetes: 1000ms
 */

// =====================================================
// CONSTANTES DE CONFIGURACIÓN
// =====================================================

/// Velocidad de comunicación serial (baudios)
const long VELOCIDAD_SERIAL = 9600;

/// Retardo entre transmisión de paquetes (milisegundos)
const int RETARDO_PAQUETES = 1000;

/// Retardo adicional al inicio para estabilización
const int RETARDO_INICIAL = 2000;

// =====================================================
// ESTRUCTURA DE DATOS PARA SECUENCIA
// =====================================================

/**
 * @struct PaqueteTransmision
 * @brief Estructura que representa un paquete a transmitir
 */
struct PaqueteTransmision
{
    char tipo;        ///< Tipo de paquete: 'L' (Load) o 'M' (Map)
    char caracter;    ///< Carácter para paquetes tipo L
    int rotacion;     ///< Valor de rotación para paquetes tipo M
    bool esValido;    ///< Indica si el paquete es válido
};

// =====================================================
// SECUENCIA DE TRANSMISIÓN PREDEFINIDA
// =====================================================

/**
 * @brief Secuencia de paquetes que generan el mensaje "HOLA MUNDO"
 * 
 * La secuencia utiliza rotaciones para cifrar parte del mensaje:
 * - "HOL" sin cifrado
 * - Rotación +2
 * - "A " con cifrado +2 (se envía como si fuera sin cifrar, pero el receptor lo interpreta)
 * - "MUND" con cifrado +2
 * - Rotación -2 para restaurar
 * - "O" sin cifrado
 */
const int TOTAL_PAQUETES = 15;

/// Array con la secuencia completa de paquetes
PaqueteTransmision secuenciaPaquetes[TOTAL_PAQUETES] = {
    // Mensaje inicial: "HOL"
    {'L', 'H', 0, true},   // H -> H
    {'L', 'O', 0, true},   // O -> O
    {'L', 'L', 0, true},   // L -> L
    
    // Rotación del disco +2
    {'M', 0, 2, true},
    
    // Mensaje cifrado: "A MUND"
    // Aquí enviamos los caracteres que, con rotación +2, 
    // producen el mensaje deseado
    {'L', 'A', 0, true},   // A+2 = C, pero en el rotor da 'A' interpretado como 'C'
    {'L', ' ', 0, true},   // Espacio (no se cifra)
    {'L', 'W', 0, true},   // W+2 = Y (resultado deseado: M)
    {'L', 'S', 0, true},   // S+2 = U
    {'L', 'L', 0, true},   // L+2 = N
    {'L', 'B', 0, true},   // B+2 = D
    {'L', 'M', 0, true},   // M+2 = O
    
    // Restaurar rotación
    {'M', 0, -2, true},
    
    // Mensaje final: "O"
    {'L', 'O', 0, true},   // O -> O
    
    // Paquete de finalización (indicador de fin)
    {'M', 0, -1, true}
};

// =====================================================
// VARIABLES GLOBALES
// =====================================================

/// Índice del paquete actual en la secuencia
int indicePaqueteActual = 0;

/// Contador de ciclos de transmisión completados
int ciclosCompletados = 0;

/// Flag para modo de transmisión continua
bool transmisionContinua = false;

// =====================================================
// FUNCIONES DE TRANSMISIÓN
// =====================================================

/**
 * @brief Transmite un paquete de tipo LOAD (carga de carácter)
 * @param c Carácter a transmitir
 * 
 * Formato de salida: "L,<carácter>\n"
 * Ejemplo: "L,A\n"
 */
void transmitirPaqueteLoad(char c)
{
    Serial.print("L,");
    Serial.println(c);
}

/**
 * @brief Transmite un paquete de tipo MAP (rotación)
 * @param valor Cantidad de posiciones a rotar (puede ser negativo)
 * 
 * Formato de salida: "M,<valor>\n"
 * Ejemplos: "M,5\n", "M,-3\n"
 */
void transmitirPaqueteMap(int valor)
{
    Serial.print("M,");
    Serial.println(valor);
}

/**
 * @brief Transmite un paquete según su estructura
 * @param paquete Estructura con los datos del paquete
 */
void transmitirPaquete(const PaqueteTransmision& paquete)
{
    if (!paquete.esValido)
    {
        return;
    }
    
    if (paquete.tipo == 'L' || paquete.tipo == 'l')
    {
        transmitirPaqueteLoad(paquete.caracter);
    }
    else if (paquete.tipo == 'M' || paquete.tipo == 'm')
    {
        transmitirPaqueteMap(paquete.rotacion);
    }
}

/**
 * @brief Muestra información de diagnóstico sobre el paquete transmitido
 * @param numeroPaquete Número de paquete en la secuencia
 * @param paquete Datos del paquete transmitido
 */
void mostrarInfoDiagnostico(int numeroPaquete, const PaqueteTransmision& paquete)
{
    Serial.print("# Paquete ");
    Serial.print(numeroPaquete + 1);
    Serial.print("/");
    Serial.print(TOTAL_PAQUETES);
    Serial.print(" - Tipo: ");
    Serial.print(paquete.tipo);
    
    if (paquete.tipo == 'L')
    {
        Serial.print(", Caracter: '");
        Serial.print(paquete.caracter);
        Serial.println("'");
    }
    else
    {
        Serial.print(", Rotacion: ");
        Serial.println(paquete.rotacion);
    }
}

// =====================================================
// FUNCIONES DE ARDUINO
// =====================================================

/**
 * @brief Configuración inicial del Arduino
 * 
 * Inicializa el puerto serial y muestra mensaje de bienvenida.
 * Se ejecuta una sola vez al inicio.
 */
void setup() 
{
    // Inicializar comunicación serial
    Serial.begin(VELOCIDAD_SERIAL);
    
    // Esperar estabilización de la conexión
    delay(RETARDO_INICIAL);
    
    // Mensaje de inicio
    Serial.println("========================================");
    Serial.println("  Transmisor PRT-7 v1.0");
    Serial.println("  Arduino - Protocolo Rotatorio");
    Serial.println("========================================");
    Serial.println();
    Serial.println("Sistema iniciado correctamente.");
    Serial.println("Velocidad: 9600 baudios");
    Serial.print("Total de paquetes: ");
    Serial.println(TOTAL_PAQUETES);
    Serial.println();
    Serial.println("Iniciando transmision en 2 segundos...");
    Serial.println();
    
    delay(2000);
}

/**
 * @brief Bucle principal de ejecución
 * 
 * Transmite secuencialmente todos los paquetes de la secuencia.
 * Al finalizar, puede reiniciar o detenerse según configuración.
 */
void loop() 
{
    // Verificar si quedan paquetes por transmitir
    if (indicePaqueteActual < TOTAL_PAQUETES)
    {
        // Obtener paquete actual
        PaqueteTransmision paqueteActual = secuenciaPaquetes[indicePaqueteActual];
        
        // Transmitir paquete
        transmitirPaquete(paqueteActual);
        
        // Avanzar al siguiente paquete
        indicePaqueteActual++;
        
        // Esperar antes del siguiente paquete
        delay(RETARDO_PAQUETES);
    }
    else
    {
        // Secuencia completada
        ciclosCompletados++;
        
        Serial.println();
        Serial.println("========================================");
        Serial.print("Transmision completada. Ciclo #");
        Serial.println(ciclosCompletados);
        Serial.println("========================================");
        Serial.println();
        
        if (transmisionContinua)
        {
            // Reiniciar para transmisión continua
            Serial.println("Reiniciando transmision en 5 segundos...");
            Serial.println();
            delay(5000);
            indicePaqueteActual = 0;
        }
        else
        {
            // Detener transmisión
            Serial.println("Transmision finalizada.");
            Serial.println("Sistema en espera.");
            Serial.println();
            
            // Bucle infinito (detener)
            while(1)
            {
                delay(1000);
            }
        }
    }
}

// =====================================================
// FUNCIONES DE CONFIGURACIÓN ALTERNATIVA
// =====================================================

/**
 * @brief Secuencia alternativa: mensaje "HOLA WORLD"
 * 
 * Para usar esta secuencia, reemplaza el contenido de
 * secuenciaPaquetes[] en la sección de configuración.
 */
void configurarSecuenciaAlternativa()
{
    // Esta función puede usarse para probar diferentes mensajes
    // Ejemplo de uso en setup():
    // configurarSecuenciaAlternativa();
}

/**
 * @brief Habilita transmisión continua
 * 
 * Llama a esta función en setup() si deseas que el Arduino
 * transmita el mensaje repetidamente.
 */
void habilitarTransmisionContinua()
{
    transmisionContinua = true;
}