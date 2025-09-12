#include "Pedido.h"
#include "Utilidades.h"
#include "Cliente.h"    
#include "Config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


Pedido* arregloPedidos = NULL;     // Arreglo dinámico
int cantidadPedidosActual = 0;      // Cantidad actual de pedidos
int capacidadDePedidosArreglo = 2;  // Tamaño inicial del arreglo


void inicializarArregloPedidos() {
    arregloPedidos = (Pedido*) malloc(capacidadDePedidosArreglo * sizeof(Pedido));
    if (arregloPedidos == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    cargarPedidosDesdeArchivo(&arregloPedidos, &cantidadPedidosActual, &capacidadDePedidosArreglo);
}


/***
 * @brief Remueve un libro del la lista mientras se crea un pedido.
 * @param codigoLibro Código del libro a remover.
 * @param pedido Puntero al pedido del cual se desea remover el libro.
 * @return void 
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido) {
    int indice = -1;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (strcmp(pedido->libros[i].codigo, codigoLibro) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Código de libro no encontrado en el pedido.\n");
        return;
    }


    if (indice != -1) {
        for (int i = indice; i < pedido->cantidadLibros - 1; i++) {
            pedido->libros[i] = pedido->libros[i + 1];
        }
        pedido->cantidadLibros--;
        pedido->libros = realloc(pedido->libros, pedido->cantidadLibros * sizeof(Libro));
    }

    printf("Libro con código %s removido del pedido exitosamente.\n", codigoLibro);
}


void mostrarDetallePedido(Pedido* pedido, Config cfg) {
    Cliente* cliente = obtenerClientePorCedula(pedido->cedula);
    if (!cliente) {
        printf("Cliente no encontrado.\n");
        return;
    }

    printf("\n--- DETALLE PEDIDO ---\n");
    printf("ID del Pedido: %s\n", pedido->id);
    printf("Fecha: %s\n", pedido->fecha);
    printf("Cliente: %s (Cédula: %s, Tel: %s)\n", cliente->nombre, cliente->cedula, cliente->telefono);
    printf("Subtotal: %.2f\n", pedido->subtotal);
    printf("Impuesto (13%%): %.2f\n", pedido->impuesto);
    printf("Total: %.2f\n", pedido->total);
    printf("--- %s ---\n", cfg.nombreLocalComercial);
    printf("Teléfono: %s | Cédula Jurídica: %s\n", cfg.telefono, cfg.cedulaJuridica);
    printf("Horario de Atención: %s\n", cfg.horarioAtencion);
    printf("----------------------\n");

}

void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9], Pedido* arregloPedidos, int* cantidadPedidosActual, Config cfg) {

    // 1. Generar ID del pedido
    char* idGenerado = generarIDPedido(cantidadPedidosActual);
    strcpy(pedido->id, idGenerado);
    free(idGenerado);
    // 2. Asociar cedula y fecha al pedido
    strcpy(pedido->cedula, cedulaCliente);
    strcpy(pedido->fecha, fechaPedido);

    // 3. Se calcula subtotal, impuesto y total
    calcularPreciosPedido(pedido);

    // 4. Marcar el pedido como generado
    pedido->generado = true;

    // 5. Descontar el stock de los libros en el pedido
    descontarStockLibro(pedido->libros, pedido->cantidadPorLibro, pedido->cantidadLibros);

    // 6. Agregar el pedido al arreglo de pedidos y actualizar la cantidad
    arregloPedidos[*cantidadPedidosActual] = *pedido;
    (*cantidadPedidosActual)+=1;

   //Mostrar detalle del pedido (Generar factura)
    mostrarDetallePedido(pedido, cfg);

}

Pedido* obtenerPedidosPorCliente(char* cedulaCliente, Pedido* arregloPedidos, int cantidadPedidosActual, int* cantidadPedidosCliente){
    Pedido* pedidosCliente = NULL;
    *cantidadPedidosCliente = 0;

    for (int i = 0; i < cantidadPedidosActual; i++) {
        if (strcmp(arregloPedidos[i].cedula, cedulaCliente) == 0) {
            pedidosCliente = realloc(pedidosCliente, sizeof(Pedido) * (*cantidadPedidosCliente + 1));
            if (!pedidosCliente) {
                printf("Error al asignar memoria para los pedidos del cliente.\n");
                *cantidadPedidosCliente = 0;
                return NULL;
            }
            pedidosCliente[*cantidadPedidosCliente] = arregloPedidos[i];
            (*cantidadPedidosCliente)++;
        }
    }

    return pedidosCliente;
}


/**
 * @brief Selecciona un libro ingresando su código y cantidad para agregar al pedido.
 *
 * - Si el libro ya existe en el pedido, se aumenta su cantidad.
 * - Se valida que exista stock suficiente en data/libros.txt; si no, no se agrega y se informa
 *   el stock existente.
 * - Al seleccionar libros se muestra la lista de compra en consola (Código, nombre, precio).
 */
void seleccionarLibro(Pedido* pedido, const char* codigoLibro, int cantidad) {
    if (!pedido || !codigoLibro || cantidad <= 0) {
        printf("Datos inválidos para seleccionar libro.\n");
        return;
    }

    // 1) Leer inventario desde data/libros.txt (formato: cod;titulo;autor;precio;cantidad)
    FILE* f = fopen("data/libros.txt", "r");
    if (!f) {
        printf("No se pudo abrir el inventario de libros.\n");
        return;
    }

    char linea[256];
    char* codigo = NULL; float precio = 0.0f; int stock = -1; char* titulo = NULL;
    int encontrado = 0;
    while (fgets(linea, sizeof(linea), f)) {
        // Eliminar salto de línea
        int i = 0;
        while (linea[i] != '\0') {
            if (linea[i] == '\n' || linea[i] == '\r') {
                linea[i] = '\0'; break;
            }
            i++;
        }

        // Separar en ';'
        char* campos[5] = {0};
        int idx = 0;
        char* p = linea;
        char* start = linea;
        while (*p != '\0' && idx < 5) {
            if (*p == ';') {
                *p = '\0';
                campos[idx++] = start;
                start = p + 1;
            }
            p++;
        }
        campos[idx++] = start;
        if (idx != 5) continue;

        if (strcmp(campos[0], codigoLibro) == 0) {
            codigo = campos[0];
            titulo = campos[1];
            precio = stringAFloat(campos[3]);
            stock = stringAInt(campos[4]);
            encontrado = 1;
            break;
        }
    }
    fclose(f);

    if (!encontrado) {
        printf("Código de libro no existe en el inventario.\n");
        return;
    }

    // 2) Validar stock disponible vs cantidad solicitada + ya en el pedido
    int yaEnPedido = 0;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (strcmp(pedido->libros[i].codigo, codigoLibro) == 0) {
            yaEnPedido = pedido->cantidadPorLibro[i];
            break;
        }
    }

    if (cantidad + yaEnPedido > stock) {
        printf("No hay stock suficiente. Disponible: %d\n", stock);
        return;
    }

    // 3) Si ya existe en el pedido, aumentar cantidad. Si no, agregar nueva línea
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (strcmp(pedido->libros[i].codigo, codigoLibro) == 0) {
            pedido->cantidadPorLibro[i] += cantidad;
            // Mostrar lista de compra
            printf("\nLista de compra:\n");
            for (int j = 0; j < pedido->cantidadLibros; j++) {
                const char* tit = pedido->libros[j].titulo ? pedido->libros[j].titulo : "";
                printf("%s - %s - %.2f (x%d)\n", pedido->libros[j].codigo, tit, pedido->libros[j].precio, pedido->cantidadPorLibro[j]);
            }
            return;
        }
    }

    // Agregar nuevo libro al pedido
    Libro* nuevosLibros = realloc(pedido->libros, sizeof(Libro) * (pedido->cantidadLibros + 1));
    int* nuevasCantidades = realloc(pedido->cantidadPorLibro, sizeof(int) * (pedido->cantidadLibros + 1));
    if (!nuevosLibros || !nuevasCantidades) {
        printf("Error al asignar memoria para agregar libro al pedido.\n");
        // Si uno falló pero el otro no, evitar fuga si corresponde
        if (nuevosLibros) pedido->libros = nuevosLibros;
        if (nuevasCantidades) pedido->cantidadPorLibro = nuevasCantidades;
        return;
    }
    pedido->libros = nuevosLibros;
    pedido->cantidadPorLibro = nuevasCantidades;

    // Copiar datos del libro (codigo y titulo deben ser duplicados porque vienen de la línea temporal)
    Libro* lib = &pedido->libros[pedido->cantidadLibros];
    size_t lenCod = strlen(codigo);
    lib->codigo = malloc(lenCod + 1);
    if (lib->codigo) { strcpy(lib->codigo, codigo); }
    size_t lenTit = strlen(titulo);
    lib->titulo = malloc(lenTit + 1);
    if (lib->titulo) { strcpy(lib->titulo, titulo); }
    lib->autor = copiarString(""); // no requerido para la lista de compra, evitar NULL
    lib->precio = precio;
    lib->cantidad = stock; // cantidad en inventario referencial

    pedido->cantidadPorLibro[pedido->cantidadLibros] = cantidad;
    pedido->cantidadLibros++;

    // 4) Mostrar lista de compra actual
    printf("\nLista de compra:\n");
    for (int j = 0; j < pedido->cantidadLibros; j++) {
        const char* tit = pedido->libros[j].titulo ? pedido->libros[j].titulo : "";
        printf("%s - %s - %.2f (x%d)\n", pedido->libros[j].codigo, tit, pedido->libros[j].precio, pedido->cantidadPorLibro[j]);
    }
}

