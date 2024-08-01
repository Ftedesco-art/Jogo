// buraco.h
#ifndef BURACO_H
#define BURACO_H

#include "mapa.h"

#define MAX_BURACOS 2 // N�mero m�ximo de buracos

typedef struct buraco
{
    int bx;
    int by;
    int indice; // �ndice do par de teleporte
} TYPE_BURACO;

extern TYPE_BURACO array_buraco[MAX_BURACOS];

void InicializarBuracos();

#endif // BURACO_H
