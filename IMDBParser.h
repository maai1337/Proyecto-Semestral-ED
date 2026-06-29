#ifndef IMDB_PARSER_H
#define IMDB_PARSER_H

#include "Grafo.h"
#include <string>

/**
 * @brief Parsea el archivo CSV del dataset de IMDB para construir el grafo de actores.
 * @param nombreArchivo Ruta del archivo CSV a procesar.
 * @return Un objeto Grafo con vértices representados por strings (actores).
 */
Grafo<std::string> parsearDatasetIMDB(const std::string& nombreArchivo);

#endif
