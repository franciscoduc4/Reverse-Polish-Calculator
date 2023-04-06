#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

char *substr(const char *str, size_t n){
    char* dest = strndup(str, n);
    return dest;
}

char **split(const char *str, char sep){
    
    size_t largo_cadena = strlen(str);
    int cant_sep = 0;
    
    for (int i = 0; i < largo_cadena; i++){
        if (str[i] == sep){
            cant_sep++; 
        }
    }
 
    char **arr_str = calloc((cant_sep + 2), sizeof(char*));
    if (!arr_str){
        return NULL;
    }
     
    if ((strcmp(str, "") == 0) || !sep){
        arr_str[0] = strdup(str);
        arr_str[1] = NULL;
        return arr_str;
    }

    size_t indice_arr = 0, inicio_substr = 0;
    
    for (int i = 0; i < largo_cadena; i++){
        if (str[i] == sep){
            arr_str[indice_arr] = strndup(str + inicio_substr, i - inicio_substr); 
            inicio_substr = i + 1;
            indice_arr++;
        }
        if (i == largo_cadena - 1){
            arr_str[indice_arr] = strdup(str + inicio_substr);
            indice_arr++;
        }
    }
    arr_str[indice_arr] = NULL;
    return arr_str;
}

char *join(char **strv, char sep){
    
    if (!strv[0]){
        return strdup("");
    }
    
    char separador = sep;
    size_t capacidad = 0;
    for (size_t i = 0; strv[i]; i++){
        capacidad += strlen(strv[i]);
        if (separador){
            capacidad++;
        }
    }

    char *res_str = calloc((capacidad + 1), sizeof(char));
    if (!res_str){
        return NULL;
    }

    size_t contador = 0;
    for (size_t i = 0; strv[i]; i++){
        strcpy(&res_str[contador], strv[i]);
        contador += strlen(strv[i]);
        if (separador){
            strcpy(&res_str[contador], &separador);
            contador++;
        }
    }
    if (res_str[contador - 1] == separador){
        res_str[contador - 1] = '\0';
    } else {
        res_str[contador] = '\0';

    }
    return res_str;
}

void free_strv(char *strv[]){
    for (size_t i = 0; strv[i]; i++){
        free(strv[i]);
    }
    free(strv);
}


