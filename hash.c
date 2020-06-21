#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "lista_hash.h"

#define CAPACIDAD_INICIAL 19
/*
    * Crear estructura nodo, llamada hash_eleme_t, creo se crea una
    * tiene que copiar la clave, siempre string (strcpy), y almacenar 
    * el valor.
    * 
    * hay que importar la lista, y cambiarla para que funcione con hash.
    * 
    * Crear lista o alguna funcion para ver numeros primos.
*/

struct hash {
    lista_hash_t** campos;
    size_t capacidad;
    unsigned long f_hash;
    void (*hash_destruir_dato_t)(void*);
    size_t cantidad_guardados;
};

struct hash_iter {
    hash_t* hash;
    lista_hash_t* lista_actual;
    lista_iter_t* iterador_actual;
    size_t num_lista;  //revisar, indicaria la posicion dentro del hash de la lista actual.
};

unsigned long funcion_hash();

bool _inicializar_listas(lista_hash_t** campos, size_t cantidad) {
    for (size_t i = 0; i < cantidad; i++) {
        campos[i] = lista_hash_crear();
        if (!campos[i]) return false;
    }
    return true;
}
hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash));
    if (!hash) return NULL;

    hash->capacidad = CAPACIDAD_INICIAL;
    hash->campos = malloc(CAPACIDAD_INICIAL * sizeof(lista_hash_t*));
    if (!hash->campos) {
        free(hash);
        return NULL;
    }
    if (!_inicializar_listas(hash->campos, CAPACIDAD_INICIAL)) {
        free(hash->campos);
        free(hash);
        return NULL;
    }
    hash->hash_destruir_dato_t = destruir_dato;
    hash->f_hash = funcion_hash;
    hash->cantidad_guardados = 0;
    return hash;
}

size_t hash_cantidad(const hash_t* hash) {
    return hash->cantidad_guardados;
}

bool hash_pertenece(const hash_t* hash, const char* clave) {
    return (hash_obtener(hash, clave) != NULL);
}