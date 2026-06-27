#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grafo.h"
#include <vector>
#include <queue>
#include <limits>

// Realiza el algoritmo de Dijkstra desde un nodo origen.
// Complejidad Temporal: O(E * log(V)) usando una priority_queue
template <typename T>
std::vector<double> calcularDijkstra(const Grafo<T>& grafo, int idOrigen) {
    int n = grafo.obtenerCantidadVertices();
    std::vector<double> distancias(n, std::numeric_limits<double>::infinity());

    // PQ: almacena pares (distancia_acumulada, id_nodo).
    // Usamos std::greater para que funcione como un Min-Heap (menor distancia va quedando en la raiz).
    using ParDistNodo = std::pair<double, int>;
    std::priority_queue<ParDistNodo, std::vector<ParDistNodo>, std::greater<ParDistNodo>> pq;

    distancias[idOrigen] = 0.0;
    pq.push({0.0, idOrigen});

    while (!pq.empty()) {
        double distActual = pq.top().first;
        int actual = pq.top().second;
        pq.pop();

        // Optimización: Si se saca una distancia desactualizada (mayor a la registrada), se ignora.
        if (distActual > distancias[actual]) continue;

        for (const auto& arista : grafo.obtenerVecinos(actual)) {
            int vecino = arista.destino;
            double pesoArista = arista.peso;

            // Relajación: Si se encuentra un camino más corto, se actualiza y se hace enqueue
            if (distancias[actual] + pesoArista < distancias[vecino]) {
                distancias[vecino] = distancias[actual] + pesoArista;
                pq.push({distancias[vecino], vecino});
            }
        }
    }

    return distancias;
}

#endif