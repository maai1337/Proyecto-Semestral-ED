#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

/**
 * @brief Representa una arista del grafo que va a un nodo destino con un peso.
 */
struct Arista {
    int destino;  // ID interno del nodo destino
    double peso;  // Peso de la arista
};

/**
 * @brief Clase plantilla para representar un Grafo mediante lista de adyacencia.
 * Permite manejar grafos dirigidos y no dirigidos con pesos.
 */
template <typename T>
class Grafo {
private:
    std::unordered_map<T, int> nodoAId;          // Mapea el valor del nodo (T) a su ID numérico
    std::vector<T> idANodo;                      // Mapea el ID numérico al valor del nodo (T)
    std::vector<std::vector<Arista>> adyacencia; // Lista de adyacencia por ID de nodo
    bool dirigido;                               // Indica si el grafo es dirigido
    int cantidadAristas;                         // Contador de aristas únicas en el grafo

    /**
     * @brief Obtiene el ID de un nodo. Si no existe, lo crea y le asigna uno nuevo.
     */
    int obtenerOAgregarId(const T& nodo) {
        auto it = nodoAId.find(nodo);
        if (it != nodoAId.end()) {
            return it->second;
        }
        int id = idANodo.size();
        nodoAId[nodo] = id;
        idANodo.push_back(nodo);
        adyacencia.push_back(std::vector<Arista>());
        return id;
    }

public:
    /**
     * @brief Constructor del grafo.
     * @param esDirigido true si el grafo es dirigido, false si no lo es (por defecto).
     */
    Grafo(bool esDirigido = false) {
        dirigido = esDirigido;
        cantidadAristas = 0;
    }

    /**
     * @brief Agrega un nuevo vértice al grafo (si no existe previamente).
     */
    void agregarVertice(const T& nodo) {
        obtenerOAgregarId(nodo);
    }
    
    /**
     * @brief Agrega una arista con peso entre dos vértices. Si ya existe, acumula el peso.
     */
    void agregarArista(const T& origen, const T& destino, double peso = 1.0) {
        int u = obtenerOAgregarId(origen);
        int v = obtenerOAgregarId(destino);

        bool encontradaU = false;
        for (auto& arista : adyacencia[u]) {
            if (arista.destino == v) {
                encontradaU = true;
                arista.peso += peso; // Acumula el peso si la arista ya existía
                break;
            }
        }

        if (!encontradaU) {
            adyacencia[u].push_back({v, peso});
            if (!dirigido) {
                adyacencia[v].push_back({u, peso});
            }
            cantidadAristas++;
        } else if (!dirigido) {
            for (auto& arista : adyacencia[v]) {
                if (arista.destino == u) {
                    arista.peso += peso;
                    break;
                }
            }
        }
    }

    /**
     * @brief Remueve la arista entre el nodo origen y el nodo destino.
     */
    void removerArista(const T& origen, const T& destino) {
        int u = obtenerId(origen);
        int v = obtenerId(destino);
        if (u == -1 || v == -1) return;

        bool eliminada = false;
        for (auto it = adyacencia[u].begin(); it != adyacencia[u].end(); ++it) {
            if (it->destino == v) {
                adyacencia[u].erase(it);
                eliminada = true;
                break;
            }
        }
        
        if (eliminada) {
            cantidadAristas--;
            if (!dirigido) {
                for (auto it = adyacencia[v].begin(); it != adyacencia[v].end(); ++it) {
                    if (it->destino == u) {
                        adyacencia[v].erase(it);
                        break;
                    }
                }
            }
        }
    }

    /**
     * @brief Retorna la lista de aristas (vecinos) asociadas a un nodo buscando por su valor de objeto.
     */
    const std::vector<Arista>& obtenerVecinos(const T& nodo) const {
        auto it = nodoAId.find(nodo);
        if (it != nodoAId.end()) {
            return adyacencia[it->second];
        }
        static const std::vector<Arista> vacio; 
        return vacio;
    }

    /**
     * @brief Retorna la lista de aristas (vecinos) asociadas a un nodo buscando por su ID interno.
     */
    const std::vector<Arista>& obtenerVecinos(int id) const {
        if (id >= 0 && id < adyacencia.size()) {
            return adyacencia[id];
        }
        static const std::vector<Arista> vacio;
        return vacio;
    }

    /**
     * @brief Retorna la cantidad total de vértices registrados en el grafo.
     */
    int obtenerCantidadVertices() const {
        return idANodo.size();
    }
    
    /**
     * @brief Retorna la cantidad total de aristas en el grafo.
     */
    int obtenerCantidadAristas() const {
        return cantidadAristas;
    }
    
    /**
     * @brief Retorna el valor original del nodo (T) asociado a un ID interno.
     */
    T obtenerNodo(int id) const {
        if (id >= 0 && id < idANodo.size()) {
            return idANodo[id];
        }
        return T();
    }
    
    /**
     * @brief Retorna el ID numérico interno correspondiente a un nodo. Retorna -1 si no existe.
     */
    int obtenerId(const T& nodo) const {
        auto it = nodoAId.find(nodo);
        if (it != nodoAId.end()) {
            return it->second;
        }
        return -1;
    }

    /**
     * @brief Imprime en consola el contenido del grafo usando las listas de adyacencia.
     */
    void mostrarGrafo() const {
        for (int i = 0; i < adyacencia.size(); i++) {
            if (adyacencia[i].empty()) continue; 
            
            std::cout << idANodo[i] << ": ";
            for (const auto& arista : adyacencia[i]) {
                std::cout << "(" << idANodo[arista.destino] << ", peso: " << arista.peso << ") ";
            }
            std::cout << std::endl;
        }
    }
};

#endif
