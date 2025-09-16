/**
 * @file Pedido.h
 * @brief Define la estructura y funciones relacionadas con los pedidos en el sistema de gestión de librería.
 *
 * Contiene la definición de la estructura Pedido, constantes de tamaño de campos y porcentaje de impuesto,
 * así como prototipos de funciones para manipular pedidos.
 */

#ifndef PEDIDO_H
#define PEDIDO_H

#include <stdbool.h>
#include "Libro.h"
#include "Config.h"

/**
 * @def MAX_ID
 * @brief Tamaño máximo para el identificador del pedido (incluye el carácter nulo '\0').
 */
#define MAX_ID  12

/**
 * @def MAX_CEDULA
 * @brief Tamaño máximo para la cédula del cliente (incluye el carácter nulo '\0').
 */
#define MAX_CEDULA 10

/**
 * @def MAX_FECHA
 * @brief Tamaño máximo para la fecha del pedido (incluye el carácter nulo '\0').
 */
#define MAX_FECHA 9

/**
 * @def IMPUESTO
 * @brief Porcentaje de impuesto aplicado a los pedidos.
 */
#define IMPUESTO 0.13

/**
 * @struct Pedido
 * @brief Estructura que representa un pedido en el sistema de gestión de librería.
 *
 * Contiene información del pedido como ID, cédula del cliente, fecha, montos, libros asociados
 * y cantidades de cada libro.
 */
typedef struct Pedido {
    char id[MAX_ID];           /**< Identificador único del pedido (cadena terminada en '\0'). */
    char cedula[MAX_CEDULA];   /**< Cédula del cliente (cadena terminada en '\0'). */
    char fecha[MAX_FECHA];     /**< Fecha del pedido (cadena terminada en '\0'). */
    float subtotal;            /**< Monto antes de aplicar el impuesto. */
    float impuesto;            /**< Monto correspondiente al impuesto aplicado. */
    float total;               /**< Monto total del pedido (subtotal + impuesto). */
    Libro* libros;             /**< Arreglo dinámico de libros incluidos en el pedido. */
    bool generado;             /**< Indica si el pedido ha sido generado (true) o no (false). */
    int cantidadLibros;        /**< Cantidad de libros en el pedido. */
    int *cantidadPorLibro;     /**< Arreglo dinámico que almacena la cantidad de cada libro. */
} Pedido;

/**
 * @brief Arreglo dinámico de todos los pedidos en el sistema.
 */
extern Pedido* arregloPedidos;

/**
 * @brief Cantidad actual de pedidos en el arreglo dinámico.
 */
extern int cantidadPedidosActual;

/**
 * @brief Capacidad inicial del arreglo dinámico de pedidos.
 */
extern int capacidadDePedidosArreglo;

/**
 * @brief Inicializa el arreglo dinámico de pedidos.
 */
void inicializarArregloPedidos(void);

/**
 * @brief Remueve un libro de la lista mientras se crea un pedido.
 * 
 * @param codigoLibro Código del libro a remover.
 * @param pedido Puntero al pedido del cual se desea remover el libro.
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido);

/**
 * @brief Muestra en consola el detalle completo de un pedido.
 * 
 * @param pedido Puntero al pedido a mostrar.
 * @param cfg Configuración del sistema para mostrar encabezado o información adicional.
 */
void mostrarDetallePedido(Pedido* pedido, Config cfg);

/**
 * @brief Selecciona un libro por código y cantidad para agregarlo a un pedido.
 * 
 * Si el libro ya existe en el pedido, incrementa la cantidad.
 * Valida el stock disponible y muestra la lista de compra.
 *
 * @param pedido Puntero al pedido donde se agregará el libro.
 * @param codigoLibro Código del libro a agregar.
 * @param cantidad Cantidad solicitada del libro.
 */
void seleccionarLibro(Pedido* pedido, const char* codigoLibro, int cantidad);

/**
 * @brief Modifica la cantidad de un libro en el pedido.
 * 
 * Permite aumentar o disminuir la cantidad. Si la cantidad llega a 0, elimina el libro del pedido.
 *
 * @param pedido Puntero al pedido.
 * @param codigoLibro Código del libro a modificar.
 * @param ajusteCantidad Cantidad a sumar o restar (negativo para disminuir).
 */
void modificarLibro(Pedido* pedido, const char* codigoLibro, int ajusteCantidad);

/**
 * @brief Genera el pedido finalizando la selección de libros.
 * 
 * Calcula montos, aplica impuestos y actualiza el arreglo global de pedidos.
 *
 * @param pedido Puntero al pedido a generar.
 * @param cedulaCliente Cédula del cliente asociado.
 * @param fechaPedido Fecha del pedido.
 * @param arregloPedidos Arreglo global de pedidos.
 * @param cantidadPedidosActual Puntero a la cantidad de pedidos actual.
 * @param cfg Configuración del sistema.
 */
