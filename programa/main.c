#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Config.h"
#include "Estadistica.h"
#include "Utilidades.h"
#include "Interfaz.h"

// Función para imprimir el menú principal
void mostrarMenuPrincipal() {
    imprimirEncabezado("SISTEMA DE GESTIÓN DE LIBRERÍA", ANSI_COLOR_CYAN);
    printf(ANSI_BOLD "1." ANSI_COLOR_RESET " Opciones Administrativas " ANSI_COLOR_YELLOW "(Seguro)" ANSI_COLOR_RESET "\n");
    printf(ANSI_BOLD "2." ANSI_COLOR_RESET " Opciones Generales\n");
    printf(ANSI_BOLD "3." ANSI_COLOR_RESET " Salir\n");
    printf(ANSI_COLOR_CYAN "----------------------------------------------------------" ANSI_COLOR_RESET "\n");
    printf("Seleccione una opción: ");
}

// Función para imprimir el submenú administrativo
void mostrarMenuAdministrativo() {
    imprimirEncabezado("MENÚ ADMINISTRATIVO", ANSI_COLOR_GREEN);
    printf("1. Registrar libros\n");
    printf("2. Cargar inventario del archivo\n");
    printf("3. Registrar clientes\n");
    printf("4. Crear pedido nuevo\n");
    printf("5. Estadísticas de negocio\n");
    printf("6. Eliminar Libro\n");
    printf("7. Eliminar Cliente\n");
    printf("8. Eliminar Pedido\n");
    printf("9. Modificar Pedido\n");
    printf(ANSI_COLOR_RED "10. Volver al inicio" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "----------------------------------------------------------" ANSI_COLOR_RESET "\n");
    printf("Seleccione una opción: ");
}

// Función para imprimir el submenú general
void mostrarMenuGeneral() {
    imprimirEncabezado("MENÚ GENERAL", ANSI_COLOR_BLUE);
    printf("1. Consulta de catálogo completo\n");
    printf("2. Consulta de información de cliente\n");
    printf(ANSI_COLOR_RED "3. Volver al menú principal" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_BLUE "----------------------------------------------------------" ANSI_COLOR_RESET "\n");
    printf("Seleccione una opción: ");
}

