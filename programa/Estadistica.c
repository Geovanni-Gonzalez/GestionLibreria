#include "Estadistica.h"
#include "Cliente.h"
#include "Pedido.h"
#include "Utilidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void inicializarEstadistica(Estadistica* estadistica) {
    //1. Asigna la cantidad de pedidos totales con una función que retorne la dirección de la variable cantidadPedidosActual en Pedido.c
    estadistica->totalPedidos = *cantidadPedidos(); 
    //2. Asigna el monto total de ventas con una función que calcule el monto total de ventas en base al arreglo de pedidos.
    estadistica->montoTotalDeVentas = calcularMontoTotalDeVentas(estadistica);
    //3. Inicializa el arreglo pedidosPorAnio con memoria dinámica

    //4. Inicializa el arreglo cantidadPedidosPorCliente con memoria dinámica

    //5. Inicializa el arreglo cantidadVendidaLibro con memoria dinámica
    
    //6. Asigna la cantidad de libros disponibles en el sistema 

}


float calcularMontoTotalDeVentas(Estadistica* estadistica) {
    float montoTotal = 0.0f;
    for (int i = 0; i < cantidadPedidos; i++) {
        montoTotal += arregloPedidos[i].total;
    }
    return montoTotal;  
}

void mostrarMontoPorAnios(Estadistica* estadistica) {
    //1. Obtener los años únicos de los pedidos usando la función obtenerAniosPedidos
    int* anios = obtenerAniosPedidos(arregloPedidos, cantidadPedidosActual);

    //2. Imprimir total de pedidos
    printf("Total de pedidos: %d\n", estadistica->totalPedidos);

    //3. Mostrar el monto total de ventas por año
    for (int i = 0; anios[i] != -1; i++) {
        printf("--------------------------\n");
        int anio = anios[i];
        float monto = obtenerMontoPorAnio(anio, arregloPedidos, cantidadPedidosActual);
        printf("Año %d: %.2f\n", anio, monto);
        printf("--------------------------\n");
    }

    //4. Liberar la memoria del arreglo de años
    free(anios);
}

void mostrarLibrosMasVendidos(Pedido* arregloPedidos, int cantidadPedidos) {
    int* cantidadesVendidas = NULL;
    char** codigosLibros = NULL;
    int totalLibros = 0;

    // 1 y 2: recorrer pedidos y sus libros
    for (int i = 0; i < cantidadPedidos; i++) {
        Pedido pedido = arregloPedidos[i];
        for (int j = 0; j < pedido.cantidadLibros; j++) {
            char* codigo = pedido.libros[j].codigo;
            int cantidad = pedido.cantidadPorLibro[j];

            // 3: verificar si ya existe
            int indice = buscarIndiceLibro(codigosLibros, totalLibros, codigo);
            if (indice != -1) {
                cantidadesVendidas[indice] += cantidad;
            } else {
                agregarLibroEstadistica(&codigosLibros, &cantidadesVendidas, &totalLibros, codigo, cantidad);
            }
        }
    }

    // 4. Ordenar por cantidad vendida descendente
    ordenarLibrosVendidos(codigosLibros, cantidadesVendidas, totalLibros);

    // 5. Mostrar resultados
    mostrarResultados(codigosLibros, cantidadesVendidas, totalLibros);

    // liberar memoria
    for (int i = 0; i < totalLibros; i++) {
        free(codigosLibros[i]);
    }
    free(codigosLibros);
    free(cantidadesVendidas);
}
void mostrarTotalPedidos(Estadistica* estadistica) {
    // Asegurar que el total refleje el estado actual
    if (estadistica) {
        estadistica->totalPedidos = cantidadPedidosActual; // sincronia básica
    }

    printf("Total pedidos: %d\n", cantidadPedidosActual);

    if (cantidadPedidosActual == 0) {
        printf("No hay pedidos registrados.\n");
        return;
    }

    for (int i = 0; i < cantidadPedidosActual; i++) {
        Pedido *p = &arregloPedidos[i];
        printf("Pedido #%d\n", i + 1);
        printf("ID: %s\n", p->id);
        printf("Fecha: %s\n", p->fecha);
        printf("Total: %.2f\n", p->total);
        printf("-----------------------------\n");
    }
}

