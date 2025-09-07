#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Libro.h"

void cargarInventario(Libro** libros, int* totalLibros, const char* rutaArchivo) {
    FILE* archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de inventario: %s\n", rutaArchivo);
        return;
    }

    char linea[128];
    int lineaActual = 1;
    while (fgets(linea, sizeof(linea), archivo)) {
        char codigo[32];
        int cantidad;
        if (sscanf(linea, "%31[^;];%d", codigo, &cantidad) != 2) {
            printf("Línea %d no procesada: formato incorrecto\n", lineaActual);
            lineaActual++;
            continue;
        }

        int encontrado = 0;
        for (int i = 0; i < *totalLibros; i++) {
            if (strcmp((*libros)[i].codigo, codigo) == 0) {
                int nuevoStock = (*libros)[i].cantidad + cantidad;
                if (nuevoStock < 0) {
                    printf("Línea %d no procesada: stock negativo para %s\n", lineaActual, codigo);
                } else {
                    (*libros)[i].cantidad = nuevoStock;
                }
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            printf("Línea %d no procesada: código no encontrado (%s)\n", lineaActual, codigo);
        }
        lineaActual++;
    }
    fclose(archivo);
}