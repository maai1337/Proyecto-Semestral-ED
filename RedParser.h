#ifndef RED_PARSER_H
#define RED_PARSER_H

#include "Grafo.h"
#include <string>

Grafo<std::string> parsearDatasetRed(const std::string& nombreArchivo, bool dirigido = false);

#endif
