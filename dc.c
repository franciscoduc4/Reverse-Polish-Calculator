
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "pila.h"
#include "strutil.h"
#include "calc_helper.h"
#include <ctype.h>
#define _POSIX_C_SOURCE 200809L

bool operar(calc_num *res, struct calc_oper operador, calc_num *operandos){
    
    long res_parcial = 0;

    if (operador.op == OP_ADD){
        res_parcial = operandos[1] + operandos[0];
    } 

    else if (operador.op == OP_SUB){
        res_parcial = operandos[1] - operandos[0];
    } 

    else if (operador.op == OP_MUL){
        res_parcial = operandos[1] * operandos[0];
    } 

    else if (operador.op == OP_DIV){
        if (operandos[0] == 0){
            return false;
        }
        res_parcial =  operandos[1] / operandos[0];
    } 

    else if (operador.op == OP_RAIZ){
        if (operandos[0] == 0){
            res_parcial = 0;
        }
        if (operandos[0] == 1){
            res_parcial = 1;
        }
        if (operandos[0] < 0){
            return false;
        } else {
            double res_raiz = sqrt((double)operandos[0]);
            res_parcial = (long) res_raiz;
        }
    }

    else if (operador.op == OP_TERN){
        res_parcial = operandos[2] ? operandos[1] : operandos[0];
    }

    else if (operador.op == OP_POW){
        if (operandos[0] == 0){
            res_parcial = 1;
        }
        else if (operandos[0] < 0 || operandos[1] < 0){
            return false;
        }
        else if (operandos[1] == 0){
            res_parcial = 0;
        
        } else {
            double res_pow; 
            bool pow_negativa = false;
            
            if (operandos[0] < 0){
                pow_negativa = true;
                operandos[0] *= -1;
            }
            res_pow = pow((double)operandos[1], (double)operandos[0]);
    
            if (pow_negativa){
                res_parcial = 1 / (long)res_pow;
            }
            else {
                res_parcial = (long)res_pow;
            }
        }
    }

    else if (operador.op == OP_LOG){
        if (operandos[0] == operandos[1]){
            res_parcial = 1;
        }
        else if (operandos[0] > operandos[1] || operandos[1] == 1){
            res_parcial = 0;
        }
        else if (operandos[0] <= 1 || operandos[1] <= 0){
            return false;
        } else {
            res_parcial = (long)(log((double)operandos[1]) / log ((double)operandos[0]));
        }
    }

    else {
        return false;
    }

    *res = (calc_num) res_parcial;
    return true;
}

void comprobar_op(calc_num *res, pilanum_t* pila_num, int num_para_op, int cant_operandos, struct calc_oper operador, bool *hay_error){
    
    calc_num operandos[cant_operandos];
    
    int j = 0;
    while (!*hay_error && j < cant_operandos){
        bool nro_desapilado = desapilar_num(pila_num, &operandos[j]);
        if (!nro_desapilado){
            *hay_error = true;
            continue;
        }
        j++;
    }

    if (!*hay_error){
        calc_num res_parcial;
        bool op_realizar = operar(&res_parcial, operador, operandos);
        if (!op_realizar){
            *hay_error = true;
        } 
        else {
            apilar_num(pila_num, res_parcial);
        }
    }
}

calc_num polish_dc(bool *hay_error, char *infix_output, size_t tam){

    char** linea;
    pilanum_t *pila_num = pilanum_crear();
    int num_para_op = 0;
    int cant_operandos = 0;
    int num = 0;
    calc_num res = 0;
    
    linea = dc_split(infix_output);
    for (int i = 0; linea[i]; i++){
        struct calc_token token;
        calc_parse(linea[i], &token);
        
        if (token.type == TOK_NUM){
            apilar_num(pila_num, token.value);
            num++;
        }

        else if (token.type == TOK_OPER){
            if (num_para_op == 0){
                num_para_op = token.oper.num_operandos;
            }
            else {
                num_para_op += (token.oper.num_operandos - 1);
            }
            cant_operandos = token.oper.num_operandos;
            comprobar_op(&res, pila_num, num_para_op, cant_operandos, token.oper, hay_error);
        }
    }
    desapilar_num(pila_num, &res);
    calc_num elemento_extra; 
    if (desapilar_num(pila_num, &elemento_extra)){
        *hay_error = true;
    }
    if (num_para_op != num){
        *hay_error = true;
    }
    pilanum_destruir(pila_num);
    free_strv(linea);
    return res;
}


int main(int argc, char const *argv[]){
    char *linea = NULL;
    size_t tam = 0;
    while (getline(&linea, &tam, stdin) >= 0){
        bool hay_error = false;
        calc_num res_output = polish_dc(&hay_error, linea, tam);
        if (hay_error){
            printf("ERROR\n");
        } else {
            printf("%li\n", res_output);
        } 
    }
    free(linea);
    return 0;
}










