#include <stdio.h>
#include "../include/Libro.h"

int main() {
    int cantidad = 0;
    Libro* libros = cargarLibros("../data/libros.txt", &cantidad);

    if (!libros) {
        printf("No se cargaron libros.\n");
        return 1;
    }

    printf("Se cargaron %d libros:\n\n", cantidad);

    for (int i = 0; i < cantidad; i++) {
        printf("Libro %d:\n", i + 1);
        printf("  Codigo  : %s\n", libros[i].codigo);
        printf("  Titulo  : %s\n", libros[i].titulo);
        printf("  Autor   : %s\n", libros[i].autor);
        printf("  Precio  : %.2f\n", libros[i].precio);
        printf("  Cantidad: %d\n\n", libros[i].cantidad);
    }

    // Liberar memoria dinamica 
    liberarLibros(libros, cantidad);

    return 0;
}
