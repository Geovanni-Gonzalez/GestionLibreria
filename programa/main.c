#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Cliente.h"
#include "Config.h"
#include "Estadistica.h"
#include "Utilidades.h"


// Función para imprimir el menú principal
void mostrarMenuPrincipal() {
    printf("\n===== SISTEMA DE GESTIÓN DE LIBRERÍA =====\n");
    printf("1. Opciones Administrativas (requiere usuario y contraseña)\n");
    printf("2. Opciones Generales\n");
    printf("3. Salir\n");
    printf("Seleccione una opción: ");
}

// Función para imprimir el submenú administrativo
void mostrarMenuAdministrativo() {
    printf("\n===== MENÚ ADMINISTRATIVO =====\n");
    printf("1. Registrar libros\n");
    printf("2. Cargar inventario\n");
    printf("3. Registrar clientes\n");
    printf("4. Crear pedido\n");
    printf("5. Estadísticas\n");
    printf("6. Eliminar Libro\n");
    printf("7. Eliminar Cliente\n");
    printf("8. Volver al menú principal\n");
    printf("Seleccione una opción: ");
}

// Función para imprimir el submenú general
void mostrarMenuGeneral() {
    printf("\n===== MENÚ GENERAL =====\n");
    printf("1. Consulta de catálogo\n");
    printf("2. Consulta de cliente\n");
    printf("3. Volver al menú principal\n");
    printf("Seleccione una opción: ");
}

// Función para imprimir un mensaje de bienvenida con la información inicial de la librería
void mostrarInfoLibreria(Config cfg) {
    printf("========== BIENVENIDO A LA LIBRERÍA ==========\n");
    printf("Nombre de la librería: %s\n", cfg.nombreLocalComercial);
    printf("Teléfono: %s\n", cfg.telefono);
    printf("Cédula jurídica: %s\n", cfg.cedulaJuridica);
    printf("Horario de atención: %s\n", cfg.horarioAtencion);
    printf("==============================================\n");
}



