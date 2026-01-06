#include "Estadistica.h"
#include "Cliente.h"
#include "Pedido.h"
#include "Utilidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inicializarEstadistica(Estadistica* estadistica) {
    estadistica->totalPedidos = *cantidadPedidos(); 
    estadistica->montoTotalDeVentas = calcularMontoTotalDeVentas(estadistica);
}

float calcularMontoTotalDeVentas(Estadistica* estadistica) {
    float montoTotal = 0.0f;
    if (!arregloPedidos || cantidadPedidosActual <= 0) {
        return montoTotal;
    }
    for (int i = 0; i < cantidadPedidosActual; i++) {
        montoTotal += arregloPedidos[i].total;
    }
    return montoTotal;
}

void mostrarMontoPorAnios(Estadistica* estadistica) {
    int* anios = obtenerAniosPedidos(arregloPedidos, cantidadPedidosActual);
    printf("Total de pedidos: %d\n", estadistica->totalPedidos);
    for (int i = 0; anios[i] != -1; i++) {
        printf("--------------------------\n");
        int anio = anios[i];
        float monto = obtenerMontoPorAnio(anio, arregloPedidos, cantidadPedidosActual);
        printf("Año %d: %.2f\n", anio, monto);
        printf("--------------------------\n");
    }
    free(anios);
}

void mostrarLibrosMasVendidos(Pedido* arregloPedidos, int cantidadPedidos) {
    int* cantidadesVendidas = NULL;
    char** codigosLibros = NULL;
    int totalLibros = 0;

    for (int i = 0; i < cantidadPedidos; i++) {
        Pedido pedido = arregloPedidos[i];
        for (int j = 0; j < pedido.cantidadLibros; j++) {
            char* codigo = pedido.libros[j].codigo;
            int cantidad = pedido.cantidadPorLibro[j];

            int indice = buscarIndiceLibro(codigosLibros, totalLibros, codigo);
            if (indice != -1) {
                cantidadesVendidas[indice] += cantidad;
            } else {
                agregarLibroEstadistica(&codigosLibros, &cantidadesVendidas, &totalLibros, codigo, cantidad);
            }
        }
    }

    ordenarLibrosVendidos(codigosLibros, cantidadesVendidas, totalLibros);
    mostrarResultados(codigosLibros, cantidadesVendidas, totalLibros);

    for (int i = 0; i < totalLibros; i++) {
        free(codigosLibros[i]);
    }
    free(codigosLibros);
    free(cantidadesVendidas);
}

