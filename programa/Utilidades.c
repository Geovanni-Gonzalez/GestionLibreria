#include "Utilidades.h"
#include <string.h>

int validarStringAlfa(char *string, int largo){
    if (strlen(string)==largo){
        for (int i=0;i<=largo;i++){
            if (!isalpha(string[i])){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int validarStringNumerico(char *string, int largo){
    if (strlen(string)==largo){
        for (int i=0;i<=largo;i++){
            if (!isdigit(string[i])){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}