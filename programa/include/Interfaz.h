#ifndef INTERFAZ_H
#define INTERFAZ_H

// Colores ANSI
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD          "\x1b[1m"

// Funciones de Interfaz
void limpiarPantalla();
void mostrarBanner();
void imprimirEncabezado(const char* titulo, const char* color);
void imprimirMensajeExito(const char* mensaje);
void imprimirMensajeError(const char* mensaje);
void imprimirMensajeInfo(const char* mensaje);
void presionarEnterParaContinuar();

#endif
