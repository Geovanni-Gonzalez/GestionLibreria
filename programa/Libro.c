#include "Libro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utilidades.h"

#define MAX_LINEA 256


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
    generarCodigoUnicoLibro(*libros, *totalLibros, nuevoLibro->codigo);

    (*totalLibros)++;
    printf("Libro agregado exitosamente: %s - %s\n", nuevoLibro->titulo, nuevoLibro->autor);

    // Persistir automáticamente el inventario actualizado en el archivo de libros
    // Nota: se usa la ruta por defecto utilizada en el resto del proyecto
    guardarLibros("data/libros.txt", *libros, *totalLibros);
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


void consultaCatalogo(const char* rutaArchivo, const char* autorFiltro) {
    int totalLibros = 0;
    Libro* libros = cargarLibros(rutaArchivo, &totalLibros);
    if (!libros || totalLibros == 0) {
        printf("No hay libros disponibles en el catálogo.\n");
        if (libros) liberarLibros(libros, totalLibros);
        return;
    }

    int mostrarTodos = 1;
    if (autorFiltro && autorFiltro[0] != '\0') {
        mostrarTodos = 0;
    }

    printf("\n=== CATÁLOGO DE LIBROS ===\n");
    if (!mostrarTodos) {
        printf("Filtro por autor: %s\n", autorFiltro);
    }
    printf("Codigo | Titulo | Autor | Precio (sin impuestos) | Stock\n");

    int encontrados = 0;
    for (int i = 0; i < totalLibros; i++) {
        if (!mostrarTodos && strcmp(libros[i].autor, autorFiltro) != 0) {
            continue;
        }
        printf("%s | %s | %s | %.2f | %d\n",
               libros[i].codigo ? libros[i].codigo : "",
               libros[i].titulo ? libros[i].titulo : "",
               libros[i].autor ? libros[i].autor : "",
               libros[i].precio,
               libros[i].cantidad);
        encontrados++;
    }

    if (encontrados == 0) {
        if (mostrarTodos) {
            printf("No hay libros para mostrar.\n");
        } else {
            printf("No se encontraron libros para el autor especificado.\n");
        }
    } else {
        printf("Total mostrados: %d\n", encontrados);
    }

    liberarLibros(libros, totalLibros);
}

void mostrarLibros(Libro* libros, int totalLibros) {
    // implementacion pendiente
}

void mostrarLibrosCatalogo(Libro* libros, int totalLibros) {
    // implementacion pendiente
}

void filtrarPorAutor(Libro* libros, int totalLibros) {
    if (!libros || totalLibros == 0) {
        printf("No hay libros cargados.\n");
        return;
    }

    char buffer[100];
    printf("Ingrese el nombre del autor a buscar: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error al leer el autor.\n");
        return;
    }

    size_t len = strlen(buffer);
    if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
        buffer[len - 1] = '\0';
    }

    // Reservar memoria dinamica para el autor buscado
    char* autor = malloc(strlen(buffer) + 1);
    if (!autor) {
        printf("Error al reservar memoria.\n");
        return;
    }
    strcpy(autor, buffer);

    int encontrados = 0;
    printf("\nLibros del autor '%s':\n", autor);

    for (int i = 0; i < totalLibros; i++) {
        if (strcmp(libros[i].autor, autor) == 0) {
            printf("(%s, %s, %.2f)\n",
                   libros[i].codigo,
                   libros[i].titulo,
                   libros[i].precio);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron libros de ese autor.\n");
    }

    // Liberar memoria usada para el autor
    free(autor);
}


// Elimina del inventario si NO esta asociado a un pedido
void eliminarLibro(Libro** libros, int* totalLibros, const char* codigo) {
    if (!libros || !*libros || !totalLibros || !codigo) return;

    int n = *totalLibros;
    int idx = -1;
    for (int i = 0; i < n; i++) {
        if ((*libros)[i].codigo && strcmp((*libros)[i].codigo, codigo) == 0) {
            idx = i; break;
        }
    }
    if (idx < 0) {
        printf("Libro [%s] no encontrado.\n", codigo);
        return;
    }

    // liberar strings
    free((*libros)[idx].codigo);
    free((*libros)[idx].titulo);
    free((*libros)[idx].autor);

    // compactar
    for (int i = idx; i < n - 1; i++) {
        (*libros)[i] = (*libros)[i + 1];
    }
    (*totalLibros)--;
    if (*totalLibros == 0) {
        free(*libros);
        *libros = NULL;
    } else {
        Libro* tmp = realloc(*libros, sizeof(Libro) * (*totalLibros));
        if (tmp) *libros = tmp;
    }
    printf("Libro [%s] eliminado del inventario.\n", codigo);
}

// prototipo de asociasion de pedido con libro 
static int estaAsociadoAPedido(const char* codigo) {
    (void)codigo;
    return 0; 
}

void guardarLibros(const char* rutaArchivo, Libro* libros, int totalLibros) {
    FILE* f = fopen(rutaArchivo, "w");
    if (!f) {
        perror("No se pudo abrir el archivo para guardar");
        return;
    }

    for (int i = 0; i < totalLibros; i++) {
        fprintf(f, "%s;%s;%s;%.2f;%d\n",
                libros[i].codigo,
                libros[i].titulo,
                libros[i].autor,
                libros[i].precio,
                libros[i].cantidad);
    }

    fclose(f);
}
