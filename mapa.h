#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 720
#define LARGURA_GRID 60
#define ALTURA_GRID 30
#define MAP_OFFSET 6

extern char map[ALTURA_GRID][LARGURA_GRID + 1]; // +1 para o caractere nulo
void LoadMap(const char *filename);
void DrawMap();

#endif // MAPA_H


