#ifndef UTIL_H
#define UTIL_H

#include "main.h"
#include "mapa.h"
#include "eventos.h"
#include "inimigo.h"

#include "raylib.h"

void DrawHealthBar(int x, int y, int vida);
void DrawBarricadeHealthBar(int x, int y, int vida);
void hud(void);
bool menu_inicial();
bool menu_pause(bool* paused);

typedef struct
{
    char map[ALTURA_GRID][LARGURA_GRID + 1];
    INIMIGO array_inimigos[MAX_INIMIGOS];
    JOGADOR player;
    BASE base;
    BARRICADA barricadas[MAX_BARRICADAS];
    MINA minas[MAX_MINAS];
    ARQUEIRO arqueiros[MAX_ARQUEIROS];
    FLECHA flechas[MAX_FLECHAS];
    double tempo;
    int recursos;
    int numInimigos;
    int numFlechas;
    int numArqueiros;
    int numMinas;
    int numBarricadas;
    int flag;
} INFO;

INFO info;

#endif // UTIL_H

