#include <stdlib.h>
#include "Libro.h"

void liberarLibros(Libro* libros, int totalLibros) {
    if (!libros) return;
    for (int i = 0; i < totalLibros; i++) {
        free(libros[i].codigo);
        free(libros[i].titulo);
        free(libros[i].autor);
    }
    free(libros);
}