#ifndef ESTADISTICA_H
#define ESTADISTICA_H

#include "Pedido.h"

/**
 * @struct Estadistica
 * @brief Estructura que representa las estadísticas de ventas del sistema de gestión de librería.
 *
 * Contiene información sobre:
 * - Total de pedidos realizados.
 * - Monto total de ventas.
 * - Pedidos agrupados por año.
 * - Cantidad de pedidos por cliente.
 * - Cantidad vendida de cada libro.
 */
typedef struct {
    int totalPedidos;                  /**< Total de pedidos realizados en el sistema */
    float montoTotalDeVentas;          /**< Monto total acumulado de ventas */
    int *pedidosPorAnio;               /**< Array dinámico: cantidad de pedidos por año                              EJEMPLO: pedidosPorAnio[0] = cantidad de pedidos en 2020 */
    int *cantidadPedidosPorCliente;    /**< Array dinámico: cantidad de pedidos por cliente */
    int *cantidadVendidaLibro;         /**< Array dinámico: cantidad vendida por libro */
    int cantidadLibros;                /**< Cantidad total de libros en el sistema */
} Estadistica;

/**
 * @brief Inicializa la estructura de estadísticas, reservando memoria y estableciendo valores iniciales.
 * 
 * @param estadistica Puntero a la estructura Estadistica a inicializar.
 */
void inicializarEstadistica(Estadistica* estadistica);

/**
 * @brief Muestra un listado detallado de todos los pedidos realizados.
 *
 * El formato de salida es:
 *   Total pedidos: N
 *   Pedido #1
 *     ID: <id>
 *     Fecha: <fecha>
 *     Total: <monto>
 *   -----------------------------
 *   Pedido #2
 *     ...
 *
 * Utiliza el arreglo global de pedidos definido en Pedido.c.
 * 
 * @param estadistica Puntero a la estructura de estadísticas (usa totalPedidos ya inicializado).
 */
void mostrarTotalPedidos(Estadistica* estadistica);

/**
 * @brief Calcula el monto total de ventas realizadas en el sistema.
 * 
 * @param estadistica Puntero a la estructura de estadísticas.
 * @return float Monto total de ventas.
 */
float calcularMontoTotalDeVentas(Estadistica* estadistica);

/**
 * @brief Muestra los pedidos agrupados por mes y año.
 *
 * Presenta cada pedido con su identificador, fecha y monto, organizados en secciones por año y mes.
 * 
 * @param estadistica Puntero a la estructura de estadísticas.
 */
void mostrarPedidosAniosMes(Estadistica* estadistica);

/**
 * @brief Muestra el monto total de ventas agrupado por años.
 * 
 * @param estadistica Puntero a la estructura de estadísticas.
 */
void mostrarMontoPorAnios(Estadistica* estadistica);

/**
 * @brief Muestra los libros más vendidos del sistema.
 *
 * Se indican el código del libro, título y cantidad vendida.
 * 
 * @param arregloPedidos Arreglo de pedidos realizados.
 * @param cantidadPedidos Cantidad de pedidos en el arreglo.
 */
void mostrarLibrosMasVendidos(Pedido* arregloPedidos, int cantidadPedidos);

/**
 * @brief Muestra los clientes que han realizado más pedidos.
 * 
 * La lista se ordena de mayor a menor según la cantidad de pedidos por cliente.
 */
void mostrarClientesConMasPedidos(void);

/**
 * @brief Muestra el autor con más ventas por año.
 *
 * Se realiza un conteo de la cantidad de libros vendidos por autor para cada año.
 */
void mostrarAutorMasVentasPorAnio(void);

#endif
