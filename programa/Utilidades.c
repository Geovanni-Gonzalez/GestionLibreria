/**
 * @file Utilidades.c
 * @brief Funciones utilitarias para el sistema de gestión de librería
 *
 * Este archivo contiene funciones para:
 * - Validación de datos (cédula y teléfono)
 * - Manejo de pedidos (generación de ID, cálculo de precios, stock)
 * - Manejo de clientes (lectura de archivo, carga en arreglo dinámico)
 *
 */

#include "Utilidades.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



/**
 * @brief Copia cadenas de manera segura, asegurando terminación nula.
 */
static bool copiarCadenaSeguro(char* destino, const char* origen, size_t maxLen) {
    if (!origen || !destino) return false;
    strncpy(destino, origen, maxLen - 1);
    destino[maxLen - 1] = '\0';
    return true;
}

/**
 * @brief Extrae los campos de un cliente desde una línea del archivo.
 */
static bool extraerCamposCliente(char* linea, char* cedula, char* nombre, char* telefono) {
    char* token = strtok(linea, ";");
    if (!token) return false;
    copiarCadenaSeguro(cedula, token, MAX_CEDULA);

    token = strtok(NULL, ";");
    if (!token) return false;
    copiarCadenaSeguro(nombre, token, MAX_NOMBRE);

    token = strtok(NULL, ";");
    if (!token) return false;
    copiarCadenaSeguro(telefono, token, MAX_TELEFONO);

    return true;
}

/**
 * @brief Redimensiona un arreglo dinámico de clientes.
 */
static bool redimensionarArregloClientes(Cliente** arregloClientes, int* capacidad) {
    int nuevaCapacidad = (*capacidad) * 2;
    Cliente* tmp = (Cliente*) realloc(*arregloClientes, nuevaCapacidad * sizeof(Cliente));
    if (!tmp) return false;
    *arregloClientes = tmp;
    *capacidad = nuevaCapacidad;
    return true;
}

/**
 * @brief Lee un archivo completo y retorna su contenido como cadena.
 */
char* leerArchivo(const char* nombre) {
    FILE* f = fopen(nombre, "rb");
    if (!f) {
        printf("No se pudo abrir %s\n", nombre);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char* data = (char*)malloc(len + 1); // +1 para el carácter nulo
    if (!data) {
        fclose(f);
        return NULL;
    }
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);
    return data;
}


bool validarCedulaUnica(const char* cedula, Cliente* arregloClientes, int cantidadDeClientes) {
    for (int i = 0; i < cantidadDeClientes; i++) {
        if (strcmp(arregloClientes[i].cedula, cedula) == 0) return false;
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
    pedido->subtotal = 0.0f;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        pedido->subtotal += pedido->libros[i].precio * pedido->cantidadPorLibro[i];
    }
    pedido->impuesto = pedido->subtotal * IMPUESTO;
    pedido->total = pedido->subtotal + pedido->impuesto;
}

/**
 * @brief Actualiza el stock de los libros en archivo según las cantidades en el pedido.
 */
void descontarStockLibro(Libro *libro, int *cantidadPorLibro, int cantidadLibros) {
    const char* rutaArchivo = "data/libros.txt";
    FILE* archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo de libros.\n");
        return;
    }

    // Leer todos los libros en memoria
    Libro libros[100];
    int totalLibros = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo) && totalLibros < 100) {
        char* token = strtok(linea, ";");
        if (!token) continue;
        libros[totalLibros].codigo = strdup(token);

        token = strtok(NULL, ",");
        libros[totalLibros].titulo = token ? strdup(token) : strdup("");
        token = strtok(NULL, ",");
        libros[totalLibros].autor = token ? strdup(token) : strdup("");
        token = strtok(NULL, ",");
        libros[totalLibros].precio = token ? atof(token) : 0;
        token = strtok(NULL, ",");
        libros[totalLibros].cantidad = token ? atoi(token) : 0;

        totalLibros++;
    }
    fclose(archivo);

    // Actualizar stock según pedido
    for (int i = 0; i < cantidadLibros; i++) {
        for (int j = 0; j < totalLibros; j++) {
            if (strcmp(libro[i].codigo, libros[j].codigo) == 0) {
                libros[j].cantidad -= cantidadPorLibro[i];
                break;
            }
        }
    }

    // Escribir de nuevo el archivo
    archivo = fopen(rutaArchivo, "w");
    if (!archivo) {
        printf("Error al abrir el archivo de libros para escritura.\n");
        return;
    }
    for (int i = 0; i < totalLibros; i++) {
        fprintf(archivo, "%s;%s,%s,%f,%d\n",
                libros[i].codigo,
                libros[i].titulo,
                libros[i].autor,
                libros[i].precio,
                libros[i].cantidad);
        free(libros[i].codigo);
        free(libros[i].titulo);
        free(libros[i].autor);
    }
    fclose(archivo);
}


void cargarClientesDesdeArchivo(Cliente** arregloClientes, int* cantidadClientesActual, int* capacidadDeClientesArreglo) {
    FILE* archivo = fopen("data/clientes.txt", "r");
    if (!archivo) { 
        printf("No se pudo abrir el archivo de clientes.\n");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        char cedula[MAX_CEDULA], nombre[MAX_NOMBRE], telefono[MAX_TELEFONO];
        if (!extraerCamposCliente(linea, cedula, nombre, telefono)) continue;

        if (!validarCedulaUnica(cedula, *arregloClientes, *cantidadClientesActual)) continue;
        if (!validarTelefono(telefono)) continue;

        // Redimensionar si es necesario
        if (*cantidadClientesActual == *capacidadDeClientesArreglo) {
            if (!redimensionarArregloClientes(arregloClientes, capacidadDeClientesArreglo)) {
                printf("Error al asignar memoria.\n");
                break;
            }
        }

        // Agregar cliente
        Cliente nuevoCliente;
        copiarCadenaSeguro(nuevoCliente.cedula, cedula, MAX_CEDULA);
        copiarCadenaSeguro(nuevoCliente.nombre, nombre, MAX_NOMBRE);
        copiarCadenaSeguro(nuevoCliente.telefono, telefono, MAX_TELEFONO);

        (*arregloClientes)[*cantidadClientesActual] = nuevoCliente;
        (*cantidadClientesActual)++;
    }

    fclose(archivo);
}
