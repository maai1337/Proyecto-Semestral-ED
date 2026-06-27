#ifndef BFS_H
#define BFS_H

#include "Grafo.h"
#include <vector>
#include <queue>
#include <limits> // Esto es para el valor infinito cuando no haya camino

// Realiza un recorrido BFS desde un nodo origen (por su ID interno).
// La complejidad temporal es O(V + E), con V cantidad de vertices y E cantidad de aristas
template <typename T>
std::vector<double> calcularBFS(const Grafo<T>& grafo, int idOrigen) {
    int n = grafo.obtenerCantidadVertices();
    // Inicializamos todas las distancias en infinito
    std::vector<double> distancias(n, std::numeric_limits<double>::infinity());
    std::queue<int> cola;

    distancias[idOrigen] = 0.0;
    cola.push(idOrigen);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        // Iterar sobre los vertices vecinos
        for (const auto& arista : grafo.obtenerVecinos(actual)) {
            int vecino = arista.destino;

            // Si el nodo no ha sido visitado (distancia es infinita)
            if (distancias[vecino] == std::numeric_limits<double>::infinity()) {
                distancias[vecino] = distancias[actual] + 1.0; // En BFS sin pesos, cada salto suma 1
                cola.push(vecino);
            }
        }
    }

    return distancias;
}

#endif