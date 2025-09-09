#ifndef LIBRO_H
#define LIBRO_H

/**
 * @file Libro.h
 * @brief Definición de la estructura Libro y funciones para manejar el inventario.
 */

/**
 * @struct Libro
 * @brief Representa un libro en el inventario de la librería.
 */
typedef struct {
    char* codigo;   /**< Código único del libro */
    char* titulo;   /**< Título del libro */
    char* autor;    /**< Autor del libro */
    float precio;   /**< Precio del libro */
    int cantidad;   /**< Cantidad en inventario */
} Libro;

/**
 * @brief Carga libros desde un archivo en memoria dinámica.
 * @param rutaArchivo Ruta del archivo a cargar.
 * @param totalLibros Puntero a la cantidad de libros cargados.
 * @return Arreglo dinámico de libros.
 */
Libro* cargarLibros(const char* rutaArchivo, int* totalLibros);

/**
 * @brief Libera la memoria de un arreglo de libros.
 */
void liberarLibros(Libro* libros, int totalLibros);

/**
 * @brief Agrega un libro a un arreglo dinámico.
 */
void agregarLibro(Libro** libros, int* totalLibros,
                  const char* titulo, const char* autor,
                  float precio, int cantidad);

/**
 * @brief Carga el inventario desde un archivo.
 */
void cargarInventario(Libro** libros, int* totalLibros,
                      const char* rutaArchivo);

/**
 * @brief Muestra todos los libros con todos los detalles.
 */
void mostrarLibros(Libro* libros, int totalLibros);

/**
 * @brief Muestra los libros en formato catálogo (solo título, autor y precio).
 */
void mostrarLibrosCatalogo(Libro* libros, int totalLibros);

/**
 * @brief Filtra y muestra los libros de un autor específico.
 */
void filtrarPorAutor(Libro* libros, int totalLibros, const char* autor);

/**
 * @brief Elimina un libro por su código.
 */
void eliminarLibro(Libro** libros, int* totalLibros, const char* codigo);

#endif /* LIBRO_H */
