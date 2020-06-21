#include "lista_hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"
typedef struct nodo {
    struct nodo* proximo;
    void* dato;
    char* clave;
} nodo_t;

struct lista_hash {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

/*
Crea un nodo con el dato que se le pasa por parametro.
devuelve true si el nodo se creo correctamente,
false en caso de error
*/
nodo_t* nodo_crear(char* clave, void* dato) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return false;

    nodo->clave = strdup(clave);
    nodo->dato = dato;
    nodo->proximo = NULL;
    return nodo;
}

lista_hash_t* lista_crear() {
    lista_hash_t* lista = malloc(sizeof(lista_hash_t));
    if (!lista) return NULL;

    lista->largo = 0;
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

bool lista_esta_vacia(const lista_hash_t* lista) {
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_hash_t* lista, char* clave, void* dato) {
    nodo_t* nodo = nodo_crear(clave, dato);
    if (!nodo) return false;

    nodo->proximo = lista->primero;
    lista->largo++;
    lista->primero = nodo;

    if (!lista->ultimo) lista->ultimo = nodo;

    return true;
}

bool lista_insertar_ultimo(lista_hash_t* lista, char* clave, void* dato) {
    nodo_t* nodo = nodo_crear(clave, dato);
    if (!nodo) return false;

    if (lista_esta_vacia(lista)) {
        lista->primero = nodo;
    } else {
        lista->ultimo->proximo = nodo;
    }

    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void* lista_borrar_primero(lista_hash_t* lista) {  //a la hora de borrar, hay qye borrar la clave
    if (lista_esta_vacia(lista)) return NULL;

    nodo_t* primero = lista->primero;
    void* dato = lista->primero->dato;
    lista->primero = lista->primero->proximo;

    if (lista->largo == 1) {
        lista->ultimo = lista->ultimo->proximo;
    }

    free(primero);
    lista->largo--;
    return dato;
}

void* lista_ver_primero(const lista_hash_t* lista) {
    return (lista_esta_vacia(lista) ? NULL : lista->primero->dato);
}

void* lista_ver_ultimo(const lista_hash_t* lista) {
    return (lista_esta_vacia(lista) ? NULL : lista->ultimo->dato);
}

size_t lista_largo(const lista_hash_t* lista) {
    return lista->largo;
}

void lista_destruir(lista_hash_t* lista, void destruir_dato(void*)) {
    while (!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato != NULL) {
            destruir_dato(dato);
        }
    }
    free(lista);
}

/*
iterador interno
*/
void lista_iterar(lista_hash_t* lista, bool visitar(void* dato, void* extra), void* extra) {
    nodo_t* actual = lista->primero;
    while (actual) {
        if (!visitar(actual->dato, extra)) return;
        actual = actual->proximo;
    }
}

/*
iterador externo
*/

struct lista_iter {
    nodo_t* actual;
    nodo_t* anterior;
    lista_hash_t* lista;
};

lista_iter_t* lista_iter_crear(lista_hash_t* lista) {
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) return NULL;

    iter->lista = lista;
    iter->actual = lista->primero;
    iter->anterior = NULL;
    return iter;
}

void* lista_iter_ver_actual(const lista_iter_t* iter) {
    return (lista_iter_al_final(iter) ? NULL : iter->actual->dato);
}

bool lista_iter_al_final(const lista_iter_t* iter) {
    return (!iter->actual ? true : false);
}

void lista_iter_destruir(lista_iter_t* iter) {
    free(iter);
}

void* lista_iter_borrar(lista_iter_t* iter) {  //borrar con iterador implica borrar clave
    if (lista_iter_al_final(iter)) return NULL;

    nodo_t* auxiliar = iter->actual;
    void* dato = auxiliar->dato;

    iter->actual = iter->actual->proximo;
    iter->lista->largo--;
    if (!iter->anterior) {
        iter->lista->primero = iter->actual;
    } else if (!iter->actual) {
        iter->lista->ultimo = iter->anterior;
        iter->anterior->proximo = NULL;
    } else {
        iter->anterior->proximo = iter->actual;
    }
    free(auxiliar);
    return dato;
}

bool lista_iter_avanzar(lista_iter_t* iter) {
    if (lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->proximo;

    return true;
}

bool lista_iter_insertar(lista_iter_t* iter, char* clave, void* dato) {
    if (!iter->actual) {
        if (!iter->anterior) {  //lista vacia
            if (!lista_insertar_primero(iter->lista, clave, dato)) return false;
            iter->actual = iter->lista->primero;
        } else {  //iter al final
            if (!lista_insertar_ultimo(iter->lista, clave, dato)) return false;
            iter->actual = iter->lista->ultimo;
        }
        return true;
    }

    if (!iter->anterior) {  // iter en primera posicion
        if (!lista_insertar_primero(iter->lista, clave, dato)) return false;
        iter->actual = iter->lista->primero;
        return true;
    }

    nodo_t* auxiliar = nodo_crear(clave, dato);
    if (!auxiliar) return false;
    auxiliar->proximo = iter->actual;
    iter->actual = auxiliar;
    iter->anterior->proximo = auxiliar;
    iter->lista->largo++;
    return true;
}