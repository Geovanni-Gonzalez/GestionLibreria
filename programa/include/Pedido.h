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
#define MAX_ID  5

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
typedef struct {
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


void inicializarArregloPedidos();

/***
 * @brief Remueve un libro del la lista mientras se crea un pedido.
 * @param codigoLibro Código del libro a remover.
 * @param pedido Puntero al pedido del cual se desea remover el libro.
 * @return void
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido);


void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9], Pedido* arregloPedidos, int* cantidadPedidosActual, Config cfg);

void mostrarDetallePedido(Pedido* pedido, Config cfg);

/**
 * @brief Obtiene todos los pedidos realizados por un cliente específico.
 * @param cedulaCliente Cédula del cliente cuyos pedidos se desean obtener.
 * @param arregloPedidos Arreglo de todos los pedidos existentes.
 * @param cantidadPedidosActual Cantidad actual de pedidos en el arreglo.
 * @param cantidadPedidosCliente Puntero a entero donde se almacenará la cantidad de pedidos del cliente.
 * @return Arreglo dinámico de pedidos realizados por el cliente (debe liberarse con free), o NULL si no tiene pedidos.
 */
Pedido* obtenerPedidosPorCliente(char* cedulaCliente, Pedido* arregloPedidos, int cantidadPedidosActual, int* cantidadPedidosCliente);

#endif /* PEDIDO_H */