/**
 * @brief Modifica la cantidad de un libro ya presente en el pedido.
 *
 * Si ajusteCantidad  > 0, intenta aumentar validando stock.
 * Si ajusteCantidad  < 0, disminuye la cantidad y elimina la línea si queda en 0.
 * Siempre imprime la lista de compra al final si se realizó algún cambio.
 */
void modificarLibro(Pedido* pedido, const char* codigoLibro, int ajusteCantidad ) {
    if (!pedido || !codigoLibro || ajusteCantidad  == 0) {
        printf("Parámetros inválidos para modificar libro.\n");
        return;
    }

    // Buscar el libro en el pedido
    int idx = -1;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (strcmp(pedido->libros[i].codigo, codigoLibro) == 0) {
            idx = i; break;}
    }

    if (idx == -1) {
        printf("El libro con código %s no existe en la lista de compra.\n", codigoLibro);
        return;
    }

    if (ajusteCantidad  > 0) {
        // Leer inventario para validar stock
        FILE* f = fopen("data/libros.txt", "r");
        if (!f) { printf("No se pudo abrir el inventario de libros.\n"); return; }
        char linea[256];
        int stock = -1;
        while (fgets(linea, sizeof(linea), f)) {
            int k = 0;
            while (linea[k] != '\0') {
                if (linea[k] == '\n' || linea[k] == '\r') {
                    linea[k] = '\0';
                    break;
                } 
                k++;
            }
            char* campos[5] = {0};
            int n = 0;
            char* p = linea;
            char* start = linea;
            while (*p != '\0' && n < 5) {
                if (*p == ';') {
                    *p = '\0';
                    campos[n++] = start;
                    start = p + 1;
                }
                p++;
            }
            campos[n++] = start;
            if (n != 5) continue;
            if (strcmp(campos[0], codigoLibro) == 0) {
                stock = stringAInt(campos[4]);
                break;
            }
        }
        fclose(f);

        if (stock < 0) {
            printf("No se encontró el stock del libro.\n");
            return;
        }

        int nuevaCantidad = pedido->cantidadPorLibro[idx] + ajusteCantidad ;
        if (nuevaCantidad > stock) {
            printf("No hay stock suficiente. Disponible: %d\n", stock);
            return;
        }
        pedido->cantidadPorLibro[idx] = nuevaCantidad;
        
    } else { // ajusteCantidad  < 0
        int nuevaCantidad = pedido->cantidadPorLibro[idx] + ajusteCantidad ; // delta es negativo
        if (nuevaCantidad < 0) {
            printf("No se puede dejar la cantidad negativa. Cantidad actual: %d\n", pedido->cantidadPorLibro[idx]);
            return;
        }

        if (nuevaCantidad == 0) {
            // Eliminar la línea: desplazar elementos y reducir arreglos
            free(pedido->libros[idx].codigo);
            free(pedido->libros[idx].titulo);  
            free(pedido->libros[idx].autor);
            for (int i = idx; i < pedido->cantidadLibros - 1; i++) {
                pedido->libros[i] = pedido->libros[i + 1];
                pedido->cantidadPorLibro[i] = pedido->cantidadPorLibro[i + 1];
            }
            pedido->cantidadLibros--;
            if (pedido->cantidadLibros > 0) {
                Libro* nuevosLibros = realloc(pedido->libros, sizeof(Libro) * pedido->cantidadLibros);
                int* nuevasCant = realloc(pedido->cantidadPorLibro, sizeof(int) * pedido->cantidadLibros);
                if (nuevosLibros) pedido->libros = nuevosLibros;
                if (nuevasCant) pedido->cantidadPorLibro = nuevasCant;
            } else {
                free(pedido->libros); pedido->libros = NULL;
                free(pedido->cantidadPorLibro); pedido->cantidadPorLibro = NULL;
            }
        } else {
            pedido->cantidadPorLibro[idx] = nuevaCantidad;
        }
    }

    // Imprimir lista de compra actualizada
    printf("\nLista de compra:\n");
    for (int j = 0; j < pedido->cantidadLibros; j++) {
        const char* tit = pedido->libros[j].titulo ? pedido->libros[j].titulo : "";
        printf("%s - %s - %.2f (x%d)\n", pedido->libros[j].codigo, tit, pedido->libros[j].precio, pedido->cantidadPorLibro[j]);
    }
}
