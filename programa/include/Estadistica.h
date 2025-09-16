#ifndef ESTADISTICA_H
#define ESTADISTICA_H
#include "Pedido.h"


typedef struct {
    int totalPedidos;
    float montoTotalDeVentas;
    int *pedidosPorAnio; // Array dinámico para almacenar la cantidad de pedidos por año    EJEMPLO: pedidosPorAnio[0] = cantidad de pedidos en 2020 
    int *cantidadPedidosPorCliente; // Array dinámico para almacenar la cantidad de pedidos por cliente
    int *cantidadVendidaLibro; // Array dinámico para almacenar la cantidad vendida por libro
    int cantidadLibros;
} Estadistica;


/**
 * @struct Estadistica
 * @brief Estructura que representa las estadísticas de ventas en el sistema de gestión de librería.
 *
 * Contiene información sobre el total de pedidos, monto total de ventas, pedidos por año,
 * cantidad de pedidos por cliente y cantidad vendida por libro.
 */

void inicializarEstadistica(Estadistica* estadistica);

/**
 * @brief Muestra un listado detallado de todos los pedidos realizados.
 *
 * Formato de salida:
 *   Total pedidos: N
 *   Pedido #1
 *   ID: <id>
 *   Fecha: <fecha>
 *   Total: <monto>
 *   -----------------------------
 *   Pedido #2
 *   ...
 *
 * Usa el arreglo global de pedidos definido en Pedido.c
 * @param estadistica Puntero a la estructura de estadísticas (usa totalPedidos ya inicializado).
 */
void mostrarTotalPedidos(Estadistica* estadistica);

float calcularMontoTotalDeVentas(Estadistica* estadistica);

// Muestra los pedidos agrupados por mes y año con formato solicitado
void mostrarPedidosAniosMes(Estadistica* estadistica);

void mostrarMontoPorAnios(Estadistica* estadistica);

void mostrarLibrosMasVendidos(Pedido* arregloPedidos, int cantidadPedidos);


void mostrarClientesConMasPedidos(void);

void mostrarAutorMasVentasPorAnio(void);


#endif 