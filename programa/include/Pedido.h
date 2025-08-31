#ifndef PEDIDO_H
#define PEDIDO_H

#define MAX_ID  5
#define MAX_CEDULA 10
#define MAX_FECHA 9
#define IMPUESTO 0.13


typedef struct {
    char id[MAX_ID];           /**< Identificador único del pedido (cadena terminada en '\0'). */
    char cedula[MAX_CEDULA];   /**< Cédula del cliente (cadena terminada en '\0'). */
    char fecha[MAX_FECHA];     /**< Fecha del pedido (cadena terminada en '\0'). */
    float total;
    float subtotal;
    float impuesto;
} Pedido;


#endif 