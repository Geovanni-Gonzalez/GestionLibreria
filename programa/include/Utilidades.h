/**
 * @file Utilidades.h
 * @brief Declaraciones de funciones utilitarias para el sistema de gestión de librería.
 *
 * Contiene funciones para:
 * - Validación de datos de clientes.
 * - Manejo de pedidos (generación de ID, cálculo de precios, stock).
 * - Manejo de clientes y carga desde archivo.
 * - Manejo de lectura de archivos y utilidades de strings.
 */

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdbool.h>
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Config.h"
#include <stddef.h>

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
 * @brief Genera un ID único para un pedido basado en el total actual de pedidos.
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
 * @brief Descuenta el stock de los libros incluidos en un pedido y actualiza el inventario.
 *
 * @param libro Arreglo de libros del pedido.
 * @param cantidadPorLibro Arreglo con las cantidades de cada libro.
 * @param cantidadLibros Número de libros en el pedido.
 */
void descontarStockLibro(Libro *libro, int *cantidadPorLibro, int cantidadLibros);

/**
 * @brief Carga pedidos desde un archivo de texto en un arreglo dinámico.
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
 * @brief Carga los clientes desde un archivo a un arreglo dinámico.
 *
 * Redimensiona el arreglo automáticamente si se llena.
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

/**
 * @brief Convierte una cadena a entero.
 *
 * @param str Cadena a convertir.
 * @return Valor entero resultante.
 */
int stringAInt(const char* str);

/**
 * @brief Convierte una cadena a flotante.
 *
 * @param str Cadena a convertir.
 * @return Valor flotante resultante.
 */
float stringAFloat(const char* str);

/**
 * @brief Duplica una cadena en memoria dinámica.
 *
 * @param src Cadena fuente a copiar.
 * @return Puntero a la copia (debe liberarse con free).
 */
char* copiarString(const char* src);

/**
 * @brief Elimina espacios en blanco al inicio y al final de la cadena.
 *
 * @param s Cadena a recortar.
 */
void trim(char* s);

/**
 * @brief Lee una línea desde consola con un límite máximo de caracteres.
 *
 * @param prompt Mensaje a mostrar al usuario.
 * @param maxLen Máximo número de caracteres a leer.
 * @return Puntero a la cadena leída (debe liberarse con free).
 */
char* leerLineaAlloc(const char* prompt, size_t maxLen);

/**
 * @brief Aplica un pedido al inventario, descontando las cantidades.
 *
 * @param inventario Arreglo de libros del inventario.
 * @param totalLibros Cantidad total de libros.
 * @param pedido Pedido a aplicar al inventario.
 */
void aplicarPedidoAlInventario(Libro* inventario, int totalLibros, const Pedido* pedido);

/**
 * @brief Revierta el descuento de stock de un pedido (aumenta stock).
 *
 * @param inventario Arreglo de libros del inventario.
 * @param totalLibros Cantidad total de libros.
 * @param pedido Pedido a revertir.
 */
void revertirPedidoDelInventario(Libro* inventario, int totalLibros, const Pedido* pedido);

/**
 * @brief Lectura segura de una línea en un buffer fijo.
 *
 * @param dst Destino donde se guardará la línea.
 * @param dstSize Tamaño del buffer destino.
 */
void leerLineaSeguro(char *dst, size_t dstSize);

/**
 * @brief Valida que una cédula tenga exactamente 9 dígitos.
 *
 * @param s Cadena a validar.
 * @return true si cumple el formato, false en caso contrario.
 */
bool validarCedula9(const char* s);

/**
 * @brief Valida que una fecha esté en formato AAAAMMDD.
 *
 * @param s Cadena a validar.
 * @return true si cumple el formato, false en caso contrario.
 */
bool validarFechaYYYYMMDD(const char* s);

/**
 * @brief Limpia el buffer de entrada estándar.
 */
void limpiar_stdin(void);

/**
 * @brief Limpia el carácter de fin de línea de una cadena.
 *
 * @param s Cadena a limpiar.
 */
void limpiarFinLinea(char *s);

/**
 * @brief Obtiene el título de un libro a partir de su código desde los pedidos.
 *
 * @param codigo Código del libro.
 * @param arregloPedidos Arreglo de pedidos.
 * @param cantidadPedidos Número de pedidos en el arreglo.
 * @return Puntero al título (no modifica memoria original).
 */
char* obtenerTituloPorCodigo(const char* codigo, Pedido* arregloPedidos, int cantidadPedidos);

/**
 * @brief Busca el índice de un libro en un arreglo de códigos.
 *
 * @param codigosLibros Arreglo de códigos de libros.
 * @param totalLibros Cantidad de libros en el arreglo.
 * @param codigo Código a buscar.
 * @return Índice del libro si existe, -1 si no se encuentra.
 */
int buscarIndiceLibro(char** codigosLibros, int totalLibros, const char* codigo);

/**
 * @brief Agrega un libro a la estructura de estadísticas de libros vendidos.
 *
 * @param codigosLibros Puntero al arreglo de códigos de libros vendidos.
 * @param cantidadesVendidas Puntero al arreglo de cantidades vendidas por libro.
 * @param totalLibros Puntero a la cantidad de libros únicos.
 * @param codigo Código del libro a agregar.
 * @param cantidad Cantidad vendida a agregar.
 */
void agregarLibroEstadistica(char*** codigosLibros, int** cantidadesVendidas, int* totalLibros, const char* codigo, int cantidad);

/**
 * @brief Ordena los libros vendidos de mayor a menor según la cantidad.
 *
 * @param codigosLibros Arreglo de códigos de libros.
 * @param cantidadesVendidas Arreglo de cantidades vendidas por libro.
 * @param totalLibros Número total de libros únicos.
 */
void ordenarLibrosVendidos(char** codigosLibros, int* cantidadesVendidas, int totalLibros);

/**
 * @brief Muestra los resultados de ventas de libros en consola.
 *
 * @param codigosLibros Arreglo de códigos de libros.
 * @param cantidadesVendidas Arreglo de cantidades vendidas.
 * @param totalLibros Número total de libros.
 */
void mostrarResultados(char** codigosLibros, int* cantidadesVendidas, int totalLibros);

/**
 * @brief Limpieza del buffer de entrada estándar (equivalente a limpiar_stdin).
 */
void limpiarStdin(void);

#endif /* UTILIDADES_H */