// Función para imprimir un mensaje de bienvenida con la información inicial de la librería
void mostrarInfoLibreria(Config cfg) {
    imprimirEncabezado("INFORMACIÓN DE LA LIBRERÍA", ANSI_COLOR_YELLOW);
    printf(ANSI_BOLD "Nombre: " ANSI_COLOR_RESET "%s\n", cfg.nombreLocalComercial);
    printf(ANSI_BOLD "Teléfono: " ANSI_COLOR_RESET "%s\n", cfg.telefono);
    printf(ANSI_BOLD "Cédula jurídica: " ANSI_COLOR_RESET "%s\n", cfg.cedulaJuridica);
    printf(ANSI_BOLD "Horario: " ANSI_COLOR_RESET "%s\n", cfg.horarioAtencion);
    printf(ANSI_COLOR_YELLOW "==========================================================" ANSI_COLOR_RESET "\n");
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    int totalLibros = 0;
    Config configuracion;
    cargarConfig("data/admin.json", &configuracion);
    Libro* inventario = cargarLibros("data/libros.txt", &totalLibros);

    Estadistica estadistica;
    inicializarArregloPedidos();
    inicializarArregloClientes();
    inicializarEstadistica(&estadistica);
    
    limpiarPantalla();
    mostrarBanner();
    mostrarInfoLibreria(configuracion);
    presionarEnterParaContinuar();

    int opcionPrincipal;
    do {
        limpiarPantalla();
        mostrarBanner();
        mostrarMenuPrincipal();
        if (scanf("%d", &opcionPrincipal) != 1) {
            limpiar_stdin();
            continue;
        }
        limpiar_stdin();

        if (opcionPrincipal == 1) {
            char usuario[50], password[50];
            imprimirEncabezado("AUTENTICACIÓN REQUERIDA", ANSI_COLOR_MAGENTA);
            printf("Ingrese usuario: ");
            fgets(usuario, sizeof(usuario), stdin);
            limpiarFinLinea(usuario);
            printf("Ingrese contraseña: ");
            fgets(password, sizeof(password), stdin);
            limpiarFinLinea(password);

            if (strcmp(usuario, configuracion.admin.usuario) == 0 &&
                strcmp(password, configuracion.admin.password) == 0) {
                int opcionAdmin;
                do {
                    limpiarPantalla();
                    mostrarMenuAdministrativo();
                    if (scanf("%d", &opcionAdmin) != 1) {
                        limpiar_stdin();
                        continue;
                    }
                    limpiar_stdin();

                    switch (opcionAdmin) {
                        case 1:
                            imprimirEncabezado("REGISTRO DE LIBRO", ANSI_COLOR_GREEN);
                            printf("Ingrese el título del libro: ");
                            char titulo[100];
                            fgets(titulo, sizeof(titulo), stdin);
                            limpiarFinLinea(titulo);

                            printf("Ingrese el autor del libro: ");
                            char autor[100];
                            fgets(autor, sizeof(autor), stdin);
                            limpiarFinLinea(autor);

                            printf("Ingrese el precio del libro: ");
                            float precio;
                            scanf("%f", &precio);
                            limpiar_stdin();

                            printf("Ingrese la cantidad inicial: ");
                            int cantidad;
                            scanf("%d", &cantidad);
                            limpiar_stdin();

                            agregarLibro(&inventario, &totalLibros, titulo, autor, precio, cantidad);
                            imprimirMensajeExito("Libro agregado al inventario.");
                            presionarEnterParaContinuar();
                            break;
                        case 2:
                            imprimirEncabezado("CARGA DE INVENTARIO", ANSI_COLOR_GREEN);
                            printf("Ingrese la ruta del archivo (.txt): ");
                            char rutaArchivo[200];
                            fgets(rutaArchivo, sizeof(rutaArchivo), stdin);
                            limpiarFinLinea(rutaArchivo);
                            cargarInventario(&inventario, &totalLibros, rutaArchivo);
                            imprimirMensajeExito("Inventario procesado.");
                            presionarEnterParaContinuar();
                            break;
                        case 3:
                            imprimirEncabezado("REGISTRO DE CLIENTE", ANSI_COLOR_GREEN);
                            printf("Ingrese la cédula: ");
                            char cedula[20];
                            fgets(cedula, sizeof(cedula), stdin);
                            limpiarFinLinea(cedula);
                            printf("Ingrese el nombre completo: ");
                            char nombre[100];
                            fgets(nombre, sizeof(nombre), stdin);
                            limpiarFinLinea(nombre);
                            printf("Ingrese el teléfono: ");
                            char telCl[20];
                            fgets(telCl, sizeof(telCl), stdin);
                            limpiarFinLinea(telCl);
                            registrarCliente(cedula, nombre, telCl);
                            imprimirMensajeExito("Cliente registrado correctamente.");
                            presionarEnterParaContinuar();
                            break;
                        case 4: {
                            imprimirEncabezado("CREACIÓN DE PEDIDO", ANSI_COLOR_GREEN);
                            Pedido* nuevoPedido = (Pedido*)malloc(sizeof(Pedido));
                            if (!nuevoPedido) {
                                imprimirMensajeError("Falla de memoria al crear pedido.");
                                break;
                            }
                            inicializarPedido(nuevoPedido); 
                            mostrarLibrosCatalogo(inventario, totalLibros);

                            char codigoLibro[20];
                            int cantidadLibro;
                            printf("\nIngrese el código del libro: ");
                            fgets(codigoLibro, sizeof(codigoLibro), stdin);
                            limpiarFinLinea(codigoLibro);

                            printf("Ingrese la cantidad: ");
                            if (scanf("%d", &cantidadLibro) != 1) {
                                imprimirMensajeError("Cantidad inválida.");
                                limpiar_stdin();
                                free(nuevoPedido);
                                presionarEnterParaContinuar();
                                break;
                            }
                            limpiar_stdin();

                            seleccionarLibro(nuevoPedido, codigoLibro, cantidadLibro, inventario, totalLibros, configuracion);
                            imprimirMensajeExito("Pedido procesado.");
                            presionarEnterParaContinuar();
                        } break;
                        case 5:
                            imprimirEncabezado("ESTADÍSTICAS Y REPORTES", ANSI_COLOR_BLUE);
                            mostrarPedidosAniosMes(&estadistica);
                            mostrarAutorMasVentasPorAnio();
                            mostrarMejorClientePorMonto();
                            mostrarLibroMasRentable();
                            presionarEnterParaContinuar();
                            break;
                        case 6: {
                            imprimirEncabezado("ELIMINAR LIBRO", ANSI_COLOR_RED);
                            char codEli[64];
                            printf("Ingrese el código a eliminar: ");
                            fgets(codEli, sizeof(codEli), stdin);
                            limpiarFinLinea(codEli);
                            if (codEli[0] != '\0') {
                                eliminarLibro(&inventario, &totalLibros, codEli);
                                guardarLibros("data/libros.txt", inventario, totalLibros);
                                imprimirMensajeExito("Libro eliminado.");
                            }
                            presionarEnterParaContinuar();
                            break;
                        }
                        case 7: {
                            imprimirEncabezado("ELIMINAR CLIENTE", ANSI_COLOR_RED);
                            char cedEli[16];
                            printf("Ingrese la cédula a eliminar: ");
                            fgets(cedEli, sizeof(cedEli), stdin);
                            limpiarFinLinea(cedEli);
                            if (cedEli[0] != '\0') {
                                if (eliminarCliente(cedEli)) {
                                    imprimirMensajeExito("Cliente removido.");
                                }
                            }
                            presionarEnterParaContinuar();
                            break;
                        }
                        case 8: {
                            imprimirEncabezado("ELIMINAR PEDIDO", ANSI_COLOR_RED);
                            char idEli[16];
                            printf("Ingrese el ID del pedido (ej. P000001): ");
                            leerLineaSeguro(idEli, sizeof(idEli));
                            if (idEli[0] != '\0') {
                                if (eliminarPedido(idEli, inventario, totalLibros)) {
                                    imprimirMensajeExito("Pedido eliminado y stock revertido.");
                                }
                            }
                            presionarEnterParaContinuar();
                            break;
                        }
                        case 9: {
                            imprimirEncabezado("MODIFICAR PEDIDO", ANSI_COLOR_MAGENTA);
                            char idMod[16];
                            printf("Ingrese el ID del pedido (ej. P000001): ");
                            leerLineaSeguro(idMod, sizeof(idMod));
                            if (idMod[0] != '\0') {
                                modificarPedido(idMod, inventario, totalLibros, configuracion);
                            }
                            presionarEnterParaContinuar();
                            break;
                        }
                        case 10:
                            break;
                        default:
                            imprimirMensajeError("Opción inválida.");
                            presionarEnterParaContinuar();
                    }
                } while (opcionAdmin != 10);
            } else {
                imprimirMensajeError("Credenciales inválidas.");
                presionarEnterParaContinuar();
            }
        } else if (opcionPrincipal == 2) {
            int opcionGeneral;
            do {
                limpiarPantalla();
                mostrarMenuGeneral();
                if (scanf("%d", &opcionGeneral) != 1) {
                    limpiar_stdin();
                    continue;
                }
                limpiar_stdin();

                switch (opcionGeneral) {
                    case 1:
                        imprimirEncabezado("CONSULTA DE CATÁLOGO", ANSI_COLOR_BLUE);
                        char autorFiltro[100];
                        printf("Filtrar por autor (deje vacío para todo): ");
                        leerLineaSeguro(autorFiltro, sizeof(autorFiltro));
                        const char* filtro = (autorFiltro[0] == '\0') ? NULL : autorFiltro;
                        consultaCatalogo("data/libros.txt", filtro);
                        presionarEnterParaContinuar();
                        break;
                    case 2:
                        imprimirEncabezado("CONSULTA DE CLIENTE", ANSI_COLOR_BLUE);
                        char cedCon[16];
                        printf("Ingrese la cédula del cliente: ");
                        leerLineaSeguro(cedCon, sizeof(cedCon));
                        if (cedCon[0] == '\0') {
                            imprimirMensajeError("Cédula vacía.");
                        } else {
                            consultaDeCliente(cedCon);
                        }
                        presionarEnterParaContinuar();
                        break;
                    case 3:
                        break;
                    default:
                        imprimirMensajeError("Opción inválida.");
                        presionarEnterParaContinuar();
                }
            } while (opcionGeneral != 3);
        }
    } while (opcionPrincipal != 3);

    imprimirMensajeInfo("Guardando sesión y configuración...");
    guardarConfig("data/admin.json", &configuracion);
    imprimirMensajeExito("Sistema cerrado correctamente.");

    return 0;
}