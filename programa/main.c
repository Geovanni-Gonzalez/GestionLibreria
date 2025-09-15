// main.c
#include "Libro.h"
#include "Pedido.h"
#include "Config.h"
#include "Utilidades.h"
#include "Cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(void) {
    // 1) Cargar config
    Config cfg = (Config){0};
    if (!cargarConfig("admin.json", &cfg)) {
        printf("No se pudo cargar config.\n");
        return 1;
    }

    // 2) Cargar inventario
    int totalLibros = 0;
    Libro* inventario = cargarLibros("libros.txt", &totalLibros);
    if (!inventario || totalLibros <= 0) {
        printf("No hay inventario.\n");
        return 1;
    }


    // 2.5) CARGAR CLIENTES EN MEMORIA
    inicializarArregloClientes();
    extern void mostrarClientes(); 
    mostrarClientes();


    // 3) Preparar pedido en blanco
    Pedido pedido = {0};
    pedido.libros = NULL;
    pedido.cantidadPorLibro = NULL;
    pedido.cantidadLibros = 0;
    pedido.generado = false;

    // 4) Arreglo de pedidos
    Pedido pedidos[100] = {0};
    int cantidadPedidos = 0;

    // === LOGICA DE SELECCION POR CODIGO ===
    for (;;) {
        // Mostrar catalogo resumido
        printf("\n=== Catalogo (codigo → título, precio, stock) ===\n");
        for (int i = 0; i < totalLibros; i++) {
            printf("[%s] %s  $%.2f  (Stock: %d)\n",
                   inventario[i].codigo,
                   inventario[i].titulo ? inventario[i].titulo : "",
                   inventario[i].precio,
                   inventario[i].cantidad);
        }

        // Pedir codigo 
        char entrada[64];
        printf("Ingrese CODIGO (ENTER para salir): ");
        if (!fgets(entrada, sizeof(entrada), stdin)) break;

        // Quitar fin de linea
        entrada[strcspn(entrada, "\r\n")] = '\0';
        // Recortar espacios 
        char *start = entrada;
        while (*start && isspace((unsigned char)*start)) start++;
        char *end = start + strlen(start);
        while (end > start && isspace((unsigned char)*(end - 1))) end--;
        *end = '\0';

        if (start[0] == '\0') break; // usuario sale

        // Buscar indice por codigo
        int idxSel = -1;
        for (int i = 0; i < totalLibros; i++) {
            if (inventario[i].codigo && strcmp(inventario[i].codigo, start) == 0) {
                idxSel = i;
                break;
            }
        }
        if (idxSel < 0) {
            printf("Codigo no encontrado. Intente de nuevo.\n");
            continue;
        }

        // Llamar al menu usando el indice encontrado
        menuPedidoTrasSeleccion(&inventario, &totalLibros, idxSel,
                                &pedido, cfg, pedidos, &cantidadPedidos);
        // limpia posibles restos de entrada antes de pedir otro codigo
        limpiar_stdin();

        // Al salir del menu, el bucle vuelve a listar deja elegir otro codigo o salir.
    }
    

    // liberar inventario
    liberarLibros(inventario, totalLibros);
    return 0;
}
