#ifndef RED_PARSER_H
#define RED_PARSER_H

#include "Grafo.h"
#include <string>

/**
 * @brief Parsea el archivo CSV de red para construir el grafo de tráfico de IPs.
 * @param nombreArchivo Ruta del archivo CSV a procesar.
 * @param dirigido Indica si las aristas del grafo deben ser dirigidas o no.
 * @return Un objeto Grafo con vértices representados por strings (direcciones IP).
 */
Grafo<std::string> parsearDatasetRed(const std::string& nombreArchivo, bool dirigido = false);

#endif
