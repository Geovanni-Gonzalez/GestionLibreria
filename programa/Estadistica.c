#include "Estadistica.h"
#include "Pedido.h"
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

void mostrarTotalPedidos(Estadistica* estadistica) {
    // Asegurar que el total refleje el estado actual
    if (estadistica) {
        estadistica->totalPedidos = cantidadPedidosActual; // sincronia básica
    }

    printf("Total pedidos: %d\n", cantidadPedidosActual);

    if (cantidadPedidosActual == 0) {
        printf("No hay pedidos registrados.\n");
        return;
    }

    for (int i = 0; i < cantidadPedidosActual; i++) {
        Pedido *p = &arregloPedidos[i];
        printf("Pedido #%d\n", i + 1);
        printf("ID: %s\n", p->id);
        printf("Fecha: %s\n", p->fecha);
        printf("Total: %.2f\n", p->total);
        printf("-----------------------------\n");
    }
}

// Imprime los pedidos agrupados por mes y año en el formato requerido
void mostrarPedidosAniosMes(Estadistica* estadistica) {
    (void)estadistica; // no estrictamente necesario aquí, pero mantenemos firma similar al resto
    if (cantidadPedidosActual == 0) {
        printf("Pedidos por mes y año\n");
        printf("No hay pedidos registrados.\n");
        return;
    }

    // Nombres de los meses en español (1..12)
    const char* meses[13] = {
        "", "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };

    printf("Pedidos por mes y año\n");

    // Aqui se recorren los años únicos, y para cada año, recorrer meses 1 al 12
    int* anios = obtenerAniosPedidos(arregloPedidos, cantidadPedidosActual);
    if (!anios) return;

    int cantidadAnios = 0;
    for (int i = 0; i < cantidadPedidosActual; i++) {
        int ai = obtenerAnioDeFecha(arregloPedidos[i].fecha);
        for (int j = 0; ; j++) {
            if (j == cantidadAnios) { cantidadAnios++; break; }
            if (anios[j] == ai) break;
        }
    }

    // Para cada año, imprimimos por mes sólo si hay pedidos en ese mes
    for (int i = 0; i < cantidadAnios; i++) {
        int anio = anios[i];
        for (int mes = 1; mes <= 12; mes++) {
            // Verificar si hay al menos un pedido en (anio, mes)
            int hay = 0;
            for (int k = 0; k < cantidadPedidosActual; k++) {
                if (obtenerAnioDeFecha(arregloPedidos[k].fecha) == anio &&
                    obtenerMesDeFecha(arregloPedidos[k].fecha) == mes) {
                    hay = 1; break;
                }
            }
            if (!hay) continue;

            // Encabezado del grupo
            printf("%s %d\n", meses[mes], anio);
            printf("ID, Total\n");

            // Listado de pedidos del grupo
            for (int k = 0; k < cantidadPedidosActual; k++) {
                if (obtenerAnioDeFecha(arregloPedidos[k].fecha) == anio &&
                    obtenerMesDeFecha(arregloPedidos[k].fecha) == mes) {
                    printf("%s, %.2f\n", arregloPedidos[k].id, arregloPedidos[k].total);
                }
            }

            printf("\n"); // línea en blanco entre grupos
        }
    }

    free(anios);
}