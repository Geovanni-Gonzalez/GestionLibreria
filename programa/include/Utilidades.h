/**
 * @file Utilidades.h
 * @brief Declaraciones de funciones utilitarias para el sistema de gestión de librería
 *
 * Este archivo contiene funciones para:
 * - Validación de datos de clientes
 * - Manejo de pedidos (generación de ID, cálculo de precios, stock)
 * - Manejo de clientes y carga desde archivo
 * - Manejo de lectura de archivos
 */

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdbool.h>
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Config.h"


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

/**
 * @brief Carga pedidos desde archivo en un arreglo dinámico.
 * 
 * @param arregloPedidos Puntero al arreglo dinámico de pedidos.
 * @param cantidadPedidosActual Puntero al número actual de pedidos.
 * @param capacidadDePedidosArreglo Puntero a la capacidad actual del arreglo.
 */
void cargarPedidosDesdeArchivo(Pedido** arregloPedidos, int* cantidadPedidosActual, int* capacidadDePedidosArreglo);


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


/**
 * @brief Genera un código único para un libro en base a los existentes.
 * 
 * @param libros Arreglo de libros actuales.
 * @param totalLibros Número de libros existentes.
 * @param nuevoCodigo Cadena donde se guardará el código generado.
 * @return 1 si se generó correctamente, 0 en caso de error.
 */
int generarCodigoUnicoLibro(Libro* libros, int totalLibros, char* nuevoCodigo);

int stringAInt(const char* str);
char* copiarString(const char* src);
float stringAFloat(const char* str);

char* obtenerTituloPorCodigo(const char* codigo, Pedido* arregloPedidos, int cantidadPedidos);
int buscarIndiceLibro(char** codigosLibros, int totalLibros, const char* codigo);
void agregarLibroEstadistica(char*** codigosLibros, int** cantidadesVendidas, int* totalLibros, const char* codigo, int cantidad);
void ordenarLibrosVendidos(char** codigosLibros, int* cantidadesVendidas, int totalLibros);
void mostrarResultados(char** codigosLibros, int* cantidadesVendidas, int totalLibros);



#endif /* UTILIDADES_H */
