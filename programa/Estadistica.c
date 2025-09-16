#include "Estadistica.h"
#include "Pedido.h"
#include "Utilidades.h"
#include <stdlib.h>
#include <stdio.h>

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

void mostrarLibrosMasVendidos(Pedido* arregloPedidos, int cantidadPedidos) {
    int* cantidadesVendidas = NULL;
    char** codigosLibros = NULL;
    int totalLibros = 0;

    // 1 y 2: recorrer pedidos y sus libros
    for (int i = 0; i < cantidadPedidos; i++) {
        Pedido pedido = arregloPedidos[i];
        for (int j = 0; j < pedido.cantidadLibros; j++) {
            char* codigo = pedido.libros[j].codigo;
            int cantidad = pedido.cantidadPorLibro[j];

            // 3: verificar si ya existe
            int indice = buscarIndiceLibro(codigosLibros, totalLibros, codigo);
            if (indice != -1) {
                cantidadesVendidas[indice] += cantidad;
            } else {
                agregarLibroEstadistica(&codigosLibros, &cantidadesVendidas, &totalLibros, codigo, cantidad);
            }
        }
    }

    // 4. Ordenar por cantidad vendida descendente
    ordenarLibrosVendidos(codigosLibros, cantidadesVendidas, totalLibros);

    // 5. Mostrar resultados
    mostrarResultados(codigosLibros, cantidadesVendidas, totalLibros);

    // liberar memoria
    for (int i = 0; i < totalLibros; i++) {
        free(codigosLibros[i]);
    }
    free(codigosLibros);
    free(cantidadesVendidas);
}