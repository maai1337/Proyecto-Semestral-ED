#ifndef METRICAS_ADICIONALES_H
#define METRICAS_ADICIONALES_H

#include "Grafo.h"
#include "BSF.h"
#include "Dijkstra.h"
#include <vector>
#include <limits>
#include <algorithm>

/**
 * @brief Calcula la Excentricidad (Eccentricity Centrality) de todos los nodos.
 * @details La excentricidad de un nodo es la máxima distancia desde él hacia cualquier otro nodo.
 * Mide cuán aislado puede llegar a estar el nodo en el peor de los casos.
 * Fórmula: E(u) = max(dist(u,v))
 * Complejidad Temporal: O(V * (V + E)) [No Ponderado] y O(V * E * Log(V)) [Ponderado]
 * Complejidad Espacial: O(V)
 */
template <typename T>
std::vector<double> eccentricityCentrality(const Grafo<T>& grafo, bool ponderado = false) {
    int V = grafo.obtenerCantidadVertices();
    std::vector<double> excentricidades(V, 0.0);

    for (int i = 0; i < V; ++i) {
        std::vector<double> distancias = ponderado ? calcularDijkstra(grafo, i) : calcularBFS(grafo, i);

        double maxDist = 0.0;
        for (int j = 0; j < V; ++j) {
            if (i != j && distancias[j] != std::numeric_limits<double>::infinity()) {
                if (distancias[j] > maxDist) {
                    maxDist = distancias[j];
                }
            }
        }
        excentricidades[i] = maxDist;
    }
    return excentricidades;
}

/**
 * @brief Calcula la Centralidad Armónica (Harmonic Centrality).
 * @details Variación robusta de Closeness Centrality para grafos desconectados.
 * Suma los inversos de las distancias. Si un nodo es inalcanzable, su inverso es 0.
 * Fórmula: H(u) = Suma(1 / dist(u, v))
 * Complejidad Temporal: O(V * (V + E)) [No Ponderado] y O(V * E * Log V) [Ponderado]
 * Complejidad Espacial: O(V)
 */
template <typename T>
std::vector<double> harmonicCentrality(const Grafo<T>& grafo, bool ponderado = false) {
    int V = grafo.obtenerCantidadVertices();
    std::vector<double> armonica(V, 0.0);

    for (int i = 0; i < V; ++i) {
        std::vector<double> distancias = ponderado ? calcularDijkstra(grafo, i) : calcularBFS(grafo, i);

        double sumaInversos = 0.0;
        for (int j = 0; j < V; ++j) {
            if (i != j) {
                if (distancias[j] != std::numeric_limits<double>::infinity() && distancias[j] > 0) {
                    sumaInversos += (1.0 / distancias[j]);
                }
            }
        }
        armonica[i] = sumaInversos;
    }
    return armonica;
}

#endif