// Imprime los pedidos agrupados por mes y año en el formato requerido
void mostrarPedidosAniosMes(Estadistica* estadistica) {
    (void)estadistica; // no estrictamente necesario aquí, pero mantenemos firma similar al resto
    if (cantidadPedidosActual == 0) {
        printf("Pedidos por mes y año\n");
        printf("No hay pedidos registrados.\n");
        return;
    }

    // Nombres de los meses en español (1..12)
    const char* meses[13] = {
        "", "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };

    printf("Pedidos por mes y año\n");

    // Aqui se recorren los años únicos, y para cada año, recorrer meses 1 al 12
    int* anios = obtenerAniosPedidos(arregloPedidos, cantidadPedidosActual);
    if (!anios) return;

    int cantidadAnios = 0;
    for (int i = 0; i < cantidadPedidosActual; i++) {
        int ai = obtenerAnioDeFecha(arregloPedidos[i].fecha);
        for (int j = 0; ; j++) {
            if (j == cantidadAnios) { cantidadAnios++; break; }
            if (anios[j] == ai) break;
        }
    }

    // Para cada año, imprimimos por mes sólo si hay pedidos en ese mes
    for (int i = 0; i < cantidadAnios; i++) {
        int anio = anios[i];
        for (int mes = 1; mes <= 12; mes++) {
            // Verificar si hay al menos un pedido en (anio, mes)
            int hay = 0;
            for (int k = 0; k < cantidadPedidosActual; k++) {
                if (obtenerAnioDeFecha(arregloPedidos[k].fecha) == anio &&
                    obtenerMesDeFecha(arregloPedidos[k].fecha) == mes) {
                    hay = 1; break;
                }
            }
            if (!hay) continue;

            // Encabezado del grupo
            printf("%s %d\n", meses[mes], anio);
            printf("ID, Total\n");

            // Listado de pedidos del grupo
            for (int k = 0; k < cantidadPedidosActual; k++) {
                if (obtenerAnioDeFecha(arregloPedidos[k].fecha) == anio &&
                    obtenerMesDeFecha(arregloPedidos[k].fecha) == mes) {
                    printf("%s, %.2f\n", arregloPedidos[k].id, arregloPedidos[k].total);
                }
            }

            printf("\n"); // línea en blanco entre grupos
        }
    }

    free(anios);
}
// Struct auxiliar solo para este archivo
typedef struct {
    Cliente* cliente;
    int      cantidadPedidos;
} ClienteConteo;
static int cmp_conteo_desc(const void* a, const void* b) {
    const ClienteConteo* ca = a;
    const ClienteConteo* cb = b;
    return cb->cantidadPedidos - ca->cantidadPedidos;
}

void mostrarClientesConMasPedidos(void) {
    // Obtener vistas a clientes y pedidos
    int nClientes = clientes_count();
    Cliente* clientes = clientes_data();

    int nPedidos = pedidos_count();
    Pedido* pedidos = pedidos_data();

    if (nClientes <= 0 || !clientes) {
        puts("No hay clientes registrados.");
        return;
    }

    // Construir lista de conteos
    ClienteConteo* lista = (ClienteConteo*)calloc((size_t)nClientes, sizeof(ClienteConteo));
    if (!lista) { perror("calloc"); return; }

    for (int i = 0; i < nClientes; i++) {
        int cont = 0;
        // Contar pedidos que tengan la misma cédula
        for (int j = 0; j < nPedidos; j++) {
            if (strcmp(pedidos[j].cedula, clientes[i].cedula) == 0) {
                cont++;
            }
        }
        lista[i].cliente = &clientes[i];
        lista[i].cantidadPedidos = cont;
    }

    // Ordenar de mayor a menor
    qsort(lista, (size_t)nClientes, sizeof(ClienteConteo), cmp_conteo_desc);

    // Imprimir
    printf("\n=== Clientes con más pedidos ===\n");
    printf("%-20s %-12s %s\n", "Nombre", "Cédula", "Pedidos");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < nClientes; i++) {
        printf("%-20s %-12s %d\n",
               lista[i].cliente->nombre,
               lista[i].cliente->cedula,
               lista[i].cantidadPedidos);
    }

    free(lista);
}

