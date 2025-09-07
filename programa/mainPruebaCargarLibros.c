#include <stdio.h>
#include <stdlib.h>
#include "Libro.h"

// Función auxiliar para imprimir libros
void imprimirLibros(Libro* libros, int totalLibros) {
    for (int i = 0; i < totalLibros; i++) {
        printf("Libro %d:\n", i+1);
        printf("  Codigo: %s\n", libros[i].codigo);
        printf("  Titulo: %s\n", libros[i].titulo);
        printf("  Autor: %s\n", libros[i].autor);
        printf("  Precio: %.2f\n", libros[i].precio);
        printf("  Cantidad: %d\n", libros[i].cantidad);
    }
}

int main() {
    Libro* libros = NULL;
    int totalLibros = 0;

    // Prueba de agregar libros
    agregarLibro(&libros, &totalLibros, "El Quijote", "Cervantes", 15000.0, 5);
    agregarLibro(&libros, &totalLibros, "Cien años de soledad", "Gabriel Garcia Marquez", 18000.0, 3);
    // Intento de agregar libro con título repetido (debería rechazar)
    agregarLibro(&libros, &totalLibros, "El Quijote", "Cervantes", 20000.0, 2);

    imprimirLibros(libros, totalLibros);

    liberarLibros(libros, totalLibros);
    return 0;
}