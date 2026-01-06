#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interfaz.h"

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarBanner() {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("  _      _____ ____  _____  ______ _____  _____          \n");
    printf(" | |    |_   _|  _ \\|  __ \\|  ____|  __ \\|_   _|   /\\    \n");
    printf(" | |      | | | |_) | |__) | |__  | |__) | | |    /  \\   \n");
    printf(" | |      | | |  _ <|  _  /|  __| |  _  /  | |   / /\\ \\  \n");
    printf(" | |____ _| |_| |_) | | \\ \\| |____| | \\ \\ _| |_ / ____ \\ \n");
    printf(" |______|_____|____/|_|  \\_\\______|_|  \\_\\_____/_/    \\_\\\n");
    printf(ANSI_COLOR_RESET "\n");
}

void imprimirEncabezado(const char* titulo, const char* color) {
    printf("\n%s" ANSI_BOLD "==========================================================\n", color);
    int len = strlen(titulo);
    int padding = (58 - len) / 2;
    for(int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", titulo);
    printf("==========================================================" ANSI_COLOR_RESET "\n");
}

void imprimirMensajeExito(const char* mensaje) {
    printf(ANSI_COLOR_GREEN ANSI_BOLD "[✓] EXITO: %s" ANSI_COLOR_RESET "\n", mensaje);
}

void imprimirMensajeError(const char* mensaje) {
    printf(ANSI_COLOR_RED ANSI_BOLD "[✗] ERROR: %s" ANSI_COLOR_RESET "\n", mensaje);
}

void imprimirMensajeInfo(const char* mensaje) {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "[ℹ] INFO: %s" ANSI_COLOR_RESET "\n", mensaje);
}

void presionarEnterParaContinuar() {
    printf("\nPresione ENTER para continuar...");
    while (getchar() != '\n');
}
