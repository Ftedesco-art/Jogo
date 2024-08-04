#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

#define LADO 20
#define MAP_WIDTH 60
#define MAP_HEIGHT 37

extern char map[MAP_HEIGHT][MAP_WIDTH + 1]; // +1 para o caractere nulo

void LoadMap(const char *filename);
void DrawMap(void);


#endif // MAPA_H
