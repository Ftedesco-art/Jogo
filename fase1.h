// buraco.h
#ifndef BURACO_H
#define BURACO_H

#include "mapa.h"

#define MAX_BURACOS 2 // Número máximo de buracos

typedef struct buraco
{
    int bx;
    int by;
    int indice; // Índice do par de teleporte
} TYPE_BURACO;

extern TYPE_BURACO array_buraco[MAX_BURACOS];

void InicializarBuracos();

#endif // BURACO_H
