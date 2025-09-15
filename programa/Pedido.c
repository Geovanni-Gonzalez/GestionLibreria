#include "Pedido.h"
#include "Utilidades.h"
#include "Cliente.h"    
#include "Config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Declaración de variables globales accesibles externamente
Pedido* arregloPedidos = NULL;     // Arreglo dinámico
int cantidadPedidosActual = 0;      // Cantidad actual de pedidos
int capacidadDePedidosArreglo = 2;  // Tamaño inicial del arreglo


void inicializarArregloPedidos() {
    arregloPedidos = (Pedido*) malloc(capacidadDePedidosArreglo * sizeof(Pedido));
    if (arregloPedidos == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    cargarPedidosDesdeArchivo(&arregloPedidos, &cantidadPedidosActual, &capacidadDePedidosArreglo);
}


/***
 * @brief Remueve un libro del la lista mientras se crea un pedido.
 * @param codigoLibro Código del libro a remover.
 * @param pedido Puntero al pedido del cual se desea remover el libro.
 * @return void 
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido) {
    int indice = -1;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (strcmp(pedido->libros[i].codigo, codigoLibro) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Código de libro no encontrado en el pedido.\n");
        return;
    }


    if (indice != -1) {
        for (int i = indice; i < pedido->cantidadLibros - 1; i++) {
            pedido->libros[i] = pedido->libros[i + 1];
        }
        pedido->cantidadLibros--;
        pedido->libros = realloc(pedido->libros, pedido->cantidadLibros * sizeof(Libro));
    }

    printf("Libro con código %s removido del pedido exitosamente.\n", codigoLibro);
}


void mostrarDetallePedido(Pedido* pedido, Config cfg) {
    Cliente* cliente = obtenerClientePorCedula(pedido->cedula);
    if (!cliente) {
        printf("Cliente no encontrado.\n");
        return;
    }

    printf("\n--- DETALLE PEDIDO ---\n");
    printf("ID del Pedido: %s\n", pedido->id);
    printf("Fecha: %s\n", pedido->fecha);
    printf("Cliente: %s (Cédula: %s, Tel: %s)\n", cliente->nombre, cliente->cedula, cliente->telefono);
    printf("Subtotal: %.2f\n", pedido->subtotal);
    printf("Impuesto (13%%): %.2f\n", pedido->impuesto);
    printf("Total: %.2f\n", pedido->total);
    printf("--- %s ---\n", cfg.nombreLocalComercial);
    printf("Teléfono: %s | Cédula Jurídica: %s\n", cfg.telefono, cfg.cedulaJuridica);
    printf("Horario de Atención: %s\n", cfg.horarioAtencion);
    printf("----------------------\n");

}

void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9], Pedido* arregloPedidos, int* cantidadPedidosActual, Config cfg) {

    // 1. Generar ID del pedido
    char* idGenerado = generarIDPedido(cantidadPedidosActual);
    strcpy(pedido->id, idGenerado);
    free(idGenerado);
    // 2. Asociar cedula y fecha al pedido
    strcpy(pedido->cedula, cedulaCliente);
    strcpy(pedido->fecha, fechaPedido);

    // 3. Se calcula subtotal, impuesto y total
    calcularPreciosPedido(pedido);

    // 4. Marcar el pedido como generado
    pedido->generado = true;

    // 5. Descontar el stock de los libros en el pedido
    descontarStockLibro(pedido->libros, pedido->cantidadPorLibro, pedido->cantidadLibros);

    // 6. Agregar el pedido al arreglo de pedidos y actualizar la cantidad
    arregloPedidos[*cantidadPedidosActual] = *pedido;
    (*cantidadPedidosActual)+=1;

   //Mostrar detalle del pedido (Generar factura)
    mostrarDetallePedido(pedido, cfg);

}

Pedido* obtenerPedidosPorCliente(char* cedulaCliente, Pedido* arregloPedidos, int cantidadPedidosActual, int* cantidadPedidosCliente){
    Pedido* pedidosCliente = NULL;
    *cantidadPedidosCliente = 0;

    for (int i = 0; i < cantidadPedidosActual; i++) {
        if (strcmp(arregloPedidos[i].cedula, cedulaCliente) == 0) {
            pedidosCliente = realloc(pedidosCliente, sizeof(Pedido) * (*cantidadPedidosCliente + 1));
            if (!pedidosCliente) {
                printf("Error al asignar memoria para los pedidos del cliente.\n");
                *cantidadPedidosCliente = 0;
                return NULL;
            }
            pedidosCliente[*cantidadPedidosCliente] = arregloPedidos[i];
            (*cantidadPedidosCliente)++;
        }
    }

    return pedidosCliente;
}


// Retornar direccion de variable cantidadPedidosActual
int* cantidadPedidos() {
    return &cantidadPedidosActual;
}

// Formato fecha DD/MM/AAAA
int obtenerAnioDeFecha(const char* fecha) {
    if (fecha == NULL || strlen(fecha) < 10) return -1;
    char anio[5] = {0};
    strncpy(anio, fecha + 6, 4);
    return atoi(anio);
}


int obtenerMontoPorAnio(int anio, Pedido* arregloPedidos, int cantidadPedidos) {
    int montoTotal = 0;

    for (int i = 0; i < cantidadPedidos; i++) {
        if (obtenerAnioDeFecha(arregloPedidos[i].fecha) == anio) {
            montoTotal += arregloPedidos[i].total;
        }
    }

    return montoTotal;
}


//Funcion que retorna un array de los anios que existen en los pedidos
int* obtenerAniosPedidos(Pedido* arregloPedidos, int cantidadPedidos) {
    int* anios = NULL;
    int* cantidadAnios = malloc(sizeof(int));
    *cantidadAnios = 0;

    for (int i = 0; i < cantidadPedidos; i++) {
        int anio = obtenerAnioDeFecha(arregloPedidos[i].fecha);
        bool existe = false;
        for (int j = 0; j < *cantidadAnios; j++) {
            if (anios[j] == anio) {
                existe = true;
                break;
            }
        }
        if (!existe) {
            anios = realloc(anios, sizeof(int) * (*cantidadAnios + 1));
            if (!anios) {
                printf("Error al asignar memoria para los años.\n");
                *cantidadAnios = 0;
                return NULL;
            }
            anios[*cantidadAnios] = anio;
            (*cantidadAnios)++;
        }
    }

    return anios;
}