int main(void) {
    // Variables principales
    int totalLibros = 0;

    // Cargar configuración inicial
    Config configuracion;
    cargarConfig("data/admin.json", &configuracion);

    // Cargar inventario de libros
    Libro* inventario = cargarLibros("data/libros.txt", &totalLibros);

    

    // Inicializar estructuras de datos
    Estadistica estadistica;
    inicializarArregloPedidos();
    inicializarArregloClientes();
    inicializarEstadistica(&estadistica);
    

    // Mostrar información inicial de la librería
    mostrarInfoLibreria(configuracion);



    int opcionPrincipal;
    do {
        mostrarMenuPrincipal();
        scanf("%d", &opcionPrincipal);
        limpiarStdin();

        if (opcionPrincipal == 1) {
            char usuario[50], password[50];
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
                    mostrarMenuAdministrativo();
                    scanf("%d", &opcionAdmin);
                    limpiar_stdin();

                    switch (opcionAdmin) {
                        case 1:
                            //Solicitar titulo, autor, precio y cantidad
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

                            printf("Ingrese la cantidad del libro: ");
                            int cantidad;
                            scanf("%d", &cantidad);
                            limpiar_stdin();

                            agregarLibro(&inventario, &totalLibros, titulo, autor, precio, cantidad);
                            break;
                        case 2:
                            //Solicita ruta del archivo
                            printf("Ingrese la ruta del archivo de inventario: ");
                            char rutaArchivo[200];
                            fgets(rutaArchivo, sizeof(rutaArchivo), stdin);
                            limpiarFinLinea(rutaArchivo);
                            cargarInventario(&inventario, &totalLibros, rutaArchivo);
                            break;
                        case 3:
                            //Solicitar datos del cliente
                            printf("Ingrese la cédula del cliente: ");
                            char cedula[20];
                            fgets(cedula, sizeof(cedula), stdin);
                            limpiarFinLinea(cedula);
                            printf("Ingrese el nombre del cliente: ");
                            char nombre[100];
                            fgets(nombre, sizeof(nombre), stdin);
                            limpiarFinLinea(nombre);
                            printf("Ingrese el teléfono del cliente: ");
                            char telefono[20];
                            fgets(telefono, sizeof(telefono), stdin);
                            limpiarFinLinea(telefono);
                            registrarCliente(cedula, nombre, telefono);
                            break;

                        case 4: {

                                // Crear y **inicializar** pedido
                                Pedido* nuevoPedido = (Pedido*)malloc(sizeof(Pedido));
                                if (!nuevoPedido) {
                                    printf("Error al crear el pedido.\n");
                                    break;
                                }
                                inicializarPedido(nuevoPedido); 

                                // Mostrar catálogo de libros
                                mostrarLibrosCatalogo(inventario, totalLibros);

                                // Ingreso de código y cantidad
                                char codigoLibro[20];
                                int cantidadLibro;

                                printf("Ingrese el código del libro a agregar al pedido: ");
                                fgets(codigoLibro, sizeof(codigoLibro), stdin);
                                limpiarFinLinea(codigoLibro);

                                printf("Ingrese la cantidad del libro a agregar al pedido: ");
                                if (scanf("%d", &cantidadLibro) != 1) {
                                    printf("Cantidad inválida.\n");
                                    limpiar_stdin();
                                    // liberarPedido(nuevoPedido);
                                    free(nuevoPedido);
                                    break;
                                }
                                limpiar_stdin();

                                // Agregar libro al pedido de forma segura
                                seleccionarLibro(nuevoPedido, codigoLibro, cantidadLibro, inventario, totalLibros, configuracion);


                            } break;


                        
                        case 5:
                            // Lógica para estadísticas
                            break;
                        case 6: {
                            // Eliminar libro por código y guardar cambios
                            char codigo[64];
                            printf("Ingrese el código del libro a eliminar: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            limpiarFinLinea(codigo);
                            if (codigo[0] == '\0') {
                                printf("Código vacío. Operación cancelada.\n");
                                break;
                            }
                            eliminarLibro(&inventario, &totalLibros, codigo);
                            // Persistir inventario actualizado
                            guardarLibros("data/libros.txt", inventario, totalLibros);
                            break;
                        }
                        case 7: {
                            // Eliminar cliente por cédula (verifica pedidos asociados dentro de la función)
                            char cedula[16];
                            printf("Ingrese la cédula del cliente a eliminar: ");
                            fgets(cedula, sizeof(cedula), stdin);
                            limpiarFinLinea(cedula);
                            if (cedula[0] == '\0') {
                                printf("Cédula vacía. Operación cancelada.\n");
                                break;
                            }
                            if (!eliminarCliente(cedula)) {
                                // Mensajes de error ya se imprimen dentro de eliminarCliente
                            }
                            break;
                        }
                        case 8:
                            printf("Volviendo al menú principal...\n");
                            break;
                        default:
                            printf("Opción inválida. Intente nuevamente.\n");
                    }
                } while (opcionAdmin != 8);
            } else {
                printf("Usuario o contraseña incorrectos.\n");
            }
        } else if (opcionPrincipal == 2) {
            int opcionGeneral;
            do {
                mostrarMenuGeneral();
                scanf("%d", &opcionGeneral);
                limpiar_stdin();

                switch (opcionGeneral) {
                    case 1:
                        // Consulta de catálogo con filtro opcional por autor
                        {
                            char autorFiltro[100];
                            const char* filtro;
                            printf("\nConsulta de catálogo\n");
                            printf("Filtrar por autor (opcional). Deje vacío para ver todos: ");
                            leerLineaSeguro(autorFiltro, sizeof(autorFiltro));

                            // Si el usuario deja vacío, pasamos NULL para mostrar todo
                            filtro = (autorFiltro[0] == '\0') ? NULL : autorFiltro;
                            consultaCatalogo("data/libros.txt", filtro);
                        }
                        break;
                    case 2:
                        // Consulta de cliente por cédula
                        {
                            char cedula[16];
                            printf("\nConsulta de cliente\n");
                            printf("Ingrese la cédula del cliente: ");
                            leerLineaSeguro(cedula, sizeof(cedula));
                            if (cedula[0] == '\0') {
                                printf("Cédula vacía. Operación cancelada.\n");
                            } else {
                                consultaDeCliente(cedula);
                            }
                        }
                        break;
                    case 3:
                        printf("Volviendo al menú principal...\n");
                        break;
                    default:
                        printf("Opción inválida. Intente nuevamente.\n");
                }
            } while (opcionGeneral != 3);
        } else if (opcionPrincipal == 3) {
            printf("Saliendo del sistema. ¡Hasta luego!\n");
        }
    } while (opcionPrincipal != 3);
    guardarConfig("data/admin.json", &configuracion);

    return 0;
}