void mostrarTotalPedidos(Estadistica* estadistica) {
    if (estadistica) {
        estadistica->totalPedidos = cantidadPedidosActual;
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

void mostrarPedidosAniosMes(Estadistica* estadistica) {
    (void)estadistica;
    if (cantidadPedidosActual == 0) {
        printf("Pedidos por mes y año\n");
        printf("No hay pedidos registrados.\n");
        return;
    }
    const char* meses[13] = {
        "", "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };
    printf("Pedidos por mes y año\n");
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
    for (int i = 0; i < cantidadAnios; i++) {
        int anio = anios[i];
        for (int mes = 1; mes <= 12; mes++) {
            int hay = 0;
            for (int k = 0; k < cantidadPedidosActual; k++) {
                if (obtenerAnioDeFecha(arregloPedidos[k].fecha) == anio &&
                    obtenerMesDeFecha(arregloPedidos[k].fecha) == mes) {
                    hay = 1; break;
                }
            }
            if (!hay) continue;
            printf("%s %d\n", meses[mes], anio);
            printf("ID, Total\n");
            for (int k = 0; k < cantidadPedidosActual; k++) {
                if (obtenerAnioDeFecha(arregloPedidos[k].fecha) == anio &&
                    obtenerMesDeFecha(arregloPedidos[k].fecha) == mes) {
                    printf("%s, %.2f\n", arregloPedidos[k].id, arregloPedidos[k].total);
                }
            }
            printf("\n");
        }
    }
    free(anios);
}

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
    int nClientes = clientes_count();
    Cliente* clientes = clientes_data();
    int nPedidos = pedidos_count();
    Pedido* pedidos = pedidos_data();
    if (nClientes <= 0 || !clientes) {
        puts("No hay clientes registrados.");
        return;
    }
    ClienteConteo* lista = (ClienteConteo*)calloc((size_t)nClientes, sizeof(ClienteConteo));
    if (!lista) { perror("calloc"); return; }
    for (int i = 0; i < nClientes; i++) {
        int cont = 0;
        for (int j = 0; j < nPedidos; j++) {
            if (strcmp(pedidos[j].cedula, clientes[i].cedula) == 0) {
                cont++;
            }
        }
        lista[i].cliente = &clientes[i];
        lista[i].cantidadPedidos = cont;
    }
    qsort(lista, (size_t)nClientes, sizeof(ClienteConteo), cmp_conteo_desc);
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

void mostrarMejorClientePorMonto(void) {
    int nClientes = clientes_count();
    Cliente* clientes = clientes_data();
    int nPedidos = pedidos_count();
    Pedido* pedidos = pedidos_data();
    if (nClientes <= 0 || !clientes) {
        puts("No hay clientes registrados.");
        return;
    }
    float maxMonto = -1.0f;
    int idxMejor = -1;
    for (int i = 0; i < nClientes; i++) {
        float montoActual = 0.0f;
        for (int j = 0; j < nPedidos; j++) {
            if (strcmp(pedidos[j].cedula, clientes[i].cedula) == 0) {
                montoActual += pedidos[j].total;
            }
        }
        if (montoActual > maxMonto) {
            maxMonto = montoActual;
            idxMejor = i;
        }
    }
    if (idxMejor != -1) {
        printf("\n--- Cliente con mayor inversión ---\n");
        printf("Nombre: %s\n", clientes[idxMejor].nombre);
        printf("Cédula: %s\n", clientes[idxMejor].cedula);
        printf("Monto Total: %.2f\n", maxMonto);
        printf("------------------------------------\n");
    }
}

void mostrarLibroMasRentable(void) {
    int nPedidos = pedidos_count();
    Pedido* pedidos = pedidos_data();
    if (nPedidos <= 0) {
        puts("No hay pedidos para analizar rentabilidad.");
        return;
    }
    typedef struct {
        char codigo[64];
        float totalIngresos;
    } LibroRentabilidad;
    LibroRentabilidad* lista = NULL;
    int nL = 0, cap = 0;
    for (int i = 0; i < nPedidos; i++) {
        for (int j = 0; j < pedidos[i].cantidadLibros; j++) {
            char* cod = pedidos[i].libros[j].codigo;
            float ingreso = pedidos[i].libros[j].precio * (pedidos[i].cantidadPorLibro ? pedidos[i].cantidadPorLibro[j] : 0);
            int hallado = 0;
            for (int k = 0; k < nL; k++) {
                if (strcmp(lista[k].codigo, cod) == 0) {
                    lista[k].totalIngresos += ingreso;
                    hallado = 1;
                    break;
                }
            }
            if (!hallado) {
                if (nL == cap) {
                    cap = cap ? cap * 2 : 4;
                    lista = realloc(lista, cap * sizeof(LibroRentabilidad));
                }
                strncpy(lista[nL].codigo, cod, 63);
                lista[nL].totalIngresos = ingreso;
                nL++;
            }
        }
    }
    if (nL > 0) {
        int idxMax = 0;
        for (int i = 1; i < nL; i++) {
            if (lista[i].totalIngresos > lista[idxMax].totalIngresos) idxMax = i;
        }
        printf("\n--- Libro más rentable (Mayores Ingresos) ---\n");
        printf("Código: %s\n", lista[idxMax].codigo);
        printf("Ingresos Totales: %.2f\n", lista[idxMax].totalIngresos);
        printf("---------------------------------------------\n");
    }
    free(lista);
}