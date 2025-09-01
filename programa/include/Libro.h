#ifndef LIBRO_H
#define LIBRO_H

typedef struct {
    char* codigo;
    char* titulo;
    char* autor;
    float precio;
    int cantidad;
} Libro;

Libro* cargarLibros(const char* rutaArchivo, int* totalLibros);
void liberarLibros(Libro* libros, int totalLibros);

// Agrega un libro al arreglo dinamico
void agregarLibro(Libro** libros, int* totalLibros, const char* titulo, const char* autor, float precio, int cantidad);

// Carga un inventario desde un archivo 
void cargarInventario(Libro** libros, int* totalLibros, const char* rutaArchivo);

// Muestra todos los libros con todos los detalles
void mostrarLibros(Libro* libros, int totalLibros);

// Muestra los libros en formato catalogo (solo título, autor, precio)
void mostrarLibrosCatalogo(Libro* libros, int totalLibros);

// Filtra y muestra los libros de un autor especifico
void filtrarPorAutor(Libro* libros, int totalLibros, const char* autor);

// Elimina un libro por su codigo
void eliminarLibro(Libro** libros, int* totalLibros, const char* codigo);


#endif
