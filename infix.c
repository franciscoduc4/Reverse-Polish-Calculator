#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "pila.h"
#include <string.h>
#include "strutil.h"
#include "calc_helper.h"


char *infix(char* cad, size_t tam){

    char **linea = infix_split(cad);
    pila_t *pila_op = pila_crear();
    unsigned precedencia[tam];
    int cant_op = 0;
    int cant_num = 0;

    if (!pila_op){
        return NULL;
    }

    for (int i = 0; linea[i]; i++){
        struct calc_token token;
        calc_parse(linea[i], &token);
    
        if (token.type == TOK_NUM){
            if (cant_num != 0){
                printf(" ");
            }
            printf("%li", token.value);
            cant_num++;    
        }

        else if (token.type == TOK_LPAREN || (token.type == TOK_OPER && cant_op == 0)){ 
            pila_apilar(pila_op, linea[i]);
            precedencia[cant_op] = token.oper.precedencia;
            cant_op++;
        }

        else if (token.type == TOK_RPAREN){
            char *tope = pila_ver_tope(pila_op);
            while (cant_op > 0 && *tope != '('){
                char *op = pila_desapilar(pila_op);
                printf(" %c", *op);
                cant_op--;
                tope = pila_ver_tope(pila_op);
            }
            pila_desapilar(pila_op);
            cant_op--;      
        }

        else if (token.type == TOK_OPER){
            char *tope = pila_ver_tope(pila_op);
            while (cant_op > 0 && (precedencia[cant_op - 1] > token.oper.precedencia || (precedencia[cant_op - 1] >= token.oper.precedencia && token.oper.asociatividad == ASSOC_IZQ)) && (*tope != '(')){
                char *op = pila_desapilar(pila_op);
                printf(" %c", *op);
                cant_op--;
                tope = pila_ver_tope(pila_op);
            }
            pila_apilar(pila_op, linea[i]);
            precedencia[cant_op] = token.oper.precedencia;
            cant_op++;
        } 
        else {
            return NULL;
        }
    }
    while (!pila_esta_vacia(pila_op) && cant_op > 0){
        char *op = pila_desapilar(pila_op);
        printf(" %c", *op);
        cant_op--;
    }
    free_strv(linea);
    pila_destruir(pila_op);
    return NULL;
}


int main(int argc, char const *argv[]){
    size_t tam = 0;
    char *linea = NULL;
    while (getline(&linea, &tam, stdin) >= 0) {
        infix(linea, tam);
        printf("\n");
    }
    free(linea);
    return 0;
}






