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
    int vida;
    double ultimoDano; // Tempo do �ltimo dano recebido
} BARRICADA;

typedef struct
{
    POSN coord;
    double ultimoRecurso; // Tempo do �ltimo recurso produzido
} MINA;

typedef struct
{
    POSN coord;
    double ultimoTiro; // Tempo do �ltimo tiro
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

#endif
