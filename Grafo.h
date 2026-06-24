#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Grafo {
private:
    //Lista de adyacencia
    // Cada vertice tiene asociada una lista con sus vecinos 
    unordered_map<string, vector<string>> adyacencia;

    bool dirigido;

    int cantidadAristas;

public:
    //Constructor del grafo, por defecto crea un grafo no dirigido
    Grafo(bool esDirigido = false);

    void agregarVertice(const string& vertice);
    void agregarArista(const string& origen, const string& destino);
    
    vector<string> obtenerVecinos(const string& vertice) const;

    int obtenerCantidadVertices() const;
    int obtenerCantidadAristas() const;

    void cargarDesdeArchivo(const string& nombreArchivo);
    void mostrarGrafo() const;
};