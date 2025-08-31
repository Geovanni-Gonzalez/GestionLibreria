#include "Cliente.h"
#include "Utilidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Cliente* arregloClientes = NULL;     // Arreglo dinámico
static int cantidadClientesActual = 0;      // Cantidad actual de clientes
static int capacidadDeClientesArreglo = 2;  // Tamaño inicial del arreglo


bool registrarCliente(const char* cedula, const char* nombre, const char* telefono) {
    if (!validarCedulaUnica(cedula, arregloClientes, cantidadClientesActual)) {
        printf("Error: la cédula ya existe.\n");
        return false;
    }
    if (!validarTelefono(telefono)) {
        printf("Error: teléfono inválido.\n");
        return false;
    }

    // Redimensionar si es necesario
    if (cantidadClientesActual == capacidadDeClientesArreglo) {
        capacidadDeClientesArreglo *= 2;
        arregloClientes = (Cliente*) realloc(arregloClientes, capacidadDeClientesArreglo * sizeof(Cliente));
        if (arregloClientes == NULL) {
            printf("Error al reasignar memoria.\n");
            exit(1);
        }
    }

    strcpy(arregloClientes[cantidadClientesActual].cedula, cedula);
    strcpy(arregloClientes[cantidadClientesActual].nombre, nombre);
    strcpy(arregloClientes[cantidadClientesActual].telefono, telefono);
    cantidadClientesActual++;

    printf("Cliente registrado correctamente.\n");
    return true;
}

// Mostrar clientes registrados
void mostrarClientes() {
    printf("\n--- Lista de Clientes ---\n");
    for (int i = 0; i < cantidadClientesActual; i++) {
        printf("%d) Cédula: %s, Nombre: %s, Tel: %s\n", 
                i+1, arregloClientes[i].cedula, arregloClientes[i].nombre, arregloClientes[i].telefono);
    }
}

// Liberar memoria al final
void liberarMemoriaClientes() {
    free(arregloClientes);
    arregloClientes = NULL;
    cantidadClientesActual = 0;
    capacidadDeClientesArreglo = 0;
}