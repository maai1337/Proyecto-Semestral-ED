#ifndef METRICAS_PRINCIPALES_H
#define METRICAS_PRINCIPALES_H

#include "Grafo.h"
#include "BFS.h"
#include "Dijkstra.h"
#include <vector>
#include <stack>
#include <queue>
#include <limits>
#include <cmath>

/**
 * @brief Calcula el Degree Centrality de todos los nodos.
 * @details Estructuras de Datos usadas: vector para almacenar resultados.
 * Complejidad Espacial: O(V)
 * Complejidad Temporal: O(V)
 */
template <typename T>
std::vector<double> degreeCentrality(const Grafo<T>& grafo) {
    int V = grafo.obtenerCantidadVertices();
    std::vector<double> grados(V, 0.0);

    for (int i = 0; i < V; ++i) {
        // En una lista de adyacencia, el grado es simplemente el tamaño del vector de vecinos
        grados[i] = grafo.obtenerVecinos(i).size();
    }
    return grados;
}

/**
 * @brief Calcula el Closeness Centrality de todos los nodos.
 * @details Fórmula: C(u) = (N-1) / Suma(distancias desde u).
 * * ESTRUCTURAS DE DATOS: vector, queue (BFS) y priority_queue (Dijkstra).
 * * Complejidad Temporal: O(V * (V + E)) [No Ponderado] o O(V * E * log(V)) [Ponderado]
 * Complejidad Espacial: O(V)
 */
template <typename T>
std::vector<double> closenessCentrality(const Grafo<T>& grafo, bool ponderado = false) {
    int V = grafo.obtenerCantidadVertices();
    std::vector<double> closeness(V, 0.0);

    for (int i = 0; i < V; ++i) {
        std::vector<double> distancias = ponderado ? calcularDijkstra(grafo, i) : calcularBFS(grafo, i);

        double sumaDistancias = 0;
        for (double d : distancias) {
            if (d != std::numeric_limits<double>::infinity()) {
                sumaDistancias += d;
            }
        }

        if (sumaDistancias > 0) {
            closeness[i] = (V - 1) / sumaDistancias; // Normalizado
        }
    }
    return closeness;
}



/**
 * @brief Calcula el PageRank Iterativo de la red.
 * @details Utiliza el metodo de las potencias. Distribuye el PageRank a los vecinos.
 * Parámetros: damping_factor = 0.85, iteraciones = 100.
 * * ESTRUCTURAS DE DATOS: vector (para almacenar los ranks actuales y nuevos).
 * * Complejidad Temporal: O(Iteraciones * (V + E))
 * Complejidad Espacial: O(V)
 */
template <typename T>
std::vector<double> pageRank(const Grafo<T>& grafo, int iteraciones = 100, double d = 0.85) {
    int V = grafo.obtenerCantidadVertices();
    std::vector<double> pr(V, 1.0 / V); // Inicialización uniforme
    std::vector<double> pr_nuevo(V, 0.0);

    for (int iter = 0; iter < iteraciones; ++iter) {
        std::fill(pr_nuevo.begin(), pr_nuevo.end(), (1.0 - d) / V);

        for (int i = 0; i < V; ++i) {
            const auto& vecinos = grafo.obtenerVecinos(i);
            int numVecinos = vecinos.size();

            if (numVecinos > 0) {
                for (const auto& arista : vecinos) {
                    int destino = arista.destino;
                    // El nodo i reparte su PageRank actual entre sus vecinos
                    pr_nuevo[destino] += d * (pr[i] / numVecinos);
                }
            } else {
                // Nodos sin salida (Dangling nodes): su rank se pierde o se suma al total (simplificado aquí)
                pr_nuevo[i] += d * pr[i];
            }
        }
        pr = pr_nuevo;
    }
    return pr;
}



#endif