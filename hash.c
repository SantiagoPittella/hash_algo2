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

unsigned long funcion_hash();  //buscar alguna de las que tengo guardadas en internet.

/*
Inicializa las lista, para cada posicion del hash, 
cuando haciendo mas facil chequear si ya esta o no una clave, 
pues si la lista esta vacia, no esta la clave directamente.
Ademas, no hay que crear una lista cada vez que se quiere guardar algo.
Esta funcion tambien se puede usar a la hora de redimensionar la tabla.
*/
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

/*
    PARA COMPARAR CLAVES, USAR strcmp()
    (en hash_obtener y hash_guardar)
*/
void* hash_obtener(const hash_t* hash, const char* clave) {
    /*
        f_hash(clave), recorrer lista, si no esta clave, devolver NULL, 
        si esta, devolver dato.
    */
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
    /*
        f_hash(clave) para obtener posicion de la lista a buscar,
        si la clave ya esta en la lista, cambiar dato.
        SI la clave no esta en la lista, lista_insertar(clave, dato) (mejor insertar al final, creo).
        En caso de error insertando devuelve false.

    */
    return true;
}

void hash_destruir(hash_t* hash) {
    /*
    Recorrer todas las listas, borrando las claves (las copiamos), 
    luego borrar las listas.
    free(hash->campos)
    free(hash)
    */
}