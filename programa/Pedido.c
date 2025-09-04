#include "Pedido.h"
#include <stdlib.h>

/***
 * @brief Remueve un libro del la lista mientras se crea un pedido.
 * @param codigoLibro Código del libro a remover.
 * @param pedido Puntero al pedido del cual se desea remover el libro.
 * @return void
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido) {
    if (pedido->cantidadLibros <= 0) {
        printf("No hay libros para remover.\n");
        return; // No hay libros para remover
    }

    int indice = -1;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (strcmp(pedido->codigosLibros[i], codigoLibro) == 0) {
            indice = i;
            break;
        }
    }
    if (indice == -1) {
        printf("Código de libro no encontrado en el pedido.\n");
        return; 
    }

    free(pedido->codigosLibros[indice]);

    // Ajusta el arreglo de códigos de libros
    for (int i = indice; i < pedido->cantidadLibros - 1; i++) {
        pedido->codigosLibros[i] = pedido->codigosLibros[i + 1];
    }

    // Reduce el tamaño del arreglo
    pedido->cantidadLibros--;
    pedido->codigosLibros = realloc(pedido->codigosLibros, pedido->cantidadLibros * sizeof(char*));
}