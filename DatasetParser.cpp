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
        string src_bytes_str, dst_bytes_str;

        int index = 0;
        while (getline(ss, campo, ',')) {
            if (index == 0) src_ip = campo;
            else if (index == 2) dst_ip = campo;
            else if (index == 7) src_bytes_str = campo;
            else if (index == 8) dst_bytes_str = campo;
            index++;
        }

        if (index < 9) continue; // Si la línea está incompleta la ignoramos

        auto parseBytes = [](const string& str) -> long {
            if (str.empty() || str == "-") return 0;
            if (!isdigit(str[0])) return 0;
            try {
                return stol(str);
            } catch (...) {
                return 0; 
            }
        };

        long src_bytes = parseBytes(src_bytes_str);
        long dst_bytes = parseBytes(dst_bytes_str);

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
