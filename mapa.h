#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

#define LADO 20
#define MAP_WIDTH 60
#define MAP_HEIGHT 30

extern Texture2D baseTexture;
extern Texture2D chaoTexture;
extern Texture2D paredeTexture;
extern Texture2D buracoTexture;
extern Texture2D recursoTexture;
extern Texture2D barricadaTexture;


extern char map[MAP_HEIGHT][MAP_WIDTH + 1]; // +1 para o caractere nulo
void LoadMap(const char *filename);
void DrawMap(void);

#endif // MAPA_H


