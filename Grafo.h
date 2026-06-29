#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

struct Arista {
    int destino;
    double peso;
};

template <typename T>
class Grafo {
private:
    std::unordered_map<T, int> nodoAId;
    std::vector<T> idANodo;
    
    std::vector<std::vector<Arista>> adyacencia;

    bool dirigido;
    int cantidadAristas;

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
    //Constructor de la clase Grafo
    //Recibe si el grafo es dirigido o no
    Grafo(bool esDirigido = false) {
        dirigido = esDirigido;
        cantidadAristas = 0;
    }

    //agrega vertice al grafo si todavia no existe
    void agregarVertice(const T& nodo) {
        obtenerOAgregarId(nodo);
    }
    
    //agrega una arista entre el vertice origen y el vertice destino
    void agregarArista(const T& origen, const T& destino, double peso = 1.0) {
        int u = obtenerOAgregarId(origen);
        int v = obtenerOAgregarId(destino);

        bool encontradaU = false;
        for (auto& arista : adyacencia[u]) {
            if (arista.destino == v) {
                encontradaU = true;
                // Acumulamos el tiempo total de conexión
                arista.peso += peso;
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

    //remueve una arista entre el vertice origen y el vertice destino
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
    //retorna los vecinos de un vertice
    //si el vertice no existe, retorna un vector vacio
    const std::vector<Arista>& obtenerVecinos(const T& nodo) const {
        auto it = nodoAId.find(nodo);
        if (it != nodoAId.end()) {
            return adyacencia[it->second];
        }
        static const std::vector<Arista> vacio; 
        return vacio;
    }

    const std::vector<Arista>& obtenerVecinos(int id) const {
        if (id >= 0 && id < adyacencia.size()) {
            return adyacencia[id];
        }
        static const std::vector<Arista> vacio;
        return vacio;
    }

    int obtenerCantidadVertices() const {
        return idANodo.size();
    }
    
    //retorna la cantidad de aristas del grafo
    int obtenerCantidadAristas() const {
        return cantidadAristas;
    }
    
    T obtenerNodo(int id) const {
        if (id >= 0 && id < idANodo.size()) {
            return idANodo[id];
        }
        return T();
    }
    
    int obtenerId(const T& nodo) const {
        auto it = nodoAId.find(nodo);
        if (it != nodoAId.end()) {
            return it->second;
        }
        return -1;
    }

    //Muestra el contenido del grafo usando la lista de adyacencia
    void mostrarGrafo() const {
        for (int i = 0; i < adyacencia.size(); i++) {
            if (adyacencia[i].empty()) continue; 
            
            std::cout << idANodo[i] << ": ";

            //muestra todos los vecinos del vertice actual
            for (const auto& arista : adyacencia[i]) {
                std::cout << "(" << idANodo[arista.destino] << ", peso: " << arista.peso << ") ";
            }
            std::cout << std::endl;
        }
    }
};

#endif
