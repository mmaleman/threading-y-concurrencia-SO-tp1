#ifndef CHM_CPP
#define CHM_CPP

#include <iostream>
#include <fstream>
#include <pthread.h>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    /* El enunciado dice "Se debe garantizar que solo haya contencion en caso de colision de hash: es decir, si dos o mas
    threads intentan incrementar concurrentemente claves que no colisionan, deben poder hacerlo sin inconvenientes"
    PONELE que tenemos dos threas. Uno ejecuta incrementar en tabla[i] y otro en tabla[j]. Entre ellos no tienen por qué restringirse.
    Pero si ambos ejecutan incrementar en tabla[i], necesitamos algo que deje pasar a uno y haga esperar al otro. Mepa que necesitamos
    eso para cada una de las 26 posiciones de la tabla... Un arreglo de 26 mutex?*/
    static unsigned int indice = HashMapConcurrente::hashIndex(clave);
    ListaAtomica<hashMapPair> *lista = tabla[indice];
    bool encontrado = false;
    // TODO: aca iria algo que maneje bien las colisiones. mutex[indice]? hacerle .wait()?
    ListaAtomica<hashMapPair>::Iterador it = (*lista).crearIt();
    while(it.haySiguiente() && !encontrado) {
        if(it.siguiente().first == clave) {
            it.siguiente().second++;
            encontrado = true;
        }
        it.avanzar();
    }
    if(!encontrado) {
        (*lista).insertar(make_pair(clave,1));
    }
    // TODO: hacer .signal()?
}

std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (
            auto it = tabla[index]->crearIt();
            it.haySiguiente();
            it.avanzar()
        ) {
            // version original
            if (it.siguiente().second > max->second) {
                max->first = it.siguiente().first;
                max->second = it.siguiente().second;
            }
            /*
            // ejercicio 3a
            // hashMapPair *possibleNewMax = new hashMapPair;
            // possibleNewMax->first = it.siguiente().first;
            // possibleNewMax->second = it.siguiente().second;
            
            // // habría que ver si se puede hacer todo el if atómico
            // if (possibleNewMax->second > max->second){
            //     max = possibleNewMax;
            // }
            */
        }
    }

    return *max;
}
/*
struct thread_args {
    hashMapPair &max;
    int (&filas)[HashMapConcurrente::cantLetras];
};

// Recorre las filas sin ocupar y las marca atómicamente cuando empieza
void *funcion_thread(void *arg) {
    thread_args *args_struct = (thread_args *) arg;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        // Si filas[index] es 1, ya fue tomada y avanzo. Sino la ocupo
        if (args_struct->filas[index].atomic_compare_exchange_strong(1,1)) break;
        for (
            auto it = tabla[index]->crearIt();
            it.haySiguiente();
            it.avanzar()
        ) {
            if (it.siguiente().second > args_struct->max->second) {
                args_struct->max->first = it.siguiente().first;
                args_struct->max->second = it.siguiente().second;
            }
        }
    }
}

hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cantThreads) {
    // Completar (Ejercicio 3)
    pthread_t tid[cantThreads];

    // declarar thread_args
    int filas[HashMapConcurrente::cantLetras] = {0};

    hashMapPair *max = new hashMapPair();
    max->second = 0;

    thread_args args = {*max, filas};

    for (int i = 0; i < cantThreads; i++){
        pthread_create(&tid[i], NULL, funcion_thread, &args);
    }

    for (int i = 0; i < cantThreads; i++){
        pthread_join(tid[i], NULL);
    }

    return *max;
}
*/

#endif
