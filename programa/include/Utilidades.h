/**
 * @file Utilidades.h
 * @brief Declaraciones de funciones utilitarias para el sistema de gestión de librería
 *
 * Este archivo contiene funciones para:
 * - Validación de datos de clientes
 * - Manejo de pedidos
 * - Manejo de clientes y carga desde archivo
 */

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdbool.h>
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Config.h"

/*******************************
 * Funciones de validación
 *******************************/

/**
 * @file Utilidades.h
 * @brief Declaraciones de funciones utilitarias para validación de datos.
 *
 * Este archivo contiene las declaraciones de funciones para validar
 * cédulas únicas y números de teléfono.
 */ 

/**
 * @brief Verifica si la cédula proporcionada es única entre los clientes registrados.
 * 
 * @param cedula Cédula a validar.
 * @param arregloClientes Arreglo de clientes existentes.
 * @param cantidadDeClientes Número de clientes en el arreglo.
 * @return true si la cédula es única, false si ya existe.
 */
bool validarCedulaUnica(const char* cedula, Cliente* arregloClientes, int cantidadDeClientes);

/**
 * @brief Valida si el número de teléfono proporcionado cumple con el formato esperado.
 * 
 * @param telefono Cadena de caracteres que representa el número de teléfono.
 * @return true si el número de teléfono es válido, false en caso contrario.
 */
bool validarTelefono(const char* telefono);


int generarCodigoUnicoLibro(Libro* libros, int totalLibros, char* nuevoCodigo);/*******************************
 * Funciones para pedidos
 *******************************/

/**
 * @brief Genera un ID para un pedido basado en el total actual de pedidos.
 * 
 * @param totalPedidos Puntero al número total de pedidos existentes.
 * @return Cadena de caracteres con el ID generado (debe liberarse con free después de usar).
 */
char* generarIDPedido(int *totalPedidos);

/**
 * @brief Calcula subtotal, impuesto y total de un pedido.
 * 
 * @param pedido Puntero al pedido a calcular.
 */
void calcularPreciosPedido(Pedido *pedido);

/**
 * @brief Descuenta el stock de los libros incluidos en un pedido y actualiza el archivo "data/libros.txt".
 * 
 * @param libro Arreglo de libros del pedido.
 * @param cantidadPorLibro Arreglo con las cantidades de cada libro.
 * @param cantidadLibros Número de libros en el pedido.
 */
void descontarStockLibro(Libro *libro, int *cantidadPorLibro, int cantidadLibros);

/*******************************
 * Funciones para clientes
 *******************************/

/**
 * @brief Lee un archivo completo y retorna su contenido como cadena.
 * 
 * @param nombre Ruta del archivo a leer.
 * @return Puntero a cadena con el contenido del archivo (debe liberarse con free), o NULL si falla.
 */
char* leerArchivo(const char* nombre);

/**
 * @brief Carga los clientes desde el archivo "data/clientes.txt" a un arreglo dinámico.
 * 
 * Si el arreglo se llena, se redimensiona automáticamente.
 * 
 * @param arregloClientes Puntero al arreglo dinámico de clientes.
 * @param cantidadClientesActual Puntero al número actual de clientes.
 * @param capacidadDeClientesArreglo Puntero a la capacidad actual del arreglo.
 */
void cargarClientesDesdeArchivo(Cliente** arregloClientes, int* cantidadClientesActual, int* capacidadDeClientesArreglo);

#endif /* UTILIDADES_H */