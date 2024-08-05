#include "eventos.h"
#include "main.h"
#include "mapa.h"
#include "inimigo.h"

#define MAX_FLECHAS 100
#define MAP_OFFSET 6

void atualizarFlechas(FLECHA flechas[], int *numFlechas, INIMIGO inimigos[], int *numInimigos)
{
    for (int i = 0; i < *numFlechas; i++)
    {
        if (flechas[i].ativa)
        {
            flechas[i].coord.x += flechas[i].direcao.x;
            flechas[i].coord.y += flechas[i].direcao.y;

            // Verifica se a flecha colidiu com algum inimigo
            for (int j = 0; j < *numInimigos; j++)
            {
                if (flechas[i].coord.x == inimigos[j].coord.x && flechas[i].coord.y == inimigos[j].coord.y)
                {
                    inimigos[j].vida--;
                    if (inimigos[j].vida <= 0)
                    {
                        map[inimigos[j].coord.y - MAP_OFFSET][inimigos[j].coord.x] = '.'; // Atualizar a posição no mapa para '.'
                        removerInimigo(inimigos, numInimigos, j);
                    }
                    flechas[i].ativa = 0;
                    break;
                }
            }

            // Verifica se a flecha saiu do mapa
            if (flechas[i].coord.x < 0 || flechas[i].coord.x >= LARGURA_GRID || flechas[i].coord.y < MAP_OFFSET || flechas[i].coord.y >= ALTURA_GRID + MAP_OFFSET)
            {
                flechas[i].ativa = 0;
            }
        }
    }
}

int encontrarInimigoMaisProximo(INIMIGO inimigos[], int numInimigos, int x, int y)
{
    int menorDistancia = 10000;
    int inimigoMaisProximo = -1;
    for (int i = 0; i < numInimigos; i++)
    {
        int distancia = abs(inimigos[i].coord.x - x) + abs(inimigos[i].coord.y - y);
        if (distancia < menorDistancia)
        {
            menorDistancia = distancia;
            inimigoMaisProximo = i;
        }
    }
    return inimigoMaisProximo;
}

void atirarFlecha(ARQUEIRO *arqueiro, FLECHA flechas[], int *numFlechas, INIMIGO inimigos[], int numInimigos)
{
    int inimigoMaisProximo = encontrarInimigoMaisProximo(inimigos, numInimigos, arqueiro->coord.x, arqueiro->coord.y);

    if (inimigoMaisProximo != -1 && *numFlechas < MAX_FLECHAS)
    {
        FLECHA *flecha = &flechas[*numFlechas];
        flecha->coord.x = arqueiro->coord.x;
        flecha->coord.y = arqueiro->coord.y;
        flecha->ativa = 1;
        int dx = inimigos[inimigoMaisProximo].coord.x - arqueiro->coord.x;
        int dy = inimigos[inimigoMaisProximo].coord.y - arqueiro->coord.y;
        if (dx != 0) flecha->direcao.x = dx / abs(dx);
        if (dy != 0) flecha->direcao.x = dy / abs(dy);
        (*numFlechas)++;
        arqueiro->ultimoTiro = GetTime();
    }
}

int podeMover(int xElemento, int yElemento, int dx, int dy)
{
    int newX = xElemento + dx;
    int newY = yElemento + dy;

    // Verificar se a nova posição está dentro dos limites do mapa
    if (newX < 0 || newX >= LARGURA_GRID || newY < MAP_OFFSET || newY >= ALTURA_GRID + MAP_OFFSET)
    {
        return 0; // Não pode mover
    }

    // Verificar se a nova posição é uma parede ou barricada
    if (map[newY - MAP_OFFSET][newX] == 'W' || map[newY - MAP_OFFSET][newX] == 'B' || map[newY - MAP_OFFSET][newX] == 'M')
    {
        return 0; // Não pode mover
    }

    return 1; // Pode mover
}

void move(int *x, int *y, int dx, int dy)
{
    if (podeMover(*x, *y, dx, dy))
    {
        *x += dx;
        *y += dy;
    }
}

