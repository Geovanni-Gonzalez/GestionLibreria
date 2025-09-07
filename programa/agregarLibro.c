#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Libro.h"

static int generarCodigoUnico(Libro* libros, int totalLibros, char* nuevoCodigo) {
    int codigo = 1000;
    int esUnico = 0;
    while (!esUnico) {
        esUnico = 1;
        sprintf(nuevoCodigo, "LIB%04d", codigo);
        for (int i = 0; i < totalLibros; i++) {
            if (strcmp(libros[i].codigo, nuevoCodigo) == 0) {
                esUnico = 0;
                codigo++;
                break;
            }
        }
    }
    return 1;
}

void agregarLibro(Libro** libros, int* totalLibros, const char* titulo, const char* autor, float precio, int cantidad) {
    if (cantidad <= 0 || precio < 0.0f || !titulo || !autor) {
        printf("Datos inválidos para el libro.\n");
        return;
    }

    // Validar unicidad del título y autor (puede cambiarse por código si se requiere)
    for (int i = 0; i < *totalLibros; i++) {
        if (strcmp((*libros)[i].titulo, titulo) == 0 && strcmp((*libros)[i].autor, autor) == 0) {
            printf("El libro ya existe en el inventario.\n");
            return;
        }
    }

    Libro* temp = realloc(*libros, sizeof(Libro) * (*totalLibros + 1));
    if (!temp) {
        printf("No se pudo asignar memoria para el nuevo libro.\n");
        return;
    }
    *libros = temp;

    Libro* nuevoLibro = &((*libros)[*totalLibros]);
    nuevoLibro->titulo = malloc(strlen(titulo) + 1);
    nuevoLibro->autor = malloc(strlen(autor) + 1);
    if (!nuevoLibro->titulo || !nuevoLibro->autor) {
        printf("No se pudo asignar memoria para los datos del libro.\n");
        return;
    }
    strcpy(nuevoLibro->titulo, titulo);
    strcpy(nuevoLibro->autor, autor);

    nuevoLibro->precio = precio;
    nuevoLibro->cantidad = cantidad;

    nuevoLibro->codigo = malloc(10);
    if (!nuevoLibro->codigo) {
        printf("No se pudo asignar memoria para el código.\n");
        return;
    }
    generarCodigoUnico(*libros, *totalLibros, nuevoLibro->codigo);

    (*totalLibros)++;
    printf("Libro agregado exitosamente: %s - %s\n", nuevoLibro->titulo, nuevoLibro->autor);
}