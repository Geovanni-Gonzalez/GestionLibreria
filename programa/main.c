#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Estadistica.h"

int main(void) {
    /* ===== 1. Inicializar y registrar clientes ===== */
    inicializarArregloClientes();
    registrarCliente("111", "María Gómez", "8888-2222");
    registrarCliente("222", "Juan Pérez",  "8888-1111");

    /* ===== 2. Crear libros de prueba ===== */
    Libro l1 = { strdup("LIB001"), strdup("El Principito"),
                 strdup("Antoine de Saint-Exupéry"), 3000.0f, 10 };
    Libro l2 = { strdup("LIB002"), strdup("Cien Años de Soledad"),
                 strdup("Gabriel García Márquez"),   5000.0f, 5  };

    /* ===== 3. Crear pedidos de prueba ===== */
    int nPedidos = 3;
    Pedido *pedidos = calloc((size_t)nPedidos, sizeof(Pedido));

    // Pedido 1 – María compra 2 libros l1
    strcpy(pedidos[0].id, "P0001");
    strcpy(pedidos[0].cedula, "111");
    strcpy(pedidos[0].fecha, "2025-09-15");
    pedidos[0].subtotal = l1.precio * 2;
    pedidos[0].impuesto = pedidos[0].subtotal * 0.13f;
    pedidos[0].total    = pedidos[0].subtotal + pedidos[0].impuesto;
    pedidos[0].cantidadLibros = 1;
    pedidos[0].libros = malloc(sizeof(Libro));
    pedidos[0].cantidadPorLibro = malloc(sizeof(int));
    pedidos[0].libros[0] = l1;
    pedidos[0].cantidadPorLibro[0] = 2;

    // Pedido 2 – María compra 1 libro l2
    strcpy(pedidos[1].id, "P0002");
    strcpy(pedidos[1].cedula, "111");
    strcpy(pedidos[1].fecha, "2025-09-16");
    pedidos[1].subtotal = l2.precio * 1;
    pedidos[1].impuesto = pedidos[1].subtotal * 0.13f;
    pedidos[1].total    = pedidos[1].subtotal + pedidos[1].impuesto;
    pedidos[1].cantidadLibros = 1;
    pedidos[1].libros = malloc(sizeof(Libro));
    pedidos[1].cantidadPorLibro = malloc(sizeof(int));
    pedidos[1].libros[0] = l2;
    pedidos[1].cantidadPorLibro[0] = 1;

    // Pedido 3 – Juan compra 1 libro l1
    strcpy(pedidos[2].id, "P0003");
    strcpy(pedidos[2].cedula, "222");
    strcpy(pedidos[2].fecha, "2025-09-17");
    pedidos[2].subtotal = l1.precio;
    pedidos[2].impuesto = pedidos[2].subtotal * 0.13f;
    pedidos[2].total    = pedidos[2].subtotal + pedidos[2].impuesto;
    pedidos[2].cantidadLibros = 1;
    pedidos[2].libros = malloc(sizeof(Libro));
    pedidos[2].cantidadPorLibro = malloc(sizeof(int));
    pedidos[2].libros[0] = l1;
    pedidos[2].cantidadPorLibro[0] = 1;

    /* ===== 4. Registrar pedidos en el modulo Pedido ===== */
    pedidos_set(pedidos, nPedidos);

    /* ===== 5. Mostrar clientes con mas pedidos ===== */
    mostrarClientesConMasPedidos();

    /* ===== 6. Liberar memoria ===== */
    for (int i = 0; i < nPedidos; i++) {
        free(pedidos[i].libros);
        free(pedidos[i].cantidadPorLibro);
    }
    free(pedidos);

    /* Liberar cadenas unicas de los libros originales */
    free(l1.codigo);  free(l1.titulo);  free(l1.autor);
    free(l2.codigo);  free(l2.titulo);  free(l2.autor);

    liberarMemoriaClientes();

    return 0;
}
