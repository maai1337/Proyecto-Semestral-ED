#include "DatasetParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

Grafo<string> parsearDatasetRed(const string& nombreArchivo, bool dirigido) {
    Grafo<string> grafo(dirigido);

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
        
        string src_ip, dst_ip;
        string duration_str;

        int index = 0;
        while (getline(ss, campo, ',')) {
            if (index == 0) src_ip = campo;
            else if (index == 2) dst_ip = campo;
            else if (index == 6) duration_str = campo;
            index++;
        }

        if (index < 7) continue; // Si la línea está incompleta la ignoramos

        auto parseDuration = [](const string& str) -> double {
            if (str.empty() || str == "-") return 0.0;
            if (!isdigit(str[0]) && str[0] != '.') return 0.0;
            try {
                return stod(str);
            } catch (...) {
                return 0.0; 
            }
        };

        double peso = parseDuration(duration_str);
        
        // Si el peso es negativo por error en los datos, lo dejamos en 0. 
        // Ya no lo forzamos a 1 porque las duraciones pueden ser fracciones pequeñas (ej. 0.0001)
        if (peso < 0) peso = 0.0;

        grafo.agregarArista(src_ip, dst_ip, peso);
    }

    archivo.close();
    
    cout << "Dataset de red parseado exitosamente." << endl;
    cout << "Vertices encontrados: " << grafo.obtenerCantidadVertices() << endl;
    cout << "Aristas construidas: " << grafo.obtenerCantidadAristas() << endl;
    
    return grafo;
}
