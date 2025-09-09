/**
 * @file Utilidades.c
 * @brief Funciones utilitarias para el sistema de gestión de librería
 *
 * Este archivo contiene funciones para:
 * - Validación de datos (cédula y teléfono)
 * - Manejo de pedidos (generación de ID, cálculo de precios, stock)
 * - Manejo de clientes (lectura de archivo, carga en arreglo dinámico)
 * - Manejo de libros (inventario)
 */

#include "Utilidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*==================== FUNCIONES AUXILIARES ====================*/

/**
 * @brief Copia un token a destino de manera segura con terminación nula.
 */
static bool copiarToken(char* destino, const char* token, size_t maxLen) {
    if (!destino || !token) return false;
    strncpy(destino, token, maxLen - 1);
    destino[maxLen - 1] = '\0';
    return true;
}

/**
 * @brief Redimensiona un arreglo dinámico genérico.
 */
static bool redimensionarArreglo(void** arreglo, int* capacidad, size_t tamElemento) {
    int nuevaCap = (*capacidad) * 2;
    void* tmp = realloc(*arreglo, nuevaCap * tamElemento);
    if (!tmp) return false;
    *arreglo = tmp;
    *capacidad = nuevaCap;
    return true;
}

/**
 * @brief Abre un archivo y muestra mensaje de error si no se puede abrir.
 */
static FILE* abrirArchivo(const char* ruta, const char* modo) {
    FILE* f = fopen(ruta, modo);
    if (!f) printf("Error al abrir %s\n", ruta);
    return f;
}

/*==================== FUNCIONES DE CLIENTES ====================*/

/**
 * @brief Extrae campos de cliente desde una línea.
 */
static bool extraerCamposCliente(char* linea, char* cedula, char* nombre, char* telefono) {
    char* token = strtok(linea, ";");
    if (!token) return false;
    copiarToken(cedula, token, MAX_CEDULA);

    token = strtok(NULL, ";");
    if (!token) return false;
    copiarToken(nombre, token, MAX_NOMBRE);

    token = strtok(NULL, ";");
    if (!token) return false;
    copiarToken(telefono, token, MAX_TELEFONO);

    return true;
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

/**
 * @brief Carga clientes desde archivo en un arreglo dinámico.
 */
void cargarClientesDesdeArchivo(Cliente** arregloClientes, int* cantActual, int* capArreglo) {
    FILE* archivo = abrirArchivo("data/clientes.txt", "r");
    if (!archivo) return;

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        char cedula[MAX_CEDULA], nombre[MAX_NOMBRE], telefono[MAX_TELEFONO];
        if (!extraerCamposCliente(linea, cedula, nombre, telefono)) continue;
        if (!validarCedulaUnica(cedula, *arregloClientes, *cantActual)) continue;
        if (!validarTelefono(telefono)) continue;

        if (*cantActual == *capArreglo && !redimensionarArreglo((void**)arregloClientes, capArreglo, sizeof(Cliente))) {
            printf("Error al asignar memoria.\n"); break;
        }

        Cliente c;
        copiarToken(c.cedula, cedula, MAX_CEDULA);
        copiarToken(c.nombre, nombre, MAX_NOMBRE);
        copiarToken(c.telefono, telefono, MAX_TELEFONO);

        (*arregloClientes)[*cantActual] = c;
        (*cantActual)++;
    }

    fclose(archivo);
}

/*==================== FUNCIONES DE PEDIDOS ====================*/

/**
 * @brief Extrae libros y cantidades de un pedido.
 */
static bool extraerLibrosPedido(char* linea, Libro** libros, int** cantidades, int cantidadLibros) {
    *libros = malloc(cantidadLibros * sizeof(Libro));
    *cantidades = malloc(cantidadLibros * sizeof(int));
    if (!*libros || !*cantidades) return false;

    int i;
    for (i = 0; i < cantidadLibros; i++) {
        char* token = strtok(NULL, ";");
        if (!token) goto error;
        char* subToken = strtok(token, ":");
        if (!subToken) goto error;
        (*libros)[i].codigo = strdup(subToken);

        subToken = strtok(NULL, ":");
        if (!subToken) goto error;
        (*cantidades)[i] = atoi(subToken);
    }

    return true;

error:
    for (int j = 0; j < i; j++) free((*libros)[j].codigo);
    free(*libros);
    free(*cantidades);
    return false;
}

static bool extraerCamposPedido(char* linea, Pedido* pedido) {
    char* token = strtok(linea, ";");
    if (!token) return false;
    copiarToken(pedido->id, token, MAX_ID);

    token = strtok(NULL, ";");
    if (!token) return false;
    copiarToken(pedido->cedula, token, MAX_CEDULA);

    token = strtok(NULL, ";");
    if (!token) return false;
    copiarToken(pedido->fecha, token, MAX_FECHA);

    token = strtok(NULL, ";");
    if (!token) return false;
    pedido->subtotal = atof(token);

    token = strtok(NULL, ";");
    if (!token) return false;
    pedido->impuesto = atof(token);

    token = strtok(NULL, ";");
    if (!token) return false;
    pedido->total = atof(token);

    token = strtok(NULL, ";");
    if (!token) return false;
    pedido->generado = (strcmp(token, "1") == 0 || strcmp(token, "true") == 0);

    token = strtok(NULL, ";");
    if (!token) return false;
    pedido->cantidadLibros = atoi(token);

    return extraerLibrosPedido(linea, &pedido->libros, &pedido->cantidadPorLibro, pedido->cantidadLibros);
}


