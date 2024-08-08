#ifndef INIMIGO_H
#define INIMIGO_H

#include "eventos.h"

typedef struct
{
    POSN coord;
    POSN direcao;
    POSN ultimaDirecao;
    int vida;
    double ultimoMovimento;
} INIMIGO;

#endif
