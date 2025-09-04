#include "Utilidades.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

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



    fclose(archivo);
}


