/**
 * @file Config.h
 * @brief Definición de estructuras y funciones para la configuración del sistema de librería.
 *
 * Este archivo contiene las estructuras de datos y las funciones necesarias para
 * manejar la configuración general del sistema, incluyendo la información del local,
 * credenciales de administrador y control del número de pedidos.
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @struct Admin
 * @brief Estructura que almacena las credenciales del administrador del sistema.
 *
 * Contiene el nombre de usuario y la contraseña del administrador, utilizadas
 * para validar el acceso al sistema.
 */
typedef struct {
    char usuario[50];   /**< Nombre de usuario del administrador */
    char password[50];  /**< Contraseña del administrador */
} Admin;

/**
 * @struct Config
 * @brief Estructura que almacena la configuración general del sistema.
 *
 * Incluye la información del local comercial, los datos de contacto,
 * la cédula jurídica, el horario de atención, el número del siguiente pedido
 * y las credenciales del administrador.
 */
typedef struct {
    char nombreLocalComercial[100]; /**< Nombre del local comercial */
    char telefono[20];              /**< Teléfono de contacto */
    char cedulaJuridica[20];        /**< Cédula jurídica del negocio */
    char horarioAtencion[50];       /**< Horario de atención del local */
    int numeroSiguientePedido;      /**< Número consecutivo del próximo pedido */
    Admin admin;                    /**< Datos de acceso del administrador */
} Config;

/**
 * @brief Carga la configuración desde un archivo JSON.
 *
 * Lee el contenido del archivo especificado, lo parsea usando cJSON y copia
 * los valores a la estructura Config. Si algún campo no existe en el JSON,
 * simplemente no se asigna.
 *
 * @param archivo Ruta del archivo de configuración (JSON).
 * @param cfg Puntero a la estructura Config donde se almacenarán los datos cargados.
 * @return 1 si la carga fue exitosa, 0 en caso de error (archivo inexistente o JSON inválido).
 */
int cargarConfig(const char* archivo, Config* cfg);

/**
 * @brief Guarda la configuración actual en un archivo JSON.
 *
 * Convierte la estructura Config en un objeto JSON y lo escribe en el archivo
 * indicado. El archivo será sobrescrito completamente.
 *
 * @param archivo Ruta del archivo de salida (JSON).
 * @param cfg Puntero a la estructura Config con los datos a guardar.
 * @return 1 si se guardó correctamente, 0 en caso de error.
 */
int guardarConfig(const char* archivo, Config* cfg);

/**
 * @brief Incrementa el número del siguiente pedido y actualiza la configuración.
 *
 * Suma 1 al campo numeroSiguientePedido en la estructura Config y guarda el
 * cambio en el archivo fijo "data/admin.json".
 *
 * @param cfg Puntero a la estructura Config que contiene la configuración actual.
 * @return 1 si se actualizó y guardó correctamente, 0 si hubo error.
 */
int actualizarNumeroSiguientePedido(Config* cfg);


#endif /* CONFIG_H */