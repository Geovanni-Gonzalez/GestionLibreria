#include "Pedido.h"
#include "Libro.h"
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>   

int main(void) {
    // 1. Prepara inventario
    int totalLibros = 2;
    Libro* inventario = malloc(sizeof(Libro) * totalLibros);
    inventario[0].codigo = strdup("ABC123");
    inventario[0].titulo = strdup("Libro de Prueba");
    inventario[0].autor  = strdup("Autor");
    inventario[0].precio = 100.0f;
    inventario[0].cantidad = 5;
    inventario[1].codigo = strdup("ABC123456");
    inventario[1].titulo = strdup("Libro de Pruebassssss");
    inventario[1].autor  = strdup("Autoresssss");
    inventario[1].precio = 100000.0f;
    inventario[1].cantidad = 8;

    // 2. Prepara un pedido que contenga el libro
    Pedido pedidos[1] = {0};
    pedidos[0].cantidadLibros = 1;
    pedidos[0].libros = malloc(sizeof(Libro));
    pedidos[0].libros[0].codigo = strdup("ABC123");

    // 3. Intenta eliminar
    const char* codigo = "ABC123456";
    if (libroAsociadoAPedido(codigo, pedidos, 1)) {
        printf("El libro %s está asociado a un pedido, no se elimina.\n", codigo);
    } else {
        eliminarLibro(&inventario, &totalLibros, codigo);
    }

    // 4. Comprueba que sigue en inventario
    printf("Libros restantes: %d\n", totalLibros);
    return 0;
}
