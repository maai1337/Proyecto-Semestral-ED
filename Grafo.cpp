#include "Grafo.h"

//Constructor de la clase Graof
//Recibe si el grafo es dirigido o no
Grafo::Grafo(bool esDirigido){
    dirigido = esDirigido;
    cantidadAristas = 0;
}

//agrega vertice al grafo si todavia no existe
void Grafo::agregarVertice(const string& vertice) {
    if (adyacencia.find(vertice) == adyacencia.end()) {
        adyacencia[vertice] = vector<string>();
    }
}

//agrega una arusta entre el vertic origen y el vertice destino
void Grafo::agregarArista(const string& origen, const string& destino) {
    agregarVertice(origen);
    agregarVertice(destino);

    adyacencia[origen].push_back(destino);

    if (!dirigido) {
        adyacencia[destino].push_back(origen);
    }

    cantidadAristas++;
}

//retorna los vecinos de un vertice
//si el vertice no existe, retorna un vector vacio
vector<string> Grafo::obtenerVecinos(const string& vertice) const {
    if (adyacencia.find(vertice) != adyacencia.end()) {
        return adyacencia.at(vertice);
    }
    return vector<string>();
}

//retorna la cantidad de aristas del grafo
int Grafo::obtenerCantidadVertices() const {
    return adyacencia.size();
}

//retorna la cantidad de aristas del grafo
int Grafo::obtenerCantidadAristas() const {
    return cantidadAristas;
}

//Carga un grafo desde un archivo
void Grafo::cargarDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error" << nombreArchivo << endl;
        return;
    }

    string origen, destino;

    while (archivo >> origen >> destino) {
        agregarArista(origen, destino);
    }

    archivo.close();

}

//Muestra el contenido del grafo usando la lista de adyacencia
void Grafo::mostrarGrafo() const {
    for (const auto& par : adyacencia) {
        cout << par.first << ": ";

        //muestra todos los vecinos del vertice actual
        for (const string& vecino : par.second) {
            cout << vecino << " ";
        }

        cout << endl;
    }
}
 