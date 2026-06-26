#include "Grafo.h"

using namespace std;

//Constructor de la clase Grafo
//Recibe si el grafo es dirigido o no
Grafo::Grafo(bool esDirigido) {
    dirigido = esDirigido;
    cantidadAristas = 0;
}

int Grafo::obtenerOAgregarId(const string& nodo) {
    auto it = nodoAId.find(nodo);
    if (it != nodoAId.end()) {
        return it->second;
    }
    int id = idANodo.size();
    nodoAId[nodo] = id;
    idANodo.push_back(nodo);
    adyacencia.push_back(vector<Arista>());
    return id;
}

//agrega vertice al grafo si todavia no existe
void Grafo::agregarVertice(const string& nodo) {
    obtenerOAgregarId(nodo);
}

//agrega una arista entre el vertice origen y el vertice destino
void Grafo::agregarArista(const string& origen, const string& destino, double peso) {
    int u = obtenerOAgregarId(origen);
    int v = obtenerOAgregarId(destino);

    bool encontradaU = false;
    for (auto& arista : adyacencia[u]) {
        if (arista.destino == v) {
            encontradaU = true;
            if (peso < arista.peso) {
                arista.peso = peso;
            }
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
                if (peso < arista.peso) arista.peso = peso;
                break;
            }
        }
    }
}

//retorna los vecinos de un vertice
//si el vertice no existe, retorna un vector vacio
const vector<Arista>& Grafo::obtenerVecinos(const string& nodo) const {
    auto it = nodoAId.find(nodo);
    if (it != nodoAId.end()) {
        return adyacencia[it->second];
    }
    static const vector<Arista> vacio; 
    return vacio;
}

const vector<Arista>& Grafo::obtenerVecinos(int id) const {
    if (id >= 0 && id < adyacencia.size()) {
        return adyacencia[id];
    }
    static const vector<Arista> vacio;
    return vacio;
}

int Grafo::obtenerCantidadVertices() const {
    return idANodo.size();
}

//retorna la cantidad de aristas del grafo
int Grafo::obtenerCantidadAristas() const {
    return cantidadAristas;
}

string Grafo::obtenerNodo(int id) const {
    if (id >= 0 && id < idANodo.size()) {
        return idANodo[id];
    }
    return "";
}

int Grafo::obtenerId(const string& nodo) const {
    auto it = nodoAId.find(nodo);
    if (it != nodoAId.end()) {
        return it->second;
    }
    return -1;
}

//Muestra el contenido del grafo usando la lista de adyacencia
void Grafo::mostrarGrafo() const {
    for (int i = 0; i < adyacencia.size(); i++) {
        if (adyacencia[i].empty()) continue; 
        
        cout << idANodo[i] << ": ";

        //muestra todos los vecinos del vertice actual
        for (const auto& arista : adyacencia[i]) {
            cout << "(" << idANodo[arista.destino] << ", peso: " << arista.peso << ") ";
        }
        cout << endl;
    }
}