void cargarPedidosDesdeArchivo(Pedido** arregloPedidos, int* cantActual, int* capArreglo) {
    FILE* archivo = abrirArchivo("data/pedidos.txt", "r");
    if (!archivo) return;

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        Pedido p;
        if (!extraerCamposPedido(linea, &p)) continue;

        if (*cantActual == *capArreglo && !redimensionarArreglo((void**)arregloPedidos, capArreglo, sizeof(Pedido))) {
            printf("Error al asignar memoria.\n"); break;
        }

        (*arregloPedidos)[*cantActual] = p;
        (*cantActual)++;
    }

    fclose(archivo);
}

/*==================== FUNCIONES DE LIBROS ====================*/

char* leerArchivo(const char* nombre) {
    FILE* f = abrirArchivo(nombre, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char* data = malloc(len + 1);
    if (!data) { fclose(f); return NULL; }
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);
    return data;
}

char* generarIDPedido(int* totalPedidos) {
    char* id = malloc(5);
    sprintf(id, "P%03d", *totalPedidos + 1);
    return id;
}

void calcularPreciosPedido(Pedido* pedido) {
    pedido->subtotal = 0.0f;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        pedido->subtotal += pedido->libros[i].precio * pedido->cantidadPorLibro[i];
    }
    pedido->impuesto = pedido->subtotal * IMPUESTO;
    pedido->total = pedido->subtotal + pedido->impuesto;
}

void descontarStockLibro(Libro* libro, int* cantidadPorLibro, int cantidadLibros) {
    const char* ruta = "data/libros.txt";
    FILE* f = abrirArchivo(ruta, "r");
    if (!f) return;

    Libro libros[100];
    int total = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), f) && total < 100) {
        char* token = strtok(linea, ";");
        if (!token) continue;
        libros[total].codigo = strdup(token);
        token = strtok(NULL, ","); libros[total].titulo = token?strdup(token):strdup("");
        token = strtok(NULL, ","); libros[total].autor = token?strdup(token):strdup("");
        token = strtok(NULL, ","); libros[total].precio = token?atof(token):0;
        token = strtok(NULL, ","); libros[total].cantidad = token?atoi(token):0;
        total++;
    }
    fclose(f);

    for (int i = 0; i < cantidadLibros; i++) {
        for (int j = 0; j < total; j++) {
            if (strcmp(libro[i].codigo, libros[j].codigo) == 0) {
                libros[j].cantidad -= cantidadPorLibro[i];
                break;
            }
        }
    }

    f = abrirArchivo(ruta, "w");
    if (!f) return;
    for (int i = 0; i < total; i++) {
        fprintf(f, "%s;%s,%s,%f,%d\n",
                libros[i].codigo,
                libros[i].titulo,
                libros[i].autor,
                libros[i].precio,
                libros[i].cantidad);
        free(libros[i].codigo);
        free(libros[i].titulo);
        free(libros[i].autor);
    }
    fclose(f);
}

int generarCodigoUnicoLibro(Libro* libros, int totalLibros, char* nuevoCodigo) {
    int maxCodigo = 0;
    for (int i = 0; i < totalLibros; i++) {
        int codigoNum = atoi(libros[i].codigo + 1);
        if (codigoNum > maxCodigo) maxCodigo = codigoNum;
    }
    sprintf(nuevoCodigo, "L%03d", maxCodigo + 1);
    return 1;
}

int stringAInt(const char* str) {
    int resultado = 0;
    int signo = 1;
    int i = 0;

    if (str[0] == '-') {
        signo = -1;
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        resultado = resultado * 10 + (str[i] - '0');
        i++;
    }

    return signo * resultado;
}

// Funcion para copiar un string caracter por caracter
char* copiarString(const char* src) {
    int len = 0;
    while (src[len] != '\0') len++;

    char* dst = malloc(len + 1);
    if (!dst) return NULL;

    for (int i = 0; i <= len; i++) {  // <= para copiar '\0'
        dst[i] = src[i];
    }
    return dst;
}
// Funcion para pasar un "String" a float
float stringAFloat(const char* str) {
    float resultado = 0.0f;
    int signo = 1;
    int i = 0;

    // Manejar signo
    if (str[0] == '-') {
        signo = -1;
        i++;
    }

    // Parte entera
    while (str[i] >= '0' && str[i] <= '9') {
        resultado = resultado * 10 + (str[i] - '0');
        i++;
    }

    // Parte decimal
    if (str[i] == '.') {
        i++;
        float decimal = 0.0f;
        float divisor = 10.0f;
        while (str[i] >= '0' && str[i] <= '9') {
            decimal += (str[i] - '0') / divisor;
            divisor *= 10;
            i++;
        }
        resultado += decimal;
    }

    return signo * resultado;
}
