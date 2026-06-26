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

class Grafo {
private:
    std::unordered_map<std::string, int> nodoAId;
    std::vector<std::string> idANodo;
    
    //Lista de adyacencia
    // Cada vertice tiene asociada una lista con sus vecinos 
    std::vector<std::vector<Arista>> adyacencia;

    bool dirigido;

    int cantidadAristas;

    int obtenerOAgregarId(const std::string& nodo);

public:
    //Constructor del grafo, por defecto crea un grafo no dirigido
    Grafo(bool esDirigido = false);

    void agregarVertice(const std::string& nodo);
    void agregarArista(const std::string& origen, const std::string& destino, double peso = 1.0);

    const std::vector<Arista>& obtenerVecinos(const std::string& nodo) const;
    const std::vector<Arista>& obtenerVecinos(int id) const;

    int obtenerCantidadVertices() const;
    int obtenerCantidadAristas() const;
    
    std::string obtenerNodo(int id) const;
    int obtenerId(const std::string& nodo) const;

    void mostrarGrafo() const;
};

#endif
