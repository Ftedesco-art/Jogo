#ifndef EVENTOS_H
#define EVENTOS_H

#include "mapa.h"

typedef struct
{
    int x;
    int y;
} POSN;

typedef struct
{
    POSN coord;
    int vidas;
} JOGADOR;

typedef struct
{
    POSN coord;
    int vidas;
} BASE;

typedef struct
{
    POSN coord;
    int vidaMax;
    int vidaAtual;
    double ultimoDano; // Tempo do �ltimo dano recebido
} BARRICADA;

typedef struct
{
    POSN coord;
    int flagDuracao;
    int timerBomba;
} BOMBA;

typedef struct
{
    POSN coord;
    double ultimoRecurso; // Tempo do �ltimo recurso produzido
    int freq;
} MINA;

typedef struct
{
    POSN coord;
    double ultimoTiro; // Tempo do �ltimo tiro
    float freq;
} ARQUEIRO;

typedef struct
{
    POSN coord;
    POSN direcao;
    int ativa;
} FLECHA;

typedef struct
{
    POSN coord;
} RECURSO;

typedef struct
{
    POSN coord;
    POSN direcao;
    POSN ultimaDirecao;
    int vida;
    double ultimoMovimento;
} INIMIGO;

typedef struct
{
    POSN coord;
    int id;
} BURACO;

typedef struct
{
    POSN spawn;
    int qtddInimigos;
} ROUND;

#endif
