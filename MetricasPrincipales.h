#ifndef METRICAS_PRINCIPALES_H
#define METRICAS_PRINCIPALES_H

#include "Grafo.h"
#include "BSF.h"
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

/**
 * @brief Calcula el Average Shortest Path de toda la red.
 * @details Metrica global, describe la red entera. Suma todas las distancias mínimas y divide por el número de caminos válidos.
 * * ESTRUCTURAS DE DATOS: vector (almacenamiento de distancias por nodo), queue y priority_queue (vía algoritmos base).
 * * Complejidad Temporal: O(V * (V + E)) [No Ponderado] o O(V * E * log(V)) [Ponderado]
 * * Complejidad Espacial: O(V)
 */
template <typename T>
double averageShortestPath(const Grafo<T>& grafo, bool ponderado = false) {
    int V = grafo.obtenerCantidadVertices();
    double sumaTotal = 0;
    long long caminosValidos = 0;

    for (int i = 0; i < V; ++i) {
        std::vector<double> distancias = ponderado ? calcularDijkstra(grafo, i) : calcularBFS(grafo, i);

        for (int j = 0; j < V; ++j) {
            if (i != j && distancias[j] != std::numeric_limits<double>::infinity()) {
                sumaTotal += distancias[j];
                caminosValidos++;
            }
        }
    }

    if (caminosValidos == 0) return 0;
    return sumaTotal / caminosValidos;
}

/**
 * @brief Calcula el Betweenness Centrality usando el Algoritmo de Brandes (No ponderado).
 * @details Estructuras de datos usadas:Stack para el orden inverso, Queue para BFS,
 * y Vectores para predecesores y caminos más cortos (DAG).
 * * Nota: No usa la función 'calcularBFS' por que requiere algoritmo de Brandes
 * Complejidad Temporal: O(V * (V + E))
 * Complejidad Espacial: O(V + E)
 */
template <typename T>
std::vector<double> betweennessCentrality(const Grafo<T>& grafo) {
    int V = grafo.obtenerCantidadVertices();
    std::vector<double> betweenness(V, 0.0);

    for (int s = 0; s < V; ++s) {
        std::stack<int> S;
        std::vector<std::vector<int>> P(V);
        std::vector<double> sigma(V, 0.0);
        sigma[s] = 1.0;
        std::vector<int> d(V, -1);
        d[s] = 0;
        std::queue<int> Q;
        Q.push(s);

        // BFS modificado para contar caminos mas cortos
        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            S.push(v);

            for (const auto& arista : grafo.obtenerVecinos(v)) {
                int w = arista.destino;
                if (d[w] < 0) {
                    Q.push(w);
                    d[w] = d[v] + 1;
                }
                if (d[w] == d[v] + 1) {
                    sigma[w] += sigma[v];
                    P[w].push_back(v);
                }
            }
        }

        std::vector<double> delta(V, 0.0);
        // Acumulacion de dependencias
        while (!S.empty()) {
            int w = S.top();
            S.pop();
            for (int v : P[w]) {
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            }
            if (w != s) {
                betweenness[w] += delta[w];
            }
        }
    }

    // Divide por 2 porque las aristas no tienen direccion y cuentan doble (en el caso de grafo no dirigido)
    for (int i = 0; i < V; ++i) {
        betweenness[i] /= 2.0;
    }
    return betweenness;
}

#endif