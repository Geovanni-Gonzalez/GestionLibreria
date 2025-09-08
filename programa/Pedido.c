#include "Pedido.h"
#include "Utilidades.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9], Pedido* arregloPedidos, int* cantidadPedidosActual) {

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

   //Mostrar detalle del pedido

}
