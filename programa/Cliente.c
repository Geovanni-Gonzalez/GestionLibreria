#include "Cliente.h"
#include "Utilidades.h"
#include "Pedido.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Cliente* arregloClientes = NULL;     // Arreglo dinámico
static int cantidadClientesActual = 0;      // Cantidad actual de clientes
static int capacidadDeClientesArreglo = 2;  // Tamaño inicial del arreglo


void inicializarArregloClientes() {
    arregloClientes = (Cliente*) malloc(capacidadDeClientesArreglo * sizeof(Cliente));
    if (arregloClientes == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    cargarClientesDesdeArchivo(&arregloClientes, &cantidadClientesActual, &capacidadDeClientesArreglo);
}

bool registrarCliente(const char* cedula, const char* nombre, const char* telefono) {
    if (!validarCedulaUnica(cedula, arregloClientes, cantidadClientesActual)) {
        printf("Error: la cédula ya existe.\n");
        return false;
    }
    if (!validarTelefono(telefono)) {
        printf("Error: teléfono inválido.\n");
        return false;
    }

    if (cantidadClientesActual == capacidadDeClientesArreglo) {
        capacidadDeClientesArreglo *= 2;
        arregloClientes = (Cliente*) realloc(arregloClientes, capacidadDeClientesArreglo * sizeof(Cliente));
        if (arregloClientes == NULL) {
            printf("Error al reasignar memoria.\n");
            exit(1);
        }
    }

    Cliente nuevoCliente;
    strncpy(nuevoCliente.cedula, cedula, MAX_CEDULA);
    strncpy(nuevoCliente.nombre, nombre, MAX_NOMBRE);
    strncpy(nuevoCliente.telefono, telefono, MAX_TELEFONO);


    arregloClientes[cantidadClientesActual] = nuevoCliente;
    cantidadClientesActual++;
    
    agregarClienteArchivo(cedula, nombre, telefono);

    printf("Cliente registrado correctamente.\n");
    return true;
}

void mostrarClientes() {
    printf("\n--- Lista de Clientes ---\n");
    if (cantidadClientesActual == 0) {
        printf("No hay clientes registrados.\n");
        return;
    }
    for (int i = 0; i < cantidadClientesActual; i++) {
        printf("%d) Cédula: %s, Nombre: %s, Tel: %s\n", 
                i+1, arregloClientes[i].cedula, arregloClientes[i].nombre, arregloClientes[i].telefono);
    }
}

Cliente *obtenerClientePorCedula(const char *cedula) {
    for (int i = 0; i < cantidadClientesActual; i++) {
        if (strcmp(arregloClientes[i].cedula, cedula) == 0) {
            return &arregloClientes[i];
        }
    }
    return NULL; 
}


void agregarClienteArchivo(const char* cedula, const char* nombre, const char* telefono) {
    FILE *archivo = fopen("data/clientes.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de clientes.\n");
        return;
    }
    fprintf(archivo, "%s;%s;%s\n", cedula, nombre, telefono);
    fclose(archivo);
}

void imprimirInformacionCliente(const Cliente* cliente) {
    if (cliente == NULL) {
        printf("Cliente no encontrado.\n");
        return;
    }
    printf("\n--- Información del Cliente ---\n");
    printf("Cédula: %s\n", cliente->cedula);
    printf("Nombre: %s\n", cliente->nombre);
    printf("Teléfono: %s\n", cliente->telefono);
}

void liberarMemoriaClientes() {
    free(arregloClientes);
    arregloClientes = NULL;
    cantidadClientesActual = 0;
    capacidadDeClientesArreglo = 0;
}

void consultaDeCliente(char* cedula) {
    Cliente* cliente = obtenerClientePorCedula(cedula);
    imprimirInformacionCliente(cliente);

    //Imprimir pedidos del cliente
    int cantidadPedidosCliente = 0;
    Pedido* pedidosCliente = obtenerPedidosPorCliente(cedula, NULL, 0, &cantidadPedidosCliente);
    if (pedidosCliente == NULL || cantidadPedidosCliente == 0) {
        printf("El cliente no tiene pedidos registrados.\n");
    } else {
        printf("\n--- Pedidos del Cliente ---\n");
        for (int i = 0; i < cantidadPedidosCliente; i++) {
            printf("%d) ID: %s, Fecha: %s, Subtotal: %.2f, Impuesto: %.2f, Total: %.2f, Cantidad de Libros: %d\n", 
                    i+1, pedidosCliente[i].id, pedidosCliente[i].fecha, pedidosCliente[i].subtotal, pedidosCliente[i].impuesto, pedidosCliente[i].total, pedidosCliente[i].cantidadLibros);
        }
        free(pedidosCliente); 
    }
}