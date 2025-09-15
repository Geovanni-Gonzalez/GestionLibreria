#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "Utilidades.h"
#include "Config.h" 



int cargarConfig(const char* archivo, Config* cfg) {
    char* texto = leerArchivo(archivo);
    if (!texto) return 0;

    cJSON* root = cJSON_Parse(texto);
    if (!root) {
        printf("Error al parsear JSON\n");
        free(texto);
        return 0;
    }

    cJSON* item;

    item = cJSON_GetObjectItem(root, "nombreLocalComercial");
    if (item && item->valuestring) strcpy(cfg->nombreLocalComercial, item->valuestring);

    item = cJSON_GetObjectItem(root, "telefono");
    if (item && item->valuestring) strcpy(cfg->telefono, item->valuestring);

    item = cJSON_GetObjectItem(root, "cedulaJuridica");
    if (item && item->valuestring) strcpy(cfg->cedulaJuridica, item->valuestring);

    item = cJSON_GetObjectItem(root, "horarioAtencion");
    if (item && item->valuestring) strcpy(cfg->horarioAtencion, item->valuestring);

    item = cJSON_GetObjectItem(root, "numeroSiguientePedido");
    if (item) cfg->numeroSiguientePedido = item->valueint;

    item = cJSON_GetObjectItem(root, "usuario");
    if (item && item->valuestring) strcpy(cfg->admin.usuario, item->valuestring);

    item = cJSON_GetObjectItem(root, "password");
    if (item && item->valuestring) strcpy(cfg->admin.password, item->valuestring);

    cJSON_Delete(root);
    free(texto);
    return 1;
}


int guardarConfig(const char* archivo, Config* cfg) {
    cJSON* root = cJSON_CreateObject();
    if (!root) return 0;

    cJSON_AddStringToObject(root, "nombreLocalComercial", cfg->nombreLocalComercial);
    cJSON_AddStringToObject(root, "telefono", cfg->telefono);
    cJSON_AddStringToObject(root, "cedulaJuridica", cfg->cedulaJuridica);
    cJSON_AddStringToObject(root, "horarioAtencion", cfg->horarioAtencion);
    cJSON_AddNumberToObject(root, "numeroSiguientePedido", cfg->numeroSiguientePedido);
    cJSON_AddStringToObject(root, "usuario", cfg->admin.usuario);
    cJSON_AddStringToObject(root, "password", cfg->admin.password);

    char* texto = cJSON_Print(root);
    if (!texto) {
        cJSON_Delete(root);
        return 0;
    }

    FILE* f = fopen(archivo, "w");
    if (!f) {
        free(texto);
        cJSON_Delete(root);
        return 0;
    }
    fprintf(f, "%s\n", texto);
    fclose(f);

    free(texto);
    cJSON_Delete(root);
    return 1;
}


int actualizarNumeroSiguientePedido(Config* cfg) {
    cfg->numeroSiguientePedido += 1;
    return guardarConfig("admin.json", cfg);
}

