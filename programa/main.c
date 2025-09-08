#include "Cliente.h"
#include "Pedido.h"
#include "Libro.h"
#include "Utilidades.h"
#include "Config.h"
#include <stdio.h>


int main() {
    Config cfg;
    if (cargarConfig("data/admin.json", &cfg)) {
        printf("Configuración cargada correctamente:\n");
        printf("Nombre del local: %s\n", cfg.nombreLocalComercial);
        printf("Teléfono: %s\n", cfg.telefono);
        printf("Cédula Jurídica: %s\n", cfg.cedulaJuridica);
        printf("Horario de Atención: %s\n", cfg.horarioAtencion);
        printf("Número del siguiente pedido: %d\n", cfg.numeroSiguientePedido);
        printf("Usuario Admin: %s\n", cfg.admin.usuario);
    } else {
        printf("Error al cargar la configuración.\n");
    }

    return 0;
}