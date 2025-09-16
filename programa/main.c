
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>   
#include "Pedido.h"
#include "Libro.h"
#include "Cliente.h"
#include "Config.h"

int main(void) {
    Config cfg = (Config){0};

    // 1) Inicializa el arreglo de clientes en memoria
    inicializarArregloClientes();

    // 2) Registra clientes de prueba en el arreglo 
    registrarCliente("222", "Juan Pérez",  "88881111");
    registrarCliente("111", "María Gómez", "88882222");


    // 3) Inventario de ejemplo
    Libro libro1 = {
        .codigo   = strdup("LIB001"),
        .titulo   = strdup("El Principito"),
        .autor    = strdup("Antoine de Saint-Exupéry"),
        .precio   = 3000.0f,
        .cantidad = 10
    };
    Libro libro2 = {
        .codigo   = strdup("LIB002"),
        .titulo   = strdup("Cien Años de Soledad"),
        .autor    = strdup("Gabriel García Márquez"),
        .precio   = 5000.0f,
        .cantidad = 5
    };

    // 4) Pedidos de ejemplo
    int cantidadPedidos = 2;
    Pedido* pedidos = calloc(cantidadPedidos, sizeof(Pedido));
    if (!pedidos) { perror("calloc pedidos"); return 1; }

    // Pedido 1 (cliente 111)
    strcpy(pedidos[0].id, "P0001");
    strcpy(pedidos[0].cedula, "111");
    strcpy(pedidos[0].fecha,  "2025-09-15");
    pedidos[0].cantidadLibros = 1;
    pedidos[0].libros = malloc(sizeof(Libro));
    pedidos[0].cantidadPorLibro = malloc(sizeof(int));
    pedidos[0].libros[0] = libro1;        // copia superficial
    pedidos[0].cantidadPorLibro[0] = 2;
    pedidos[0].subtotal = libro1.precio * pedidos[0].cantidadPorLibro[0];
    pedidos[0].impuesto = pedidos[0].subtotal * 0.13f;
    pedidos[0].total    = pedidos[0].subtotal + pedidos[0].impuesto;

    // Pedido 2 (cliente 111)
    strcpy(pedidos[1].id, "P0002");
    strcpy(pedidos[1].cedula, "111");
    strcpy(pedidos[1].fecha,  "2025-09-16");
    pedidos[1].cantidadLibros = 1;
    pedidos[1].libros = malloc(sizeof(Libro));
    pedidos[1].cantidadPorLibro = malloc(sizeof(int));
    pedidos[1].libros[0] = libro2;        // copia superficial
    pedidos[1].cantidadPorLibro[0] = 1;
    pedidos[1].subtotal = libro2.precio * pedidos[1].cantidadPorLibro[0];
    pedidos[1].impuesto = pedidos[1].subtotal * 0.13f;
    pedidos[1].total    = pedidos[1].subtotal + pedidos[1].impuesto;

    // 5) Consulta de pedidos
    listarPedidosCLI(pedidos, cantidadPedidos, cfg);

    // 6) Liberación de memoria de pedidos/libros
    for (int i = 0; i < cantidadPedidos; i++) {
        for (int j = 0; j < pedidos[i].cantidadLibros; j++) {
            free(pedidos[i].libros[j].codigo);
            free(pedidos[i].libros[j].titulo);
            free(pedidos[i].libros[j].autor);
        }
        free(pedidos[i].libros);
        free(pedidos[i].cantidadPorLibro);
    }
    free(pedidos);

    // 7) 
    liberarMemoriaClientes();

    return 0;
}
