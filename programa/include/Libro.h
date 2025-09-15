/**
 * @file Libro.h
 * @brief Declaración de la estructura Libro y funciones para manejar el inventario de la librería.
 *
 * Este archivo define la estructura que representa un libro en el inventario, así como funciones
 * para cargar, liberar, agregar, eliminar, filtrar y mostrar libros. Se utiliza memoria dinámica
 * para permitir que el inventario crezca según sea necesario.
 *
 * @author Geovanni
 * @date 2025
 */

#ifndef LIBRO_H
#define LIBRO_H

/**
 * @struct Libro
 * @brief Representa un libro en el inventario de la librería.
 *
 * Contiene información básica de un libro, incluyendo:
 * - código único
 * - título
 * - autor
 * - precio
 * - cantidad en inventario
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
 *
 * Lee el archivo especificado y devuelve un arreglo dinámico de libros.
 *
 * @param rutaArchivo Ruta del archivo a cargar.
 * @param totalLibros Puntero a un entero donde se guardará la cantidad de libros cargados.
 * @return Puntero al arreglo dinámico de libros. NULL si ocurre un error.
 */
Libro* cargarLibros(const char* rutaArchivo, int* totalLibros);

/**
 * @brief Libera la memoria utilizada por un arreglo de libros.
 *
 * Debe llamarse cuando ya no se necesiten los libros para evitar fugas de memoria.
 *
 * @param libros Arreglo dinámico de libros.
 * @param totalLibros Cantidad de libros en el arreglo.
 */
void liberarLibros(Libro* libros, int totalLibros);

/**
 * @brief Agrega un libro al arreglo dinámico.
 *
 * Redimensiona el arreglo si es necesario.
 *
 * @param libros Puntero al arreglo dinámico de libros.
 * @param totalLibros Puntero al contador de libros.
 * @param titulo Título del libro a agregar.
 * @param autor Autor del libro a agregar.
 * @param precio Precio del libro a agregar.
 * @param cantidad Cantidad inicial del libro.
 */
void agregarLibro(Libro** libros, int* totalLibros,
                    const char* titulo, const char* autor,
                    float precio, int cantidad);

/**
 * @brief Carga el inventario completo desde un archivo.
 *
 * Actualiza el arreglo dinámico de libros con los datos del archivo.
 *
 * @param libros Puntero al arreglo dinámico de libros.
 * @param totalLibros Puntero a la cantidad de libros en el arreglo.
 * @param rutaArchivo Ruta del archivo de inventario.
 */
void cargarInventario(Libro** libros, int* totalLibros,
                      const char* rutaArchivo);

/**
 * @brief Muestra todos los libros con todos los detalles.
 *
 * Incluye código, título, autor, precio y cantidad.
 *
 * @param libros Arreglo de libros.
 * @param totalLibros Cantidad de libros en el arreglo.
 */
void mostrarLibros(Libro* libros, int totalLibros);

/**
 * @brief Muestra los libros en formato catálogo.
 *
 * Solo se muestra título, autor y precio.
 *
 * @param libros Arreglo de libros.
 * @param totalLibros Cantidad de libros en el arreglo.
 */
void mostrarLibrosCatalogo(Libro* libros, int totalLibros);

/**
 * @brief Filtra y muestra los libros de un autor específico.
 *
 * @param libros Arreglo de libros.
 * @param totalLibros Cantidad de libros en el arreglo.
 * @param autor Nombre del autor a filtrar.
 */
void filtrarPorAutor(Libro* libros, int totalLibros);

/**
 * @brief Elimina un libro del arreglo según su código.
 *
 * Redimensiona el arreglo si es necesario.
 *
 * @param libros Puntero al arreglo dinámico de libros.
 * @param totalLibros Puntero a la cantidad de libros en el arreglo.
 * @param codigo Código del libro a eliminar.
 */
void eliminarLibro(Libro** libros, int* totalLibros, const char* codigo);

/**
 * @brief Consulta y muestra el catálogo de libros.
 *
 * Muestra: código, título, autor, precio sin impuestos y cantidad en stock.
 * Si autorFiltro es NULL o cadena vacía, se muestran todos los libros.
 * Si autorFiltro tiene valor, se muestran solo los libros de ese autor.
 *
 * @param rutaArchivo Ruta del archivo de libros (por ejemplo: "data/libros.txt").
 * @param autorFiltro Autor a filtrar (opcional). Usar NULL o "" para no filtrar.
 */
void consultaCatalogo(const char* rutaArchivo, const char* autorFiltro);
// Se encarga de hacer cambios en el txt de libros
void guardarLibros(const char* rutaArchivo, Libro* libros, int totalLibros);

#endif /* LIBRO_H */
