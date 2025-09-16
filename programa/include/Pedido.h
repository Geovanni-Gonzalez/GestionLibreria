/**
 * @file Pedido.h
 * @brief Define la estructura y constantes relacionadas con los pedidos en el sistema de gestión de librería.
 *
 * Este archivo contiene la definición de la estructura Pedido, que almacena la información relevante de un pedido,
 * así como las constantes utilizadas para definir los tamaños máximos de los campos y el valor del impuesto.
 */

#ifndef PEDIDO_H
#define PEDIDO_H
#include <stdbool.h>
#include "Libro.h"
#include "Config.h"

/**
 * @def MAX_ID
 * @brief Tamaño máximo para el identificador del pedido (incluye el carácter nulo).
 */
#define MAX_ID  12

/**
 * @def MAX_CEDULA
 * @brief Tamaño máximo para la cédula del cliente (incluye el carácter nulo).
 */
#define MAX_CEDULA 10

/**
 * @def MAX_FECHA
 * @brief Tamaño máximo para la fecha del pedido (incluye el carácter nulo).
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
 * Contiene la información del pedido, incluyendo identificador, cliente asociado, fecha
 * y montos de subtotal, impuesto y total.
 */
typedef struct Pedido{
    char id[MAX_ID];           /**< Identificador único del pedido (cadena terminada en '\0'). */
    char cedula[MAX_CEDULA];   /**< Cédula del cliente (cadena terminada en '\0'). */
    char fecha[MAX_FECHA];     /**< Fecha en la que se realizó el pedido (cadena terminada en '\0'). */
    float subtotal;            /**< Monto antes de aplicar el impuesto. */
    float impuesto;            /**< Monto correspondiente al impuesto aplicado. */
    float total;               /**< Monto total del pedido (subtotal + impuesto). */
    Libro* libros;          /**< Arreglo dinámico de libros incluidos en el pedido. */
    bool generado;          /**< Indica si el pedido ha sido generado (true) o no (false). */
    int cantidadLibros;      /**< Cantidad de libros en el pedido. */
    int *cantidadPorLibro;   /**< Arreglo dinámico que almacena la cantidad de cada libro en el pedido. */
} Pedido;


extern Pedido* arregloPedidos;     // Arreglo dinámico
extern int cantidadPedidosActual;      // Cantidad actual de pedidos
extern int capacidadDePedidosArreglo;  // Tamaño inicial del arreglo


void inicializarArregloPedidos();

/***
 * @brief Remueve un libro del la lista mientras se crea un pedido.
 * @param codigoLibro Código del libro a remover.
 * @param pedido Puntero al pedido del cual se desea remover el libro.
 * @return void
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido);



void mostrarDetallePedido(Pedido* pedido, Config cfg);

/**
 * @brief Obtiene todos los pedidos realizados por un cliente específico.
 * @param cedulaCliente Cédula del cliente cuyos pedidos se desean obtener.
 * @param arregloPedidos Arreglo de todos los pedidos existentes.
 * @param cantidadPedidosActual Cantidad actual de pedidos en el arreglo.
 * @param cantidadPedidosCliente Puntero a entero donde se almacenará la cantidad de pedidos del cliente.
 * @return Arreglo dinámico de pedidos realizados por el cliente (debe liberarse con free), o NULL si no tiene pedidos.
 */
//Pedido* obtenerPedidosPorCliente(char* cedulaCliente, Pedido* arregloPedidos, int cantidadPedidosActual, int* cantidadPedidosCliente);



//void imprimirPedidos(Pedido* arregloPedidos, int cantidadPedidosActual);


/**
 * @brief Selecciona un libro por código y cantidad para agregarlo a un pedido.
 *
 * Si el libro ya se encuentra en el pedido, se incrementa su cantidad.
 * Se valida el stock contra el inventario (data/libros.txt); si no hay suficiente,
 * no se agrega y se informa al usuario el stock disponible.
 * Al finalizar, se muestra en consola la lista de compra (código, nombre y precio).
 *
 * @param pedido Puntero al pedido donde se agregará el libro.
 * @param codigoLibro Código del libro a agregar.
 * @param cantidad Cantidad solicitada del libro.
 */
void seleccionarLibro(Pedido* pedido, const char* codigoLibro, int cantidad);


/**
 * @brief Modifica la cantidad de un libro en la lista del pedido.
 *
 * Permite aumentar (delta > 0) o disminuir (delta < 0) la cantidad de un libro ya
 * presente en el pedido. Si se aumenta, se valida contra el stock del inventario.
 * Si al disminuir la cantidad llega a 0, se elimina la línea del pedido.
 * Siempre muestra la lista de compra luego del cambio.
 *
 * @param pedido Puntero al pedido.
 * @param codigoLibro Código del libro a modificar.
 * @param ajusteCantidad Cantidad a sumar o restar (puede ser negativa).
 */
void modificarLibro(Pedido* pedido, const char* codigoLibro, int ajusteCantidad);


void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9], Pedido* arregloPedidos, int* cantidadPedidosActual, Config* cfg);

void mostrarDetallePedido(Pedido* pedido, Config cfg);

void menuPedidoTrasSeleccion(Libro** inventario, int* totalLibros, int idxSel,
                             Pedido* pedido, Config cfg,
                             Pedido* arregloPedidos, int* cantidadPedidosActual);

int guardarPedidoTxt(const Pedido* pedido, const char* rutaArchivo);

void limpiarPedido(Pedido* p);


int* cantidadPedidos();

int obtenerAnioDeFecha(const char* fecha);

int obtenerMontoPorAnio(int anio, Pedido* arregloPedidos, int cantidadPedidos);

int* obtenerAniosPedidos(Pedido* arregloPedidos, int cantidadPedidos);

// Devuelve true si el código aparece en cualquier pedido
bool libroAsociadoAPedido(const char* codigo, const Pedido* pedidos, int cantidadPedidos);

// Lista en consola todos los pedidos (tabla) y permite seleccionar uno para ver detalle.
void listarPedidosCLI(Pedido* pedidos, int cantidadPedidos, Config cfg);

// Separa la impresion y la seleccion:
void imprimirTablaPedidos(const Pedido* pedidos, int cantidadPedidos);
int  seleccionarPedidoPorIndiceOId(const Pedido* pedidos, int cantidadPedidos);
const Pedido* buscarPedidoPorId(const Pedido* pedidos, int cantidadPedidos, const char* id);


//accesores
int pedidos_count(void);
Pedido* pedidos_data(void);
//setter 
void pedidos_set(Pedido* arr, int n);




#endif /* PEDIDO_H */