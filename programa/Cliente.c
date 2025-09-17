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

bool eliminarCliente(const char* cedula) {
    if (cedula == NULL || cedula[0] == '\0') {
        printf("Cédula inválida.\n");
        return false;
    }

    // Buscar índice del cliente en el arreglo
    int indiceCliente = -1;
    for (int indiceBusqueda = 0; indiceBusqueda < cantidadClientesActual; indiceBusqueda++) {
        if (strcmp(arregloClientes[indiceBusqueda].cedula, cedula) == 0) {
            indiceCliente = indiceBusqueda;
            break;
        }
    }

    if (indiceCliente == -1) {
        printf("Cliente no encontrado.\n");
        return false;
    }

    // Validar que no tenga pedidos asociados
    extern Pedido* arregloPedidos;        // Definidos en Pedido.c
    extern int cantidadPedidosActual;     // Definidos en Pedido.c

    int cantidadPedidosAsociados = 0;
    Pedido* pedidosCliente = obtenerPedidosPorCliente((char*)cedula, arregloPedidos, cantidadPedidosActual, &cantidadPedidosAsociados);
    if (cantidadPedidosAsociados > 0) {
        printf("No se puede eliminar el cliente: tiene pedidos asociados (%d).\n", cantidadPedidosAsociados);
        if (pedidosCliente != NULL) {
            free(pedidosCliente);
        }
        return false;
    }
    if (pedidosCliente != NULL) {
        free(pedidosCliente);
    }

    // Eliminar del arreglo dinámico (desplazar elementos)
    for (int indiceDesplazamiento = indiceCliente; indiceDesplazamiento < cantidadClientesActual - 1; indiceDesplazamiento++) {
        arregloClientes[indiceDesplazamiento] = arregloClientes[indiceDesplazamiento + 1];
    }
    cantidadClientesActual--;

    /*
     * - Tras eliminar un cliente, si el uso del arreglo es la mitad o menos
     *   de su capacidad actual, intentamos reducir la capacidad a la mitad
     *   para evitar memoria que no esta haciendo nada.
     * - Nunca reducimos por debajo de 2 elementos para mantener un mínimo razonable.
     * - Usamos realloc y solo actualizamos el puntero y la capacidad si la
     *   reasignación se realiza con éxito, evitando perder la referencia en caso que de error.
     */
    if (cantidadClientesActual > 0 && cantidadClientesActual <= (capacidadDeClientesArreglo / 2)) {
        int nuevaCapacidadClientes = capacidadDeClientesArreglo / 2;
        if (nuevaCapacidadClientes < 2) {
            nuevaCapacidadClientes = 2;
        }
        Cliente* arregloClientesRedimensionado = (Cliente*)realloc(arregloClientes, nuevaCapacidadClientes * sizeof(Cliente));
        if (arregloClientesRedimensionado != NULL) {
            arregloClientes = arregloClientesRedimensionado;
            capacidadDeClientesArreglo = nuevaCapacidadClientes;
        }
    }

    // Actualizar archivo: reescribir data/clientes.txt sin el cliente eliminado
    const char* rutaArchivoClientes = "data/clientes.txt";
    const char* rutaArchivoTemporal = "data/clientes.tmp";

    FILE* archivoOriginal = fopen(rutaArchivoClientes, "r");
    if (archivoOriginal == NULL) {
        printf("Advertencia: no se pudo abrir el archivo de clientes para lectura. Solo se eliminó en memoria.\n");
        printf("Cliente eliminado correctamente en memoria.\n");
        return true;
    }

    FILE* archivoTemporal = fopen(rutaArchivoTemporal, "w");
    if (archivoTemporal == NULL) {
        fclose(archivoOriginal);
        printf("Advertencia: no se pudo crear el archivo temporal. Solo se eliminó en memoria.\n");
        printf("Cliente eliminado correctamente en memoria.\n");
        return true;
    }

    char lineaLeida[256];
    while (fgets(lineaLeida, sizeof(lineaLeida), archivoOriginal)) {
        // Copiar para no modificar la línea original al separar
        char copiaLinea[256];
        strncpy(copiaLinea, lineaLeida, sizeof(copiaLinea) - 1);
        copiaLinea[sizeof(copiaLinea) - 1] = '\0';

        // Separar por ';' para extraer la cédula
        char* campos[3] = {0};
        int cantidadCampos = 0;
        char* punteroRecorrido = copiaLinea;
        char* inicioCampo = copiaLinea;

        while (*punteroRecorrido != '\0' && cantidadCampos < 3) {
            if (*punteroRecorrido == ';') {
                *punteroRecorrido = '\0';
                campos[cantidadCampos++] = inicioCampo;
                inicioCampo = punteroRecorrido + 1;
            }
            punteroRecorrido++;
        }
        campos[cantidadCampos++] = inicioCampo;

        const char* cedulaEnArchivo = (cantidadCampos >= 1 && campos[0]) ? campos[0] : NULL;

        // Normalizar fin de línea en cedulaEnArchivo
        if (cedulaEnArchivo != NULL) {
            size_t longitudCedulaArchivo = strlen(cedulaEnArchivo);
            if (longitudCedulaArchivo > 0 && (cedulaEnArchivo[longitudCedulaArchivo - 1] == '\n' || cedulaEnArchivo[longitudCedulaArchivo - 1] == '\r')) {
                // No se requiere modificar porque comparamos contra 'cedula' sin EOL;
                // la línea original 'lineaLeida' mantiene los EOL para reescribir tal cual.
            }
        }

        // Si la cédula coincide, no escribir la línea (se elimina)
        if (cedulaEnArchivo != NULL && strcmp(cedulaEnArchivo, cedula) == 0) {
            continue;
        }

        fputs(lineaLeida, archivoTemporal);
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    // Reemplazar archivo original por el temporal
    if (remove(rutaArchivoClientes) != 0) {
        printf("Advertencia: no se pudo eliminar el archivo original de clientes.\n");
        // Intentar limpiar el temporal para evitar basura
        remove(rutaArchivoTemporal);
        printf("Cliente eliminado en memoria, pero no en archivo.\n");
        return true;
    }
    if (rename(rutaArchivoTemporal, rutaArchivoClientes) != 0) {
        printf("Advertencia: no se pudo renombrar el archivo temporal de clientes.\n");
        printf("Cliente eliminado en memoria, pero no en archivo.\n");
        return true;
    }

    printf("Cliente eliminado correctamente.\n");
    return true;
}


int clientes_count(void) {
    return cantidadClientesActual;
}

Cliente* clientes_data(void) {
    return arregloClientes;
}
