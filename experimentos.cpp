#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include "Grafo.h"
#include "IMDBParser.h"
#include "RedParser.h"
#include "MetricasPrincipales.h"
#include "MetricasAdicionales.h"

using namespace std;
using namespace std::chrono;

/**
 * @brief Estima el uso de memoria ocupado por la estructura de datos del grafo.
 * Calcula el espacio aproximado de los mapas, vectores y listas de adyacencia.
 */
template <typename T>
size_t estimarMemoriaGrafo(const Grafo<T>& grafo) {
    size_t mem = sizeof(grafo);
    int V = grafo.obtenerCantidadVertices();
    int E = grafo.obtenerCantidadAristas();
    
    mem += V * sizeof(T); 
    mem += V * sizeof(int); 
    mem += V * sizeof(T);   
    mem += V * sizeof(std::vector<Arista>); 
    mem += E * sizeof(Arista) * 2; 
    return mem;
}

/**
 * @brief Estima el uso de memoria auxiliar (las estructuras internas de datos locales) de un algoritmo.
 * @param grafo Referencia al grafo para obtener el conteo de vértices y aristas.
 * @param metrica Nombre de la métrica (algoritmo) a evaluar.
 */
template <typename T>
size_t estimarMemoriaAlgoritmo(const Grafo<T>& grafo, const std::string& metrica) {
    int V = grafo.obtenerCantidadVertices();
    int E = grafo.obtenerCantidadAristas();
    
    if (metrica == "DegreeCentrality") {
        return sizeof(std::vector<double>) + V * sizeof(double);
    }
    else if (metrica == "ClosenessCentrality") {
        size_t memCloseness = sizeof(std::vector<double>) + V * sizeof(double);
        size_t memBFS = (sizeof(std::vector<double>) + V * sizeof(double)) + 
                        (sizeof(std::queue<int>) + V * sizeof(int));
        return memCloseness + memBFS;
    }
    else if (metrica == "PageRank") {
        return 2 * (sizeof(std::vector<double>) + V * sizeof(double));
    }
    else if (metrica == "EccentricityCentrality") {
        size_t memEcc = sizeof(std::vector<double>) + V * sizeof(double);
        size_t memBFS = (sizeof(std::vector<double>) + V * sizeof(double)) + 
                        (sizeof(std::queue<int>) + V * sizeof(int));
        return memEcc + memBFS;
    }
    else if (metrica == "HarmonicCentrality") {
        size_t memHarm = sizeof(std::vector<double>) + V * sizeof(double);
        size_t memBFS = (sizeof(std::vector<double>) + V * sizeof(double)) + 
                        (sizeof(std::queue<int>) + V * sizeof(int));
        return memHarm + memBFS;
    }
    else if (metrica == "AverageShortestPath") {
        size_t memBFS = (sizeof(std::vector<double>) + V * sizeof(double)) + 
                        (sizeof(std::queue<int>) + V * sizeof(int));
        return memBFS;
    }
    else if (metrica == "BetweennessCentrality") {
        size_t memBetweenness = sizeof(std::vector<double>) + V * sizeof(double);
        size_t memS = sizeof(std::stack<int>) + V * sizeof(int);
        size_t memP = sizeof(std::vector<std::vector<int>>) + V * sizeof(std::vector<int>) + E * sizeof(int);
        size_t memSigma = sizeof(std::vector<double>) + V * sizeof(double);
        size_t memD = sizeof(std::vector<int>) + V * sizeof(int);
        size_t memQ = sizeof(std::queue<int>) + V * sizeof(int);
        size_t memDelta = sizeof(std::vector<double>) + V * sizeof(double);
        return memBetweenness + memS + memP + memSigma + memD + memQ + memDelta;
    }
    return 0;
}

/**
 * @brief Ejecuta el algoritmo, mide el tiempo que toma y guarda los resultados junto con la memoria en el CSV.
 */
template<typename Func, typename T>
void medirYGuardarTiempo(Func f, const string& dataset, const string& escenario, const string& metrica, int repeticiones, ofstream& csv, const Grafo<T>& grafo) {
    size_t memGrafo = estimarMemoriaGrafo(grafo);
    size_t memAlgo = estimarMemoriaAlgoritmo(grafo, metrica);
    size_t memTotal = memGrafo + memAlgo;
    for (int i = 1; i <= repeticiones; i++) {
        auto start = high_resolution_clock::now();
        f();
        auto end = high_resolution_clock::now();
        duration<double, std::milli> diff = end - start;
        
        // Dataset,Escenario,Metrica,Iteracion,Tiempo_ms,Memoria_Total_Bytes
        csv << dataset << "," << escenario << "," << metrica << "," << i << "," << diff.count() << "," << memTotal << "\n";
    }
}

/**
 * @brief Corre toda la batería de experimentos (métricas en grafo original, al añadir arista y al quitar arista) para un dataset.
 */
