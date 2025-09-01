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
    // implementacion pendiente
}

void cargarInventario(Libro** libros, int* totalLibros, const char* rutaArchivo) {
    // implementacion pendiente
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
