#include "Pedido.h"
#include "Utilidades.h"
#include "Cliente.h"    
#include "Config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Declaración de variables globales accesibles externamente
Pedido* arregloPedidos = NULL;     // Arreglo dinámico
int cantidadPedidosActual = 0;      // Cantidad actual de pedidos
int capacidadDePedidosArreglo = 2;  // Tamaño inicial del arreglo

void inicializarPedido(Pedido* p) {
    if (!p) return;
    p->libros = NULL;
    p->cantidadPorLibro = NULL;
    p->cantidadLibros = 0;
    p->subtotal = p->impuesto = p->total = 0.0f;
    p->generado = false;
}

void inicializarArregloPedidos() {
    arregloPedidos = (Pedido*) malloc(capacidadDePedidosArreglo * sizeof(Pedido));
    if (arregloPedidos == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    cargarPedidosDesdeArchivo(&arregloPedidos, &cantidadPedidosActual, &capacidadDePedidosArreglo);
}


/**
 * @brief Remueve un libro del la lista mientras se crea un pedido.
 * @param[in] codigoLibro Código del libro a remover (ej. "L0001").
 * @param[in,out] pedido Puntero al pedido del cual se desea remover el libro.
 * @return void
 * @pre pedido != NULL.
 * @post Si el libro existía en el pedido:
 *       - Se libera la memoria de sus cadenas (codigo/titulo/autor).
 *       - Se compactan arreglos y se reduce `cantidadLibros`.
 *       - Si el pedido queda vacío, se liberan `pedido->libros` y `pedido->cantidadPorLibro`.
 * @note No modifica el inventario; sólo el pedido en edición.
 */
void removerLibroDeListaPedido(char* codigoLibro, Pedido* pedido) {
    if (!pedido || pedido->cantidadLibros <= 0) {
        printf("Pedido vacío.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < pedido->cantidadLibros; i++) {
        if (pedido->libros[i].codigo && strcmp(pedido->libros[i].codigo, codigoLibro) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Código de libro no encontrado en el pedido.\n");
        return;
    }

    // liberar strings del libro removido 
    free(pedido->libros[idx].codigo);
    free(pedido->libros[idx].titulo);
    free(pedido->libros[idx].autor);

    // compactar ambos arreglos: libros y cantidadPorLibro
    for (int i = idx; i < pedido->cantidadLibros - 1; i++) {
        pedido->libros[i] = pedido->libros[i + 1];
        pedido->cantidadPorLibro[i] = pedido->cantidadPorLibro[i + 1];
    }
    pedido->cantidadLibros--;

    if (pedido->cantidadLibros == 0) {
        // si quedó vacio, libera y anula
        free(pedido->libros); pedido->libros = NULL;
        free(pedido->cantidadPorLibro); pedido->cantidadPorLibro = NULL;
    } else {
        Libro* nl = realloc(pedido->libros, pedido->cantidadLibros * sizeof(Libro));
        if (nl) pedido->libros = nl;
        int* nc = realloc(pedido->cantidadPorLibro, pedido->cantidadLibros * sizeof(int));
        if (nc) pedido->cantidadPorLibro = nc;
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

/**
 * @brief Completa/genera un pedido: asigna ID, copia cédula/fecha, calcula totales y lo agrega al arreglo.
 * @param[in,out] pedido Pedido en edicion; se marcara como generado y con ID asignado.
 * @param[in] cedulaCliente Cadena (9 dígitos + '\0').
 * @param[in] fechaPedido Cadena (YYYYMMDD + '\0').
 * @param[in,out] arregloPedidos Arreglo destino en memoria al que se agrega el pedido.
 * @param[in,out] cantidadPedidosActual Contador de pedidos; se incrementa en 1.
 * @param[in,out] cfg Config desde donde se toma "numeroSiguientePedido" para el ID y que luego
 *                    debe actualizarse fuera de esta funcion.
 * @return void
 * @post pedido->generado == true. Se incrementa *cantidadPedidosActual.
 * @note Esta función **no** persiste el pedido a disco ni descuenta inventario; 
 */
void generarPedido(Pedido* pedido, char cedulaCliente[10], char fechaPedido[9],
                   Pedido* arregloPedidos, int* cantidadPedidosActual, Config* cfg) {

    // 1) Generar ID persistente con el consecutivo de admin.json
    snprintf(pedido->id, MAX_ID, "P%06d", cfg->numeroSiguientePedido);

    // 2) Asociar cédula y fecha
    strcpy(pedido->cedula, cedulaCliente);
    strcpy(pedido->fecha,  fechaPedido);

    // 3) Calcular montos
    calcularPreciosPedido(pedido);

    // 4) Marcar generado
    pedido->generado = true;

   
    // descontarStockLibro(pedido->libros, pedido->cantidadPorLibro, pedido->cantidadLibros);

    // 6) Agregar el arreglo en memoria
    arregloPedidos[*cantidadPedidosActual] = *pedido;
    (*cantidadPedidosActual)++;

    // 7) Incrementar y persistir el consecutivo para la próxima vez
    snprintf(pedido->id, MAX_ID, "P%06d", cfg->numeroSiguientePedido);
    // 8) Mostrar detalle 
    mostrarDetallePedido(pedido, *cfg);
}

static char* duplicar(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char* p = (char*)malloc(n + 1);
    if (p) memcpy(p, s, n + 1);
    return p;
}

static int buscarLibroPorCodigo(Libro* arr, int n, const char* codigo) {
    if (!arr || !codigo) return -1;
    for (int i = 0; i < n; i++)
        if (arr[i].codigo && strcmp(arr[i].codigo, codigo) == 0) return i;
    return -1;
}

/**
 * @brief Agrega (o acumula) un libro al pedido por codigo y cantidad.
 * @param[in,out] pedido Pedido en edicion.
 * @param[in] inventario Arreglo de libros del inventario.
 * @param[in] totalLibros Tamaño del inventario.
 * @param[in] codigo Codigo del libro a agregar.
 * @param[in] cantidad Unidades a agregar (>0).
 * @return void
 * @pre cantidad > 0 y debe haber stock suficiente (considerando lo ya pedido).
 * @post Si el libro ya estaba en el pedido, se acumula; si no, se inserta como nuevo item.
 * @note Duplica cadenas del libro para mantener una variante temporal independiente del inventario.
 */
static void agregarLibroAPedido(Pedido* pedido, Libro* inventario, int totalLibros,
                                const char* codigo, int cantidad) {
    if (!pedido || !inventario || !codigo || cantidad <= 0) {
        printf("Parámetros inválidos.\n"); return;
    }
    int idxInv = buscarLibroPorCodigo(inventario, totalLibros, codigo);
    if (idxInv < 0) { printf("Código no existe en inventario.\n"); return; }

    int idxPed = buscarLibroPorCodigo(pedido->libros, pedido->cantidadLibros, codigo);
    int ya = (idxPed >= 0) ? pedido->cantidadPorLibro[idxPed] : 0;

    if (inventario[idxInv].cantidad < ya + cantidad) {
        printf("Stock insuficiente. Disponible: %d\n", inventario[idxInv].cantidad);
        return;
    }

    if (idxPed >= 0) {
        pedido->cantidadPorLibro[idxPed] += cantidad;
        printf("Actualizado [%s] a %d unidades.\n", codigo, pedido->cantidadPorLibro[idxPed]);
        return;
    }

    // insertar nuevo item (copias de las cadenas)
    Libro* nl = realloc(pedido->libros, (pedido->cantidadLibros + 1) * sizeof(Libro));
    int*   nc = realloc(pedido->cantidadPorLibro, (pedido->cantidadLibros + 1) * sizeof(int));
    if (!nl || !nc) {
        printf("Error de memoria.\n");
        if (nl) pedido->libros = nl;
        if (nc) pedido->cantidadPorLibro = nc;
        return;
    }
    pedido->libros = nl;
    pedido->cantidadPorLibro = nc;

    int pos = pedido->cantidadLibros;
    pedido->libros[pos].codigo   = duplicar(inventario[idxInv].codigo);
    pedido->libros[pos].titulo   = duplicar(inventario[idxInv].titulo);
    pedido->libros[pos].autor    = duplicar(inventario[idxInv].autor);
    pedido->libros[pos].precio   = inventario[idxInv].precio;
    pedido->libros[pos].cantidad = 0; 

    pedido->cantidadPorLibro[pos] = cantidad;
    pedido->cantidadLibros++;

    printf("Agregado [%s] x %d al pedido.\n", codigo, cantidad);
}

/**
 * @brief Muestra por consola el contenido actual del pedido en edicion.
 * @param[in] p Pedido a listar.
 * @return void
 */
static void mostrarPedidoActual(const Pedido* p) {
    if (!p || p->cantidadLibros == 0) { printf("\nPedido vacío.\n"); return; }
    printf("\n=== Pedido actual ===\n");
    float total = 0.0f;
    for (int i = 0; i < p->cantidadLibros; i++) {
        float sub = p->libros[i].precio * (float)p->cantidadPorLibro[i];
        printf("%2d) [%s] %-28s x %d  @ %.2f  => %.2f\n",
               i+1, p->libros[i].codigo, p->libros[i].titulo ? p->libros[i].titulo : "",
               p->cantidadPorLibro[i], p->libros[i].precio, sub);
        total += sub;
    }
    printf("Total parcial: %.2f\n", total);
}

/**
 * @brief Cambia la cantidad pedida de un libro; si nuevaCantidad==0, lo remueve del pedido.
 * @param[in,out] pedido Pedido en edicion.
 * @param[in] inventario Inventario actual (para validar stock).
 * @param[in] totalLibros Tamaño del inventario.
 * @param[in] codigo Codigo del libro a modificar.
 * @param[in] nuevaCantidad Nueva cantidad (>=0).
 * @return void
 * @note Si tras remover queda vacio, imprime mensaje y retorna (de esta manera muestra el catalogo y continua el flujo).
 */
static void modificarCantidadEnPedido(Pedido* pedido, Libro* inventario, int totalLibros,
                                      const char* codigo, int nuevaCantidad) {
    if (!pedido || !inventario || !codigo || nuevaCantidad < 0) {
        printf("Parámetros inválidos.\n"); return;
    }
    if (pedido->cantidadLibros <= 0) {
        printf("Pedido vacío.\n"); return;
    }

    int idxPed = buscarLibroPorCodigo(pedido->libros, pedido->cantidadLibros, codigo);
    if (idxPed < 0) { printf("Ese libro no está en el pedido.\n"); return; }

    if (nuevaCantidad == 0) {
        removerLibroDeListaPedido((char*)codigo, pedido);
        if (pedido->cantidadLibros == 0) {
            printf("Pedido vacío, regresando al catálogo...\n");
        }
        return;
    }

    int idxInv = buscarLibroPorCodigo(inventario, totalLibros, codigo);
    if (idxInv < 0) { printf("El libro ya no existe en inventario.\n"); return; }
    if (inventario[idxInv].cantidad < nuevaCantidad) {
        printf("Stock insuficiente. Disponible: %d\n", inventario[idxInv].cantidad);
        return;
    }

    pedido->cantidadPorLibro[idxPed] = nuevaCantidad;
    printf("Cantidad de [%s] actualizada a %d.\n", codigo, nuevaCantidad);
}


/**
 * @brief Persiste un pedido al final del archivo TXT en el formato:
 *        "ID;CEDULA;FECHA;SUBTOTAL;IMPUESTO;TOTAL;GENERADO;CANTIDADLIBROS;{COD:QTY,...}""
 * @param[in] p Pedido a guardar.
 * @param[in] rutaArchivo Ruta del archivo ("../y/x.txt").
 * @return 1 si se guardo OK, 0 si fallo.
 * @note Abre en modo append ("a"). No escribe inventario ni cliente, solo el pedido.
 */
int guardarPedidoTxt(const Pedido* p, const char* rutaArchivo) {
    if (!p || !rutaArchivo) return 0;

    FILE* f = fopen(rutaArchivo, "a");   // append
    if (!f) {
        perror("No se pudo abrir pedidos.txt");
        return 0;
    }

    // ID;CEDULA;FECHA;SUBTOTAL;IMPUESTO;TOTAL;GENERADO;CANTIDADLIBROS;{COD:QTY,...}
    fprintf(f, "%s;%s;%s;%.2f;%.2f;%.2f;%d;%d;{",
            p->id,
            p->cedula,
            p->fecha,
            p->subtotal,
            p->impuesto,
            p->total,
            p->generado ? 1 : 0,
            p->cantidadLibros);

    for (int i = 0; i < p->cantidadLibros; i++) {
        const char* cod = (p->libros[i].codigo ? p->libros[i].codigo : "");
        int qty = (p->cantidadPorLibro ? p->cantidadPorLibro[i] : 0);
        fprintf(f, "%s:%d", cod, qty);
        if (i < p->cantidadLibros - 1) fputc(',', f);
    }

    fprintf(f, "}\n");
    fclose(f);
    return 1;
}

/**
 * @brief Deja el pedido en estado “en blanco” y libera toda memoria dinamica asociada.
 * @param[in,out] p Pedido a limpiar.
 * @return void
 * @post p->libros == NULL, p->cantidadPorLibro == NULL, p->cantidadLibros == 0,
 *       totales en 0, flags e identificadores vacios.
 */
void limpiarPedido(Pedido* p) {
    if (!p) return;

    // Liberar copias hechas al agregar al pedido
    for (int i = 0; i < p->cantidadLibros; i++) {
        free(p->libros[i].codigo);
        free(p->libros[i].titulo);
        free(p->libros[i].autor);
        // p->libros[i].precio y cantidad son escalares
    }

    free(p->libros);
    free(p->cantidadPorLibro);

    p->libros = NULL;
    p->cantidadPorLibro = NULL;
    p->cantidadLibros = 0;

    p->subtotal = 0.0f;
    p->impuesto = 0.0f;
    p->total    = 0.0f;
    p->generado = false;

    p->id[0]     = '\0';
    p->cedula[0] = '\0';
    p->fecha[0]  = '\0';
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
void seleccionarLibro(Pedido* pedido, const char* codigoLibro, int cantidad, Libro* inventario, int totalLibros, Config configuracion) {
    if (!pedido || !codigoLibro || cantidad <= 0) {
        printf(" Datos inválidos para seleccionar libro.\n");
        return;
    }

    // 1) Buscar libro en inventario
    int idxInventario = buscarLibroPorCodigo(inventario, totalLibros, codigoLibro);
    if (idxInventario < 0) {
        printf("Código de libro [%s] no existe en el inventario.\n", codigoLibro);
        return;
    }

    int stock = inventario[idxInventario].cantidad;
    if (stock < cantidad) {
        printf("Stock insuficiente. Disponible: %d\n", stock);
        return;
    }

    // 2) Verificar si ya está en el pedido
    int idxPedido = buscarLibroPorCodigo(pedido->libros, pedido->cantidadLibros, codigoLibro);
    if (idxPedido >= 0) {
        // Ya existe → aumentar cantidad
        if (pedido->cantidadPorLibro[idxPedido] + cantidad > stock) {
            printf("No hay stock suficiente para aumentar. Disponible: %d\n", stock);
            return;
        }
        pedido->cantidadPorLibro[idxPedido] += cantidad;
        printf("Cantidad del libro [%s] actualizada a %d en el pedido.\n",
               codigoLibro, pedido->cantidadPorLibro[idxPedido]);
        mostrarPedidoActual(pedido);
        return;
    }

    // 3) Redimensionar arreglos de forma segura
    Libro* tmpLibros = realloc(pedido->libros, sizeof(Libro) * (pedido->cantidadLibros + 1));
    if (!tmpLibros) {
        printf("Error al asignar memoria para libros.\n");
        return;
    }
    pedido->libros = tmpLibros;

    int* tmpCantidades = realloc(pedido->cantidadPorLibro, sizeof(int) * (pedido->cantidadLibros + 1));
    if (!tmpCantidades) {
        printf("Error al asignar memoria para cantidades.\n");
        return;
    }
    pedido->cantidadPorLibro = tmpCantidades;

    // 4) Copiar datos del libro (deep copy de strings)
    Libro* nuevoLibro = &pedido->libros[pedido->cantidadLibros];

    nuevoLibro->codigo = copiarString(inventario[idxInventario].codigo);
    nuevoLibro->titulo = copiarString(inventario[idxInventario].titulo);
    nuevoLibro->autor  = copiarString(inventario[idxInventario].autor ? inventario[idxInventario].autor : "");
    nuevoLibro->precio = inventario[idxInventario].precio;
    nuevoLibro->cantidad = inventario[idxInventario].cantidad; // referencia del inventario

    pedido->cantidadPorLibro[pedido->cantidadLibros] = cantidad;
    pedido->cantidadLibros++;

    // 5) Mostrar pedido actualizado
    printf("\nLista de compra actualizada:\n");
    for (int j = 0; j < pedido->cantidadLibros; j++) {
        printf(" - %s | %s | %.2f (x%d)\n",
               pedido->libros[j].codigo,
               pedido->libros[j].titulo,
               pedido->libros[j].precio,
               pedido->cantidadPorLibro[j]);
    }
    menuPedidoTrasSeleccion(&inventario, &totalLibros, 0, pedido, configuracion, arregloPedidos, &cantidadPedidosActual);
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

//----------------- Menú tras seleccionar libro -----------------
/**
 * @brief Menu interactivo para operar sobre el libro seleccionado del catalogo.
 * @param[in,out] inventario Doble puntero al arreglo de libros en inventario.
 * @param[in,out] totalLibros Numero de libros en inventario.
 * @param[in] idxSel Indice del libro actualmente seleccionado en el catalogo.
 * @param[in,out] pedido Pedido en edicion.
 * @param[in] cfg Config (se pasa por valor para impresion del detalle).
 * @param[in,out] arregloPedidos Arreglo de pedidos en memoria (historico de todo el txt).
 * @param[in,out] cantidadPedidosActual Contador de pedidos de las ejecuciones.
 * @return void
 * @details Opciones:
 *  1) Agregar libro al pedido actual.
 *  2) Modificar cantidad (0 = remover).
 *  3) Remover libro por codigo.
 *  4) Generar pedido (valida cedula/fecha, guarda TXT, actualiza inventario y admin.json).
 *  5) Salir del menu (volver al catalogo).
 * @note Si el pedido queda vacio despues de una operacion, se retorna al catalogo automaticamente.
 */
void menuPedidoTrasSeleccion(
    Libro** inventario, int* totalLibros, int idxSel,
    Pedido* pedido, Config cfg,
    Pedido* arregloPedidos, int* cantidadPedidosActual
    ) {
    if (!inventario || !*inventario || !totalLibros || *totalLibros <= 0 ||
        !pedido || idxSel < 0 || idxSel >= *totalLibros ||
        !arregloPedidos || !cantidadPedidosActual) {
        printf("Parámetros inválidos para el menú de pedido.\n");
        return;
    }

    int opcion = 0;
    char linea[128];

    do {
        printf("\nLibro seleccionado: [%s] %s  $%.2f  (Stock: %d)\n",
               (*inventario)[idxSel].codigo,
               (*inventario)[idxSel].titulo,
               (*inventario)[idxSel].precio,
               (*inventario)[idxSel].cantidad);

        mostrarPedidoActual(pedido);

        printf("\n=== Menú de Pedido ===\n");
        printf("1. Agregar libro a pedido\n");
        printf("2. Modificar pedido de libro\n");
        printf("3. Remover libro\n");
        printf("4. Generar pedido\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");

        if (!fgets(linea, sizeof(linea), stdin)) { clearerr(stdin); continue; }
        opcion = atoi(linea);

        switch (opcion) {
            case 1: {
                // Agregar el libro actualmente seleccionado
                printf("Cantidad a agregar: ");
                if (!fgets(linea, sizeof(linea), stdin)) { clearerr(stdin); break; }
                int cant = atoi(linea);
                agregarLibroAPedido(pedido, *inventario, *totalLibros,
                                    (*inventario)[idxSel].codigo, cant);
            } break;

            case 2: {
                char codigo[64];
                printf("Código del libro a modificar: ");
                if (!fgets(codigo, sizeof(codigo), stdin)) { clearerr(stdin); break; }
                limpiarFinLinea(codigo);

                char linea[128];
                printf("Nueva cantidad (0 para remover): ");
                if (!fgets(linea, sizeof(linea), stdin)) { clearerr(stdin); break; }
                int nueva = atoi(linea);

                modificarCantidadEnPedido(pedido, *inventario, *totalLibros, codigo, nueva);

                if (pedido->cantidadLibros == 0) {
                    return; // Regresa al catalogo
                }
            } break;

            case 3: {
                char codigo[64];
                printf("Código del libro a remover: ");
                if (!fgets(codigo, sizeof(codigo), stdin)) { clearerr(stdin); break; }
                limpiarFinLinea(codigo);

                removerLibroDeListaPedido(codigo, pedido);

                if (pedido->cantidadLibros == 0) {
                    return;
                }
            } break;

        case 4: {
            if (pedido->generado) {
                puts("Este pedido ya fue generado y guardado.");
                break;
            }
            if (pedido->cantidadLibros <= 0) {
                puts("El pedido está vacío.");
                break;
            }

            char ced[16], fec[16];

            /* CÉDULA */
            printf("Cédula (9 dígitos, sin guiones): ");
            if (!fgets(ced, sizeof(ced), stdin)) { clearerr(stdin); break; }
            limpiarFinLinea(ced);
            if (!validarCedula9(ced)) {           
                puts("Cédula inválida.");
                break;
            }

            /* Verificar cliente */
            Cliente* cli = obtenerClientePorCedula(ced);
            if (!cli) {
                puts("Cliente no encontrado. Regístrelo o intente de nuevo.");
                break; 
            }

            /* FECHA */
            printf("Fecha (YYYYMMDD): ");
            if (!fgets(fec, sizeof(fec), stdin)) { clearerr(stdin); break; }
            limpiarFinLinea(fec);
            if (!validarFechaYYYYMMDD(fec)) {     
                puts("Fecha inválida. Use solo dígitos, formato YYYYMMDD.");
                break;
            }

            /* Generar */
            
            generarPedido(pedido, ced, fec, arregloPedidos, cantidadPedidosActual, &cfg);

            /* Guardar pedido */
            guardarPedidoTxt(pedido, "data/pedidos.txt");

            /* Incrementar correlativo y persistir admin.json */
            actualizarNumeroSiguientePedido(&cfg);
            guardarConfig("data/admin.json", &cfg);

            /* Aplicar al inventario y persistir */
            aplicarPedidoAlInventario(*inventario, *totalLibros, pedido);
            guardarLibros("data/libros.txt", *inventario, *totalLibros);
            puts("Inventario actualizado y guardado.");

            /* Dejar el pedido limpio y volver al catalogo */
            limpiarPedido(pedido);
            return; /* salimos del menu y volvemos al loop del catalogo */
        } break;


            case 5:
                printf("Saliendo del menú.\n");
                break;

            default:
                printf("Opción inválida. Intente de nuevo.\n");
        }

    } while (opcion != 5);
}



// Retornar direccion de variable cantidadPedidosActual
int* cantidadPedidos() {
    return &cantidadPedidosActual;
}

// Formato fecha DD/MM/AAAA
int obtenerAnioDeFecha(const char* fecha) {
    if (fecha == NULL || strlen(fecha) < 10) return -1;
    char anio[5] = {0};
    strncpy(anio, fecha + 6, 4);
    return atoi(anio);
}

// Formato fecha DD/MM/AAAA -> extrae MM
int obtenerMesDeFecha(const char* fecha) {
    if (fecha == NULL || strlen(fecha) < 5) return -1;
    char mes[3] = {0};
    strncpy(mes, fecha + 3, 2);
    return atoi(mes);
}


int obtenerMontoPorAnio(int anio, Pedido* arregloPedidos, int cantidadPedidos) {
    int montoTotal = 0;

    for (int i = 0; i < cantidadPedidos; i++) {
        if (obtenerAnioDeFecha(arregloPedidos[i].fecha) == anio) {
            montoTotal += arregloPedidos[i].total;
        }
    }

    return montoTotal;
}


//Funcion que retorna un array de los anios que existen en los pedidos
int* obtenerAniosPedidos(Pedido* arregloPedidos, int cantidadPedidos) {
    int* anios = NULL;
    int* cantidadAnios = malloc(sizeof(int));
    *cantidadAnios = 0;

    for (int i = 0; i < cantidadPedidos; i++) {
        int anio = obtenerAnioDeFecha(arregloPedidos[i].fecha);
        bool existe = false;
        for (int j = 0; j < *cantidadAnios; j++) {
            if (anios[j] == anio) {
                existe = true;
                break;
            }
        }
        if (!existe) {
            anios = realloc(anios, sizeof(int) * (*cantidadAnios + 1));
            if (!anios) {
                printf("Error al asignar memoria para los años.\n");
                *cantidadAnios = 0;
                return NULL;
            }
            anios[*cantidadAnios] = anio;
            (*cantidadAnios)++;
        }
    }

    return anios;
}


bool libroAsociadoAPedido(const char* codigo, const Pedido* pedidos, int cantidadPedidos) {
    if (!codigo || !pedidos || cantidadPedidos <= 0) return false;
    for (int p = 0; p < cantidadPedidos; p++) {
        const Pedido* ped = &pedidos[p];
        for (int i = 0; i < ped->cantidadLibros; i++) {
            if (ped->libros[i].codigo && strcmp(ped->libros[i].codigo, codigo) == 0) {
                return true;
            }
        }
    }
    return false;
}


// Formato especial para consola, bueno visualmente
static void print_sep(int w1,int w2,int w3,int w4,int w5){
    printf("+-%.*s-+-%.*s-+-%.*s-+-%.*s-+-%.*s-+\n",
        w1, "----------------------------------------",
        w2, "----------------------------------------",
        w3, "----------------------------------------",
        w4, "----------------------------------------",
        w5, "----------------------------------------");
}
// Imprime las filas 
static void print_row(const char* c1,const char* c2,const char* c3,const char* c4,const char* c5,
                      int w1,int w2,int w3,int w4,int w5){
    printf("| %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s |\n",
        w1,w1,c1?c1:"", w2,w2,c2?c2:"",
        w3,w3,c3?c3:"", w4,w4,c4?c4:"",
        w5,w5,c5?c5:"");
}
// Imprime las variables
void imprimirTablaPedidos(const Pedido* pedidos, int cantidadPedidos){
    const int W1=10, W2=18, W3=12, W4=12, W5=12;
    print_sep(W1,W2,W3,W4,W5);
    print_row("Indice","ID","Fecha","Subtotal","Total", W1,W2,W3,W4,W5);
    print_sep(W1,W2,W3,W4,W5);

    for(int i=0;i<cantidadPedidos;i++){
        char bufIdx[16]; snprintf(bufIdx,sizeof bufIdx,"%d",i);
        char bufSub[32]; snprintf(bufSub,sizeof bufSub,"%.2f", pedidos[i].subtotal);
        char bufTot[32]; snprintf(bufTot,sizeof bufTot,"%.2f", pedidos[i].total);
        print_row(bufIdx, pedidos[i].id, pedidos[i].fecha, bufSub, bufTot, W1,W2,W3,W4,W5);
    }
    print_sep(W1,W2,W3,W4,W5);
    puts("Puedes seleccionar por 'indice' (número) o por 'id' (por ejemplo L002).");
}

// Buscar el pedido por el id, aux
const Pedido* buscarPedidoPorId(const Pedido* pedidos, int cantidadPedidos, const char* id){
    if(!pedidos || !id) return NULL;
    for(int i=0;i<cantidadPedidos;i++){
        if(pedidos[i].id && strcmp(pedidos[i].id,id)==0) return &pedidos[i];
    }
    return NULL;
}

// Se extraen los pedidos
int seleccionarPedidoPorIndiceOId(const Pedido* pedidos, int cantidadPedidos){
    // Devuelve indice (>=0) o -1 si cancela
    char entrada[64];
    printf("Selecciona un pedido (indice o id). Enter para cancelar: ");
    if(!fgets(entrada,sizeof entrada, stdin)) return -1;
    // trim 
    size_t n=strlen(entrada); if(n && (entrada[n-1]=='\n'||entrada[n-1]=='\r')) entrada[n-1]='\0';
    if(entrada[0]=='\0') return -1;

    // ¿es numero?
    char *end=NULL;
    long asNum = strtol(entrada,&end,10);
    if(end && *end=='\0'){ // entero válido
        if(asNum>=0 && asNum<cantidadPedidos) return (int)asNum;
        printf("Indice fuera de rango.\n");
        return -1;
    }

    // si no es numero, se trata como un id
    const Pedido* p = buscarPedidoPorId(pedidos,cantidadPedidos,entrada);
    if(!p){ printf("No se encontró un pedido con id '%s'.\n", entrada); return -1; }

    // convertir puntero a indice
    int idx = (int)(p - pedidos);
    return idx;
}

void listarPedidosCLI(Pedido* pedidos, int cantidadPedidos, Config cfg){
    if(!pedidos || cantidadPedidos<=0){
        puts("No hay pedidos para mostrar.");
        return;
    }

    puts("\n=== LISTA DE PEDIDOS ===");
    imprimirTablaPedidos(pedidos, cantidadPedidos);

    int idx = seleccionarPedidoPorIndiceOId(pedidos, cantidadPedidos);
    if(idx<0){
        puts("Operación cancelada.");
        return;
    }

    // Mostrar encabezado extendido (cliente, fecha, montos)
    const Pedido* sel = &pedidos[idx];
    Cliente* cli = obtenerClientePorCedula(sel->cedula);
    printf("\n--- Encabezado del Pedido ---\n");
    printf("ID: %s\n",   sel->id);
    printf("Cliente: %s (%s)\n", cli && cli->nombre ? cli->nombre : "(sin nombre)",
                                 sel->cedula ? sel->cedula : "");
    printf("Fecha: %s\n", sel->fecha ? sel->fecha : "");
    printf("Subtotal: %.2f\nImpuesto: %.2f\nTotal: %.2f\n",
            sel->subtotal, sel->impuesto, sel->total);

    // Detalle 
    printf("\n--- Detalle del Pedido ---\n");
    mostrarDetallePedido((Pedido*)sel, cfg);

    
    for(int i=0;i<sel->cantidadLibros;i++){
        const Libro* L = &sel->libros[i];
        int qty = sel->cantidadPorLibro ? sel->cantidadPorLibro[i] : 0;
        float sub = (L->precio) * (float)qty;
        printf("%2d) %s | %s | %.2f x %d = %.2f\n",
    
            i+1, L->codigo?L->codigo:"", L->titulo?L->titulo:"",
               L->precio, qty, sub);
    }
    
}

int pedidos_count(void) { return cantidadPedidosActual; }
Pedido* pedidos_data(void) { return arregloPedidos; }
void pedidos_set(Pedido* arr, int n) {
    arregloPedidos = arr;
    cantidadPedidosActual = n;
}
