
#include "pila.h"
#include <stdlib.h>
#define FACTOR_REDUCIR 2
#define FACTOR_AUMENTAR 2
#define CAPACIDAD_I 10
#define CANTIDAD_I 0


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    if (!pila){
        return NULL;
    }
    pila->capacidad = CAPACIDAD_I;
    pila->cantidad = CANTIDAD_I;
    pila->datos = malloc(sizeof(void**) * CAPACIDAD_I);
    if (!pila->datos){
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == CANTIDAD_I;
}

bool redimensionar(pila_t *pila, bool redim_arriba){
    void *elem_redim = NULL;
    size_t nueva_capacidad;
    
    if (redim_arriba){
        elem_redim = realloc(pila->datos, FACTOR_AUMENTAR * (pila->capacidad) * sizeof(void*));
        nueva_capacidad = (pila->capacidad) * FACTOR_AUMENTAR;
    } else {
        elem_redim = realloc(pila -> datos, sizeof(void*) * ((pila->capacidad)/FACTOR_REDUCIR));
        nueva_capacidad = (pila->capacidad) / FACTOR_REDUCIR;
    }
    if (!elem_redim){
        return false;
    }
    pila->capacidad = nueva_capacidad;
    pila->datos = elem_redim;
    return true; 
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad){
        if (!redimensionar(pila, true)){
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[(pila->cantidad) - 1];
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    void *ex_tope = pila->datos[(pila->cantidad) - 1];
    pila->cantidad--;
    if ((pila->cantidad) > CANTIDAD_I && (2 * FACTOR_REDUCIR * pila->cantidad) < (pila->capacidad)){
        if (!redimensionar(pila, false)){
            return NULL;
        }
    }
    return ex_tope;
}

