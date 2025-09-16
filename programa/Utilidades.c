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

//Funcion estandar para obtener solo la cadena de caracteres 
void trim(char *s) {
    if (!s) return;
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1))) end--;
    size_t len = (size_t)(end - start);
    memmove(s, start, len);
    s[len] = '\0';
}

//Funcion para leer punteros sin espacios (funcion trim en su version puntero)
char* leerLineaAlloc(const char* prompt, size_t maxLen) {
    char *buf = (char*)malloc(maxLen);
    if (!buf) return NULL;
    printf("%s", prompt);
    if (!fgets(buf, maxLen, stdin)) { free(buf); return NULL; }
    trim(buf);
    return buf;
}

static void trim_inplace(char* s) {
    if (!s) return;
    char *p = s, *q = s + strlen(s);
    while (*p && isspace((unsigned char)*p)) p++;
    while (q > p && isspace((unsigned char)q[-1])) q--;
    size_t n = (size_t)(q - p);
    memmove(s, p, n);
    s[n] = '\0';
}

static char* dupstr_local(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char* d = (char*)malloc(n + 1);
    if (d) memcpy(d, s, n + 1);
    return d;
}

// Se asegura de que no arrastre datos ni saltos de linea extra 
void leerLineaSeguro(char *dst, size_t dstSize) {
    if (!fgets(dst, dstSize, stdin)) {
        clearerr(stdin);
        dst[0] = '\0';
        return;
    }
    // Quita \r\n si vinieron
    size_t n = strcspn(dst, "\r\n");
    if (dst[n] != '\0') {
        dst[n] = '\0';
    } else {
        // No habia \n en el buffer => la linea fue mas larga que dstSize-1.
        // Consumimos el resto hasta el \n para no contaminar la siguiente lectura.
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { /* descartar */ }
    }
}

void limpiar_stdin(void) {
    int c;
    // consume todo hasta el próximo '\n' o carcatrer EOF
    while ((c = getchar()) != '\n' && c != EOF) { /* nada */ }
}

bool validarCedula9(const char* s) {
    if (!s) return false;
    for (int i = 0; i < 9; i++) if (!isdigit((unsigned char)s[i])) return false;
    return s[9] == '\0'; // exactamente 9
}

bool validarFechaYYYYMMDD(const char* s) {
    if (!s) return false;
    for (int i = 0; i < 8; i++) if (!isdigit((unsigned char)s[i])) return false;
    return s[8] == '\0';
}

void aplicarPedidoAlInventario(Libro* inventario, int totalLibros, const Pedido* pedido) {
    if (!inventario || totalLibros <= 0 || !pedido || pedido->cantidadLibros <= 0) return;

    for (int i = 0; i < pedido->cantidadLibros; i++) {
        const char* cod = pedido->libros[i].codigo;
        int restar = pedido->cantidadPorLibro[i];

        for (int j = 0; j < totalLibros; j++) {
            if (inventario[j].codigo && strcmp(inventario[j].codigo, cod) == 0) {
                if (restar <= 0) break;
                if (inventario[j].cantidad >= restar) inventario[j].cantidad -= restar;
                else inventario[j].cantidad = 0; 
                break;
            }
        }
    }
}

void limpiarFinLinea(char *s) { if (s) s[strcspn(s, "\r\n")] = '\0'; }

char* obtenerTituloPorCodigo(const char* codigo, Pedido* arregloPedidos, int cantidadPedidos) {
    for (int i = 0; i < cantidadPedidos; i++) {
        for (int j = 0; j < arregloPedidos[i].cantidadLibros; j++) {
            if (strcmp(arregloPedidos[i].libros[j].codigo, codigo) == 0) {
                return arregloPedidos[i].libros[j].titulo ? arregloPedidos[i].libros[j].titulo : "";
            }
        }
    }
    return "";
}


int buscarIndiceLibro(char** codigosLibros, int totalLibros, const char* codigo) {
    for (int i = 0; i < totalLibros; i++) {
        if (strcmp(codigosLibros[i], codigo) == 0) {
            return i;
        }
    }
    return -1;
}

// Agregar un nuevo libro con su cantidad
void agregarLibroEstadistica(char*** codigosLibros, int** cantidadesVendidas, int* totalLibros, const char* codigo, int cantidad) {
    *codigosLibros = realloc(*codigosLibros, sizeof(char*) * (*totalLibros + 1));
    *cantidadesVendidas = realloc(*cantidadesVendidas, sizeof(int) * (*totalLibros + 1));

    (*codigosLibros)[*totalLibros] = malloc(strlen(codigo) + 1);
    strcpy((*codigosLibros)[*totalLibros], codigo);
    (*cantidadesVendidas)[*totalLibros] = cantidad;

    (*totalLibros)++;
}

// Ordenar descendentemente por cantidad usando burbuja
void ordenarLibrosVendidos(char** codigosLibros, int* cantidadesVendidas, int totalLibros) {
    for (int i = 0; i < totalLibros - 1; i++) {
        for (int j = 0; j < totalLibros - i - 1; j++) {
            if (cantidadesVendidas[j] < cantidadesVendidas[j + 1]) {
                int tempCantidad = cantidadesVendidas[j];
                cantidadesVendidas[j] = cantidadesVendidas[j + 1];
                cantidadesVendidas[j + 1] = tempCantidad;

                char* tempCodigo = codigosLibros[j];
                codigosLibros[j] = codigosLibros[j + 1];
                codigosLibros[j + 1] = tempCodigo;
            }
        }
    }
}

// Mostrar resultados
void mostrarResultados(char** codigosLibros, int* cantidadesVendidas, int totalLibros) {
    printf("Libros más vendidos:\n");
    for (int i = 0; i < totalLibros; i++) {
        printf("Código: %s, Título: %s, Cantidad: %d\n",
                codigosLibros[i],
                obtenerTituloPorCodigo(codigosLibros[i], arregloPedidos, cantidadPedidosActual),
                cantidadesVendidas[i]);
    }
}