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



#endif 