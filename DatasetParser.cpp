#include "DatasetParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

Grafo parsearDatasetRed(const string& nombreArchivo, bool dirigido) {
    Grafo grafo(dirigido);

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
        vector<string> columnas;

        while (getline(ss, campo, ',')) {
            columnas.push_back(campo);
        }

        if (columnas.size() < 43) continue;

        string src_ip = columnas[0];
        string dst_ip = columnas[2];

        long src_bytes = columnas[7].empty() ? 0 : stol(columnas[7]);
        long dst_bytes = columnas[8].empty() ? 0 : stol(columnas[8]);

        double peso = src_bytes + dst_bytes;
        if (peso <= 0) peso = 1;

        grafo.agregarArista(src_ip, dst_ip, peso);
    }

    archivo.close();
    
    cout << "Dataset de red parseado exitosamente." << endl;
    cout << "Vertices encontrados: " << grafo.obtenerCantidadVertices() << endl;
    cout << "Aristas construidas: " << grafo.obtenerCantidadAristas() << endl;
    
    return grafo;
}
