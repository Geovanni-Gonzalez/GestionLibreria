#include "../include/Libro.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINEA 256

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


Libro* cargarLibros(const char* rutaArchivo, int* totalLibros) {
    FILE* archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        *totalLibros = 0;
        return NULL;
    }

    Libro* libros = NULL;
    *totalLibros = 0;
    char linea[MAX_LINEA];

    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar salto de linea
        int i = 0;
        while (linea[i] != '\0') {
            if (linea[i] == '\n' || linea[i] == '\r') {
                linea[i] = '\0';
                break;
            }
            i++;
        }

        // Separar campos manualmente usando ';'
        char* campos[5] = {0};
        int campo = 0;
        char* ptr = linea;
        char* start = linea;

        while (*ptr != '\0' && campo < 5) {
            if (*ptr == ';') {
                *ptr = '\0';
                campos[campo++] = start;
                start = ptr + 1;
            }
            ptr++;
        }
        campos[campo++] = start;

        if (campo != 5) {
            fprintf(stderr, "Formato inválido en línea\n");
            continue;
        }

        // Reservar memoria para un nuevo libro
        libros = realloc(libros, (*totalLibros + 1) * sizeof(Libro));
        if (!libros) {
            perror("Error en realloc");
            fclose(archivo);
            *totalLibros = 0;
            return NULL;
        }

        // Copiar *chars a "strings" 
        libros[*totalLibros].codigo = copiarString(campos[0]);
        libros[*totalLibros].titulo  = copiarString(campos[1]);
        libros[*totalLibros].autor   = copiarString(campos[2]);
        // Copiar *char a float
        libros[*totalLibros].precio = stringAFloat(campos[3]);
        // Copiar *char a int
        libros[*totalLibros].cantidad = stringAInt(campos[4]);

        (*totalLibros)++;
    }

    fclose(archivo);
    return libros;
}

void liberarLibros(Libro* libros, int totalLibros) {
    for (int i = 0; i < totalLibros; i++) {
        free(libros[i].codigo);
        free(libros[i].titulo);
        free(libros[i].autor);
    }
    free(libros);
}

// ---------------- Funciones principales ----------------

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

void mostrarLibros(Libro* libros, int totalLibros) {
    // implementacion pendiente
}

void mostrarLibrosCatalogo(Libro* libros, int totalLibros) {
    // implementacion pendiente
}

void filtrarPorAutor(Libro* libros, int totalLibros, const char* autor) {
    // implementacion pendiente
}

void eliminarLibro(Libro** libros, int* totalLibros, const char* codigo) {
    // implementacion pendiente
}
