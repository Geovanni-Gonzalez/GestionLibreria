/**
 * @file Cliente.h
 * @brief Declaraciones y definiciones para la gestión de clientes en el sistema de librería.
 *
 * Este archivo contiene la definición de la estructura Cliente y las funciones asociadas
 * para inicializar, registrar, validar y mostrar clientes, así como liberar la memoria utilizada.
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdbool.h>

/**
 * @def MAX_CEDULA
 * @brief Longitud máxima permitida para el campo cédula del cliente (incluyendo el carácter nulo).
 */
#define MAX_CEDULA 10  // 9 dígitos + '\0'

/**
 * @def MAX_NOMBRE
 * @brief Longitud máxima permitida para el campo nombre del cliente (incluyendo el carácter nulo).
 */
#define MAX_NOMBRE 50

/**
 * @def MAX_TELEFONO
 * @brief Longitud máxima permitida para el campo teléfono del cliente (incluyendo el carácter nulo).
 */
#define MAX_TELEFONO 9

/**
 * @struct Cliente
 * @brief Estructura que representa a un cliente.
 *
 * Contiene información básica de un cliente: cédula, nombre y teléfono.
 */
typedef struct {
    char cedula[MAX_CEDULA];   /**< Cédula de identidad del cliente (única). */
    char nombre[MAX_NOMBRE];   /**< Nombre completo del cliente. */
    char telefono[MAX_TELEFONO]; /**< Número de teléfono del cliente (solo dígitos). */
} Cliente;


/**
 * @brief Registra un nuevo cliente con los datos proporcionados.
 *
 * Realiza validaciones: cédula única y formato del teléfono (solo dígitos y longitud mínima).
 * Si el arreglo dinámico está lleno, se redimensiona automáticamente.
 *
 * @param cedula Cédula del cliente (cadena terminada en '\0').
 * @param nombre Nombre del cliente (cadena terminada en '\0').
 * @param telefono Teléfono del cliente (cadena terminada en '\0').
 * @return true si el registro fue exitoso, false si la cédula ya existe o el teléfono es inválido.
 * @pre inicializarClientes() debe haberse llamado previamente.
 * @post El cliente se agrega al arreglo dinámico si es válido.
 */
bool registrarCliente(const char* cedula, const char* nombre, const char* telefono);

/**
 * @brief Muestra la lista de clientes registrados en la consola.
 *
 * Imprime cada cliente con cédula, nombre y teléfono.
 * @pre inicializarClientes() debe haberse llamado previamente.
 */
void mostrarClientes();

/**
 * @brief Libera la memoria utilizada por la estructura o lista dinámica de clientes.
 *
 * Debe llamarse al finalizar el programa para evitar fugas de memoria.
 *
 * @post La memoria asignada a la lista de clientes queda liberada.
 */
void liberarMemoriaClientes();


Cliente *obtenerClientePorCedula(const char *cedula);


void agregarClienteArchivo(const char* cedula, const char* nombre, const char* telefono);


#endif