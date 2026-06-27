#include "DatasetParser.h"
#include <iostream>

using namespace std;

int main() {
    // Codigo de prueba generado por IA

    cout << "Iniciando parseo del dataset..." << endl;
    
    Grafo<string> grafo = parsearDatasetRed("train_test_network.csv", false);

    cout << "\nParseo finalizado desde el main." << endl;
    cout << "Total de vertices en el grafo: " << grafo.obtenerCantidadVertices() << endl;
    cout << "Total de aristas en el grafo: " << grafo.obtenerCantidadAristas() << endl;

    cout << "\n--- Muestra de los primeros 5 nodos y sus conexiones ---" << endl;
    int limite = 5;
    for (int i = 0; i < grafo.obtenerCantidadVertices() && i < limite; ++i) {
        string ip_origen = grafo.obtenerNodo(i);
        cout << "Nodo Origen (IP): " << ip_origen << endl;
        
        const auto& vecinos = grafo.obtenerVecinos(ip_origen);
        for (const auto& arista : vecinos) {
            string ip_destino = grafo.obtenerNodo(arista.destino);
            cout << "  -> Conectado a (IP Destino): " << ip_destino 
                 << " | Peso (Tiempo total de conexion): " << arista.peso << " segs" << endl;
        }
    }

    return 0;
}
