#include "Utilidades.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

bool validarCedulaUnica(const char* cedula, Cliente* arregloClientes, int cantidadDeClientes) {
    for (int i = 0; i < cantidadDeClientes; i++) {
        if (strcmp(arregloClientes[i].cedula, cedula) == 0) {
            return false; // ya existe
        }
    }
    return true;
}

bool validarTelefono(const char* telefono) {
    int len = strlen(telefono);
    if (len < 8) return false;
    for (int i = 0; i < len; i++) {
        if (!isdigit(telefono[i])) return false;
    }
    return true;
}


char* generarIDPedido(int *totalPedidos) {
    char* id = (char*)malloc(5 * sizeof(char));
    sprintf(id, "P%03d", *totalPedidos + 1);
    return id;
}

void calcularPreciosPedido(Pedido *pedido) {
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        pedido->subtotal += pedido->libros[i].precio;
    }
    pedido->impuesto = pedido->subtotal * IMPUESTO;
    pedido->total = pedido->subtotal + pedido->impuesto;
}

// Función para descontar el stock de un libro
void descontarStockLibro(Libro *libro, int *cantidadPorLibro, int cantidadLibros) {
    char *rutaArchivo = "data/libros.txt";
    FILE *archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo de libros.\n");
        return;
    }
    Libro libros[100]; // Suponiendo un máximo de 100 libros
    int totalLibros = 0;
    char linea[256];

    // Leer los libros del archivo
    while (fgets(linea, sizeof(linea), archivo)) {
        char *token = strtok(linea, ";");
        if (token) {
            libros[totalLibros].codigo = strdup(token);
            token = strtok(NULL, ",");
            if (token) {
                libros[totalLibros].titulo = strdup(token);
                token = strtok(NULL, ",");
                if (token) {
                    libros[totalLibros].autor = strdup(token);
                    token = strtok(NULL, ",");
                    if (token) {
                        libros[totalLibros].precio = atof(token);
                        token = strtok(NULL, ",");
                        if (token) {
                            libros[totalLibros].cantidad = atoi(token);
                            totalLibros++;
                        }
                    }
                }
            }
        }
    }

    // Actualizar el stock de los libros
    for (int i = 0; i < cantidadLibros; i++) {
        for (int j = 0; j < totalLibros; j++) {
            if (strcmp(libros[j].codigo, libro[i].codigo) == 0) {
                libros[j].cantidad -= cantidadPorLibro[i];
                break;
            }
        }
    }

    // Escribir los libros actualizados de nuevo en el archivo
    archivo = fopen(rutaArchivo, "w");
    if (!archivo) {
        printf("Error al abrir el archivo de libros.\n");
        return;
    }
    for (int i = 0; i < totalLibros; i++) {
        fprintf(archivo, "%s;%s,%s,%f,%d\n", libros[i].codigo, libros[i].titulo, libros[i].autor, libros[i].precio, libros[i].cantidad);
    }

    fclose(archivo);
}


