#include "IMDBParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief Implementación de la función para leer y construir el grafo desde el archivo IMDB.
 * Lee las relaciones de co-actuación entre actores y sus correspondientes pesos.
 */
Grafo<string> parsearDatasetIMDB(const string& nombreArchivo) {
    Grafo<string> grafo(false); // Grafo no dirigido

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return grafo;
    }

    string linea;
    
    if (!getline(archivo, linea)) return grafo;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        
        string src, dst;
        string strength_str;

        int index = 0;
        while (getline(ss, campo, ',')) {
            if (index == 0) src = campo;
            else if (index == 1) dst = campo;
            else if (index == 2) strength_str = campo;
            index++;
        }

        if (index < 3) continue; 

        double peso = 1.0;
        if (!strength_str.empty()) {
            try {
                peso = stod(strength_str);
            } catch (...) {
                peso = 1.0; 
            }
        }

        grafo.agregarArista(src, dst, peso);
    }

    archivo.close();
    
    cout << "Dataset IMDB parseado exitosamente." << endl;
    cout << "Vertices encontrados: " << grafo.obtenerCantidadVertices() << endl;
    cout << "Aristas construidas: " << grafo.obtenerCantidadAristas() << endl;
    
    return grafo;
}
