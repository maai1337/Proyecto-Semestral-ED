#include "IMDBParser.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Iniciando parseo del dataset IMDB..." << endl;
    
    // Parseamos el dataset como un grafo no dirigido
    Grafo<string> grafo = parsearDatasetIMDB("imdb_edgelist.csv");

    cout << "\nParseo finalizado desde el main." << endl;
    cout << "Total de vertices (actores) en el grafo: " << grafo.obtenerCantidadVertices() << endl;
    cout << "Total de aristas (conexiones) en el grafo: " << grafo.obtenerCantidadAristas() << endl;

    cout << "\n--- Muestra de los primeros 5 actores y sus conexiones ---" << endl;
    int limite = 5;
    for (int i = 0; i < grafo.obtenerCantidadVertices() && i < limite; ++i) {
        string actor_origen = grafo.obtenerNodo(i);
        cout << "Actor Origen: " << actor_origen << endl;
        
        const auto& vecinos = grafo.obtenerVecinos(actor_origen);
        for (const auto& arista : vecinos) {
            string actor_destino = grafo.obtenerNodo(arista.destino);
            cout << "  -> Conectado a (Actor Destino): " << actor_destino 
                 << " | Peso (Strength): " << arista.peso << endl;
        }
    }

    return 0;
}
