#ifndef HUD_H
#define HUD_H

#include "main.h"
#include "mapa.h"
#include "eventos.h"
#include "hud.h"
#include "raylib.h"

extern Font defaultFont;

void CarregaFont();
void DescarregaFont();
void drawBarraDeVidaBarricada(int x, int y, int vidaAtual, int vidaMax);
void drawBarraDeVidaInimigo(int x, int y, int vida);
void DrawHud(int recursos, int tempoAtual, BASE base, JOGADOR player);
void DrawGeneral(FLECHA *flechas, int numFlechas,
                INIMIGO *array_inimigos, int numInimigos, BARRICADA *barricadas, int numBarricadas);

typedef struct {
    char map[ALTURA_GRID][LARGURA_GRID + 1];
    INIMIGO array_inimigos[MAX_INIMIGOS];
    JOGADOR player;
    BASE base;
    BARRICADA barricadas[MAX_BARRICADAS];
    MINA minas[MAX_MINAS];
    ARQUEIRO arqueiros[MAX_ARQUEIROS];
    FLECHA flechas[MAX_FLECHAS];
    BOMBA bombas[MAX_BOMBAS];
    double tempo;
    int recursos;
    int numInimigos;
    int numFlechas;
    int numArqueiros;
    int numMinas;
    int numBarricadas;
    int numBombas;
    int flag;
} INFO;

INFO info;

#endif