void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9], Pedido* arregloPedidos, int* cantidadPedidosActual, Config* cfg);

/**
 * @brief Muestra un menú para agregar, modificar o remover libros de un pedido tras la selección.
 *
 * @param inventario Arreglo de libros disponibles.
 * @param totalLibros Cantidad total de libros en inventario.
 * @param idxSel Índice del libro seleccionado.
 * @param pedido Puntero al pedido en construcción.
 * @param cfg Configuración del sistema.
 * @param arregloPedidos Arreglo global de pedidos.
 * @param cantidadPedidosActual Puntero a la cantidad actual de pedidos.
 */
void menuPedidoTrasSeleccion(Libro** inventario, int* totalLibros, int idxSel,
                             Pedido* pedido, Config cfg,
                             Pedido* arregloPedidos, int* cantidadPedidosActual);

/**
 * @brief Guarda un pedido en un archivo de texto.
 * 
 * @param pedido Puntero al pedido a guardar.
 * @param rutaArchivo Ruta del archivo donde se almacenará.
 * @return int Retorna 0 si fue exitoso, distinto de 0 si hubo error.
 */
int guardarPedidoTxt(const Pedido* pedido, const char* rutaArchivo);

/**
 * @brief Limpia la memoria de un pedido y reinicia sus campos.
 * 
 * @param p Puntero al pedido a limpiar.
 */
void limpiarPedido(Pedido* p);

/**
 * @brief Obtiene el arreglo de cantidades de pedidos (auxiliar).
 */
int* cantidadPedidos(void);

/**
 * @brief Obtiene el año de una fecha dada en formato AAAAMMDD o similar.
 * 
 * @param fecha Cadena de fecha.
 * @return int Año extraído de la fecha.
 */
int obtenerAnioDeFecha(const char* fecha);

/**
 * @brief Obtiene el monto total de pedidos de un año específico.
 * 
 * @param anio Año a consultar.
 * @param arregloPedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos en el arreglo.
 * @return int Monto total.
 */
int obtenerMontoPorAnio(int anio, Pedido* arregloPedidos, int cantidadPedidos);

/**
 * @brief Obtiene un arreglo de los años de todos los pedidos.
 *
 * @param arregloPedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos.
 * @return int* Arreglo dinámico de años.
 */
int* obtenerAniosPedidos(Pedido* arregloPedidos, int cantidadPedidos);

/**
 * @brief Determina si un libro está asociado a algún pedido.
 *
 * @param codigo Código del libro.
 * @param pedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos.
 * @return true Si el libro aparece en algún pedido.
 * @return false Si no está asociado.
 */
bool libroAsociadoAPedido(const char* codigo, const Pedido* pedidos, int cantidadPedidos);

/**
 * @brief Lista todos los pedidos en consola en formato de tabla.
 * Permite seleccionar un pedido para ver su detalle.
 *
 * @param pedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos.
 * @param cfg Configuración del sistema.
 */
void listarPedidosCLI(Pedido* pedidos, int cantidadPedidos, Config cfg);

/**
 * @brief Imprime la tabla de pedidos sin interacción.
 *
 * @param pedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos.
 */
void imprimirTablaPedidos(const Pedido* pedidos, int cantidadPedidos);

/**
 * @brief Permite seleccionar un pedido por índice o ID.
 *
 * @param pedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos.
 * @return int Índice seleccionado, -1 si no se encontró.
 */
int seleccionarPedidoPorIndiceOId(const Pedido* pedidos, int cantidadPedidos);

/**
 * @brief Busca un pedido por su ID.
 *
 * @param pedidos Arreglo de pedidos.
 * @param cantidadPedidos Cantidad de pedidos.
 * @param id ID del pedido a buscar.
 * @return const Pedido* Puntero al pedido encontrado, NULL si no existe.
 */
const Pedido* buscarPedidoPorId(const Pedido* pedidos, int cantidadPedidos, const char* id);

/**
 * @brief Obtiene el mes de una fecha dada en formato AAAAMMDD o similar.
 *
 * @param fecha Cadena de fecha.
 * @return int Mes extraído (1-12).
 */
int obtenerMesDeFecha(const char* fecha);

/**
 * @brief Accesor: obtiene la cantidad de pedidos.
 * @return int Cantidad de pedidos.
 */
int pedidos_count(void);

/**
 * @brief Accesor: obtiene el arreglo global de pedidos.
 * @return Pedido* Puntero al arreglo de pedidos.
 */
Pedido* pedidos_data(void);

/**
 * @brief Setter: asigna un nuevo arreglo de pedidos y su tamaño.
 *
 * @param arr Arreglo de pedidos.
 * @param n Cantidad de pedidos.
 */
void pedidos_set(Pedido* arr, int n);

#endif /* PEDIDO_H */