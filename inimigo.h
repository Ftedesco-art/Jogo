#ifndef INIMIGO_H
#define INIMIGO_H

#include "eventos.h"

typedef struct
{
    int id;
    POSN coord;
    POSN direcao;
    POSN ultimaDirecao;
    int vida;
    double ultimoMovimento; // Tempo do último movimento

} INIMIGO;

#endif