void correrExperimentosDataset(const string& nombreDataset, const string& archivoDataset, Grafo<string> (*parser)(const string&), ofstream& csv) {
    cout << "========================================" << endl;
    cout << "Procesando dataset: " << nombreDataset << endl;
    
    auto start = high_resolution_clock::now();
    Grafo<string> grafo = parser(archivoDataset);
    auto end = high_resolution_clock::now();
    duration<double, std::milli> diffConstruccion = end - start;
    
    size_t memoria = estimarMemoriaGrafo(grafo);
    
    // Dataset, Escenario, Metrica, Iteracion, Tiempo_ms, Memoria_Bytes
    csv << nombreDataset << ",Construccion,N/A,1," << diffConstruccion.count() << "," << memoria << "\n";
    
    int repeticiones = 2;
    int V = grafo.obtenerCantidadVertices();
    
    auto calcDegree = [&]() { degreeCentrality(grafo); };
    auto calcCloseness = [&]() { closenessCentrality(grafo, false); };
    auto calcPageRank = [&]() { pageRank(grafo); };
    auto calcAveragePath = [&]() { averageShortestPath(grafo, false); };
    auto calcBetweenness = [&]() { betweennessCentrality(grafo); };
    auto calcEccentricity = [&]() { eccentricityCentrality(grafo, false); };
    auto calcHarmonic = [&]() { harmonicCentrality(grafo, false); };
    
    // 1. ORIGINAL
    cout << "Calculando metricas (Original)..." << endl;
    
    medirYGuardarTiempo(calcDegree, nombreDataset, "Original", "DegreeCentrality", repeticiones, csv, grafo);
    medirYGuardarTiempo(calcCloseness, nombreDataset, "Original", "ClosenessCentrality", repeticiones, csv, grafo);
    medirYGuardarTiempo(calcPageRank, nombreDataset, "Original", "PageRank", repeticiones, csv, grafo);
    medirYGuardarTiempo(calcEccentricity, nombreDataset, "Original", "EccentricityCentrality", repeticiones, csv, grafo);
    medirYGuardarTiempo(calcHarmonic, nombreDataset, "Original", "HarmonicCentrality", repeticiones, csv, grafo);
    medirYGuardarTiempo(calcAveragePath, nombreDataset, "Original", "AverageShortestPath", repeticiones, csv, grafo);
    medirYGuardarTiempo(calcBetweenness, nombreDataset, "Original", "BetweennessCentrality", repeticiones, csv, grafo);
    
    // 2. AÑADIR ARISTA
    if (V >= 2) {
        string n1 = grafo.obtenerNodo(0);
        string n2 = grafo.obtenerNodo(V/2); 
        cout << "Añadiendo arista entre " << n1 << " y " << n2 << "..." << endl;
        grafo.agregarArista(n1, n2);
        
        medirYGuardarTiempo(calcDegree, nombreDataset, "AddArista", "DegreeCentrality", repeticiones, csv, grafo);
        medirYGuardarTiempo(calcCloseness, nombreDataset, "AddArista", "ClosenessCentrality", repeticiones, csv, grafo);
        medirYGuardarTiempo(calcPageRank, nombreDataset, "AddArista", "PageRank", repeticiones, csv, grafo);
        medirYGuardarTiempo(calcEccentricity, nombreDataset, "AddArista", "EccentricityCentrality", repeticiones, csv, grafo);
        medirYGuardarTiempo(calcHarmonic, nombreDataset, "AddArista", "HarmonicCentrality", repeticiones, csv, grafo);
        medirYGuardarTiempo(calcAveragePath, nombreDataset, "AddArista", "AverageShortestPath", repeticiones, csv, grafo);
        medirYGuardarTiempo(calcBetweenness, nombreDataset, "AddArista", "BetweennessCentrality", repeticiones, csv, grafo);

        grafo.removerArista(n1, n2); // restaurar
    }
    
    // 3. QUITAR ARISTA
    if (V >= 1) {
        string n1 = grafo.obtenerNodo(0);
        const auto& vecinos = grafo.obtenerVecinos(n1);
        if (!vecinos.empty()) {
            string n2 = grafo.obtenerNodo(vecinos[0].destino);
            cout << "Quitando arista entre " << n1 << " y " << n2 << "..." << endl;
            grafo.removerArista(n1, n2);
            
            medirYGuardarTiempo(calcDegree, nombreDataset, "RemoveArista", "DegreeCentrality", repeticiones, csv, grafo);
            medirYGuardarTiempo(calcCloseness, nombreDataset, "RemoveArista", "ClosenessCentrality", repeticiones, csv, grafo);
            medirYGuardarTiempo(calcPageRank, nombreDataset, "RemoveArista", "PageRank", repeticiones, csv, grafo);
            medirYGuardarTiempo(calcEccentricity, nombreDataset, "RemoveArista", "EccentricityCentrality", repeticiones, csv, grafo);
            medirYGuardarTiempo(calcHarmonic, nombreDataset, "RemoveArista", "HarmonicCentrality", repeticiones, csv, grafo);
            medirYGuardarTiempo(calcAveragePath, nombreDataset, "RemoveArista", "AverageShortestPath", repeticiones, csv, grafo);
            medirYGuardarTiempo(calcBetweenness, nombreDataset, "RemoveArista", "BetweennessCentrality", repeticiones, csv, grafo);

            grafo.agregarArista(n1, n2); // restaurar
        }
    }
    cout << "Dataset " << nombreDataset << " finalizado." << endl;
}

/**
 * @brief Función adaptadora (wrapper) para cargar el dataset de red configurándolo como no dirigido.
 */
Grafo<string> parsearDatasetRedWrapper(const string& nombreArchivo) {
    return parsearDatasetRed(nombreArchivo, false);
}

/**
 * @brief Función de inicio que orquesta la creación del archivo de salida CSV y la ejecución de experimentos.
 */
int main() {
    ofstream csv("resultados_experimentos.csv");
    if (!csv.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        return 1;
    }
    
    csv << "Dataset,Escenario,Metrica,Iteracion,Tiempo_ms,Memoria_Total_Bytes\n";
    
    correrExperimentosDataset("IMDB", "imdb_edgelist.csv", parsearDatasetIMDB, csv);
    correrExperimentosDataset("Red", "train_test_network.csv", parsearDatasetRedWrapper, csv);
    
    csv.close();
    cout << "Experimentos completados exitosamente. Resultados guardados en 'resultados_experimentos.csv'" << endl;
    return 0;
}
