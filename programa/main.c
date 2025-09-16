#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Cliente.h"
#include "Config.h"
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
    int nPedidos = 5;
    Pedido *pedidos = calloc((size_t)nPedidos, sizeof(Pedido));

    // Pedido 1 – María compra 2 libros l1
    strcpy(pedidos[0].id, "P0001");
    strcpy(pedidos[0].cedula, "111");
    strcpy(pedidos[0].fecha, "17092025");
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
    strcpy(pedidos[1].fecha, "16092025");
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
    strcpy(pedidos[2].fecha, "15092025");
    pedidos[2].subtotal = l1.precio;
    pedidos[2].impuesto = pedidos[2].subtotal * 0.13f;
    pedidos[2].total    = pedidos[2].subtotal + pedidos[2].impuesto;
    pedidos[2].cantidadLibros = 1;
    pedidos[2].libros = malloc(sizeof(Libro));
    pedidos[2].cantidadPorLibro = malloc(sizeof(int));
    pedidos[2].libros[0] = l1;
    pedidos[2].cantidadPorLibro[0] = 1;

    // Pedido 4 – Juan compra 1 libro l2 en 2024
    strcpy(pedidos[3].id, "P0004");
    strcpy(pedidos[3].cedula, "222");
    strcpy(pedidos[3].fecha, "01122024");
    pedidos[3].subtotal = l2.precio;
    pedidos[3].impuesto = pedidos[3].subtotal * 0.13f;
    pedidos[3].total    = pedidos[3].subtotal + pedidos[3].impuesto;
    pedidos[3].cantidadLibros = 1;
    pedidos[3].libros = malloc(sizeof(Libro));
    pedidos[3].cantidadPorLibro = malloc(sizeof(int));
    pedidos[3].libros[0] = l2;
    pedidos[3].cantidadPorLibro[0] = 1;

    // Pedido 5 – Maria compra 3 libros l1 en 2026
    strcpy(pedidos[4].id, "P0005");
    strcpy(pedidos[4].cedula, "111");
    strcpy(pedidos[4].fecha, "15032026");
    pedidos[4].subtotal = l1.precio * 3;
    pedidos[4].impuesto = pedidos[4].subtotal * 0.13f;
    pedidos[4].total    = pedidos[4].subtotal + pedidos[4].impuesto;
    pedidos[4].cantidadLibros = 1;
    pedidos[4].libros = malloc(sizeof(Libro));
    pedidos[4].cantidadPorLibro = malloc(sizeof(int));
    pedidos[4].libros[0] = l1;
    pedidos[4].cantidadPorLibro[0] = 3;

    /* ===== 4. Registrar pedidos en el modulo Pedido ===== */
    pedidos_set(pedidos, nPedidos);

    /* ===== 5. Mostrar Autores con mas pedidos por años ===== */
    mostrarAutorMasVentasPorAnio();


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
