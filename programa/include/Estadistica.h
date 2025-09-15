#ifndef ESTADISTICA_H
#define ESTADISTICA_H


typedef struct {
    int totalPedidos;
    float montoTotalDeVentas;
    int *pedidosPorAnio; 
    int *cantidadPedidosPorCliente;
    int *cantidadVendidaLibro;
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



void mostrarTotalPedidos(Estadistica* estadistica);
void mostrarMontoPorAnios(Estadistica* estadistica);


#endif 