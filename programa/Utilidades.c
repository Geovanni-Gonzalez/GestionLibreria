#include "Utilidades.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

bool validarCedulaUnica(const char* cedula, Cliente* arregloClientes, int cantidadDeClientes) {
    for (int i = 0; i < cantidadDeClientes; i++) {
        if (strcmp(arregloClientes[i].cedula, cedula) == 0) {
            return false; // ya existe
        }
    }
    return true;
}

bool validarTelefono(const char* telefono) {
    int len = strlen(telefono);
    if (len < 8) return false;
    for (int i = 0; i < len; i++) {
        if (!isdigit(telefono[i])) return false;
    }
    return true;
}