#include "Estadistica.h"

void mostrarTotalPedidos(Estadistica* estadistica) {
    printf("Total de Pedidos: %d\n", estadistica->totalPedidos);
}

void mostrarMontoPorAnios(Estadistica* estadistica, int anioInicio, int anioFin) {
    printf("Pedidos por Año:\n");
    for (int i = 0; i <= (anioFin - anioInicio); i++) {
        printf("Año %d: %d pedidos\n", anioInicio + i, estadistica->pedidosPorAnio[i]);
    }
}