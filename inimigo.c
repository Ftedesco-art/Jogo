#include "inimigo.h"

void moverInimigo(INIMIGO *inimigo, int objetivoX, int objetivoY, int *novoX, int *novoY)
{
    int movimentos[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int melhorDx = 0, melhorDy = 0;
    int menorDistancia = 10000;

    for (int i = 0; i < 4; i++)
    {
        int dx = movimentos[i][0];
        int dy = movimentos[i][1];
        int nextX = inimigo->coord.x + dx;
        int nextY = inimigo->coord.y + dy;

        if (podeMover(inimigo->coord.x, inimigo->coord.y, dx, dy) &&
                (dx != -inimigo->ultimaDirecao.x || dy != -inimigo->ultimaDirecao.y))
        {
            int distancia = abs(objetivoX - nextX) + abs(objetivoY - nextY);
            if (distancia < menorDistancia)
            {
                menorDistancia = distancia;
                melhorDx = dx;
                melhorDy = dy;
            }
        }
    }

    if (melhorDx != 0 || melhorDy != 0)
    {
        *novoX = inimigo->coord.x + melhorDx;
        *novoY = inimigo->coord.y + melhorDy;
        inimigo->ultimaDirecao.x = melhorDx;
        inimigo->ultimaDirecao.y = melhorDy;
    }
    else
    {
        *novoX = inimigo->coord.x;
        *novoY = inimigo->coord.y;
    }
}

void removerInimigo(INIMIGO inimigos[], int *numInimigos, int index)
{
    for (int i = index; i < *numInimigos - 1; i++)
    {
        inimigos[i] = inimigos[i + 1];
    }
    (*numInimigos)--;
}

int inimigoAoLado(INIMIGO inimigos[], int numInimigos, int x, int y)
{
    for (int i = 0; i < numInimigos; i++)
    {
        if ((inimigos[i].coord.x == x + 1 && inimigos[i].coord.y == y) ||
            (inimigos[i].coord.x == x - 1 && inimigos[i].coord.y == y) ||
            (inimigos[i].coord.x == x && inimigos[i].coord.y == y + 1) ||
            (inimigos[i].coord.x == x && inimigos[i].coord.y == y - 1))
        {
            return 1;
        }
    }
    return 0;
}

