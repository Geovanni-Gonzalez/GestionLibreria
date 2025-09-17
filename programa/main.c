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
    printf("2. Manejo de inventario\n");
    printf("3. Registrar clientes\n");
    printf("4. Crear pedido\n");
    printf("5. Estadísticas\n");
    printf("6. Volver al menú principal\n");
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

                            printf("Funcionalidad de registrar libros no implementada aún.\n");
                            break;
                        case 2:
                            // Lógica para manejo de inventario
                            printf("Funcionalidad de manejo de inventario no implementada aún.\n");
                            break;
                        case 3:
                            // Lógica para registrar clientes
                            printf("Funcionalidad de registrar clientes no implementada aún.\n");
                            break;
                        case 4:
                            // Lógica para crear pedido
                            printf("Funcionalidad de crear pedido no implementada aún.\n");
                            break;
                        case 5:
                            // Lógica para estadísticas
                            break;
                        case 6:
                            printf("Volviendo al menú principal...\n");
                            break;
                        default:
                            printf("Opción inválida. Intente nuevamente.\n");
                    }
                } while (opcionAdmin != 6);
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
                        // Lógica para consulta de catálogo
                        printf("Funcionalidad de consulta de catálogo no implementada aún.\n");
                        break;
                    case 2:
                        // Lógica para consulta de cliente
                        printf("Funcionalidad de consulta de cliente no implementada aún.\n");
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