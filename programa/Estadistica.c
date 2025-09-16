#include "Estadistica.h"
#include "Cliente.h"
#include "Pedido.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void inicializarEstadistica(Estadistica* estadistica) {
    //1. Asigna la cantidad de pedidos totales con una función que retorne la dirección de la variable cantidadPedidosActual en Pedido.c
    estadistica->totalPedidos = *cantidadPedidos(); 
    //2. Asigna el monto total de ventas con una función que calcule el monto total de ventas en base al arreglo de pedidos.
    estadistica->montoTotalDeVentas = calcularMontoTotalDeVentas(estadistica);
    //3. Inicializa el arreglo pedidosPorAnio con memoria dinámica

    //4. Inicializa el arreglo cantidadPedidosPorCliente con memoria dinámica

    //5. Inicializa el arreglo cantidadVendidaLibro con memoria dinámica

    //6. Asigna la cantidad de libros disponibles en el sistema 


}

void mostrarTotalPedidos(Estadistica* estadistica) {
    printf("Total de Pedidos: %d\n", estadistica->totalPedidos);
}



float calcularMontoTotalDeVentas(Estadistica* estadistica) {
    float montoTotal = 0.0f;
    for (int i = 0; i < cantidadPedidos; i++) {
        montoTotal += arregloPedidos[i].total;
    }
    return montoTotal;  
}

void mostrarMontoPorAnios(Estadistica* estadistica) {
    //1. Obtener los años únicos de los pedidos usando la función obtenerAniosPedidos
    int* anios = obtenerAniosPedidos(arregloPedidos, cantidadPedidosActual);

    //2. Imprimir total de pedidos
    printf("Total de pedidos: %d\n", estadistica->totalPedidos);

    //3. Mostrar el monto total de ventas por año
    for (int i = 0; anios[i] != -1; i++) {
        printf("--------------------------\n");
        int anio = anios[i];
        float monto = obtenerMontoPorAnio(anio, arregloPedidos, cantidadPedidosActual);
        printf("Año %d: %.2f\n", anio, monto);
        printf("--------------------------\n");
    }

    //4. Liberar la memoria del arreglo de años
    free(anios);
}
// Struct auxiliar solo para este archivo
typedef struct {
    Cliente* cliente;
    int      cantidadPedidos;
} ClienteConteo;
static int cmp_conteo_desc(const void* a, const void* b) {
    const ClienteConteo* ca = a;
    const ClienteConteo* cb = b;
    return cb->cantidadPedidos - ca->cantidadPedidos;
}

void mostrarClientesConMasPedidos(void) {
    // Obtener vistas a clientes y pedidos
    int nClientes = clientes_count();
    Cliente* clientes = clientes_data();

    int nPedidos = pedidos_count();
    Pedido* pedidos = pedidos_data();

    if (nClientes <= 0 || !clientes) {
        puts("No hay clientes registrados.");
        return;
    }

    // Construir lista de conteos
    ClienteConteo* lista = (ClienteConteo*)calloc((size_t)nClientes, sizeof(ClienteConteo));
    if (!lista) { perror("calloc"); return; }

    for (int i = 0; i < nClientes; i++) {
        int cont = 0;
        // Contar pedidos que tengan la misma cédula
        for (int j = 0; j < nPedidos; j++) {
            if (strcmp(pedidos[j].cedula, clientes[i].cedula) == 0) {
                cont++;
            }
        }
        lista[i].cliente = &clientes[i];
        lista[i].cantidadPedidos = cont;
    }

    // Ordenar de mayor a menor
    qsort(lista, (size_t)nClientes, sizeof(ClienteConteo), cmp_conteo_desc);

    // Imprimir
    printf("\n=== Clientes con más pedidos ===\n");
    printf("%-20s %-12s %s\n", "Nombre", "Cédula", "Pedidos");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < nClientes; i++) {
        printf("%-20s %-12s %d\n",
               lista[i].cliente->nombre,
               lista[i].cliente->cedula,
               lista[i].cantidadPedidos);
    }

    free(lista);
}