typedef struct {
    char* autor;
    float monto;
} AutorMonto;

/* Extrae el año desde una fecha en formato DDMMYYYY (8 caracteres) */
static int extraer_anio_ddmmyyyy(const char* fecha) {
    if (!fecha || strlen(fecha) < 8) return -1;
    return (fecha[4]-'0')*1000 + (fecha[5]-'0')*100
         + (fecha[6]-'0')*10   + (fecha[7]-'0');
}

static void acumular(AutorMonto** arr, int* n, int* cap,
                     const char* autor, float monto) {
    for (int i = 0; i < *n; i++) {
        if (strcmp((*arr)[i].autor, autor) == 0) {
            (*arr)[i].monto += monto;
            return;
        }
    }
    if (*n == *cap) {
        int nueva = *cap ? *cap * 2 : 4;
        AutorMonto* tmp = realloc(*arr, nueva * sizeof(AutorMonto));
        if (!tmp) { perror("realloc"); return; }
        *arr = tmp; *cap = nueva;
    }
    (*arr)[*n].autor = strdup(autor ? autor : "");
    (*arr)[*n].monto = monto;
    (*n)++;
}

static void liberar(AutorMonto* arr, int n) {
    for (int i = 0; i < n; i++) free(arr[i].autor);
    free(arr);
}

void mostrarAutorMasVentasPorAnio(void) {
    Pedido* pedidos = pedidos_data();
    int nPedidos    = pedidos_count();
    if (!pedidos || nPedidos == 0) {
        puts("No hay pedidos registrados.");
        return;
    }

    puts("\n=== Autor con más ventas por año ===");

    /* Reunir años */
    int* anios = NULL;
    int nA = 0, capA = 0;
    for (int i = 0; i < nPedidos; i++) {
        int anio = extraer_anio_ddmmyyyy(pedidos[i].fecha);
        if (anio < 0) continue;
        int existe = 0;
        for (int k = 0; k < nA; k++)
            if (anios[k] == anio) { existe = 1; break; }
        if (!existe) {
            if (nA == capA) {
                int nueva = capA ? capA * 2 : 4;
                int* tmp = realloc(anios, nueva * sizeof(int));
                if (!tmp) { perror("realloc"); free(anios); return; }
                anios = tmp; capA = nueva;
            }
            anios[nA++] = anio;
        }
    }

    /* Calcular por cada año */
    for (int a = 0; a < nA; a++) {
        int anioObjetivo = anios[a];
        AutorMonto* lista = NULL;
        int nLista = 0, capLista = 0;

        for (int i = 0; i < nPedidos; i++) {
            if (extraer_anio_ddmmyyyy(pedidos[i].fecha) != anioObjetivo) continue;
            for (int j = 0; j < pedidos[i].cantidadLibros; j++) {
                const char* autor = pedidos[i].libros[j].autor;
                float precio = pedidos[i].libros[j].precio;
                int cantidad = pedidos[i].cantidadPorLibro ?
                               pedidos[i].cantidadPorLibro[j] : 0;
                acumular(&lista, &nLista, &capLista,
                         autor ? autor : "(sin autor)",
                         precio * cantidad);
            }
        }

        if (nLista == 0) {
            printf("Año %d: sin ventas\n", anioObjetivo);
            continue;
        }

        int idxMax = 0;
        for (int i = 1; i < nLista; i++)
            if (lista[i].monto > lista[idxMax].monto) idxMax = i;

        printf("Año %d: %s (Monto: %.2f)\n",
               anioObjetivo,
               lista[idxMax].autor,
               lista[idxMax].monto);

        liberar(lista, nLista);
    }
    free(anios);
}