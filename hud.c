#include "mapa.h"
#include "load.h"
#include "eventos.h"


#include <stdio.h>
#include <string.h>

void drawBarraDeVidaInimigo(int x, int y, int vida)
{
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, RED);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, GREEN);
}

void drawBarraDeVidaBarricada(int x, int y, int vida)
{
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, BLUE);
}

void DrawHud(int recursos, int tempoAtual)
{
    int minutos = tempoAtual / 60;
    int segundos = tempoAtual % 60;

    //Desenho dos recursos
    DrawText(TextFormat("x %d", recursos), 55, 80, 20, BLACK);

    //Desenho do tempo
    char tempoString[20];
    snprintf(tempoString, sizeof(tempoString), "Tempo: %02d:%02d", minutos, segundos);
    DrawText(tempoString, 10, 5, 20, BLACK);
    DrawText("Wave", 15, 30, 20, BLACK);

    //TEXTURAS DA HUD
    DrawTexture(vidahudtexture, 20, 40, WHITE);
    DrawTexture(vidahudtexture, 40, 40, WHITE);
    DrawTexture(vidahudtexture, 60, 40, WHITE);
    DrawTexture(bombahudtexture, 200, 20, WHITE);
    DrawTexture(barricadahudtexture, 300, 20, WHITE);
    DrawTexture(minahudtexture, 400, 20, WHITE);
    DrawTexture(arqueirohudtexture, 500, 20, WHITE);
    DrawTexture(recursohudtexture, 30, 80, WHITE);
}

int DrawGeneral(int flagAcabou, int *timerAcabou, FLECHA *flechas, int numFlechas,
                 INIMIGO *array_inimigos, int numInimigos, BARRICADA *barricadas, int numBarricadas) {

    // Desenha os inimigos e suas barras de vida
    for (int j = 0; j < numInimigos; j++)
    {
        DrawTexture(inimigotexture, array_inimigos[j].coord.x * LADO, array_inimigos[j].coord.y * LADO, WHITE);
        drawBarraDeVidaInimigo(array_inimigos[j].coord.x, array_inimigos[j].coord.y, array_inimigos[j].vida);
    }

    // Desenha as barricadas e suas barras de vida
    for (int k = 0; k < numBarricadas; k++)
    {
        DrawTexture(barricadatexture, barricadas[k].coord.x * LADO, barricadas[k].coord.y * LADO, WHITE);
        drawBarraDeVidaBarricada(barricadas[k].coord.x, barricadas[k].coord.y, barricadas[k].vida);
    }

    // Desenha as flechas
    for (int f = 0; f < numFlechas; f++)
    {
        if (flechas[f].ativa)
        {
            if(flechas[f].direcao.x == 1) {// Direita
                DrawTexture(flechadireitatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            }
            else if (flechas[f].direcao.x == -1) {// Esquerda
                DrawTexture(flechaesquerdatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            }
            else if (flechas[f].direcao.y == 1) {// Baixo
                DrawTexture(flechacimatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            }
            else if (flechas[f].direcao.y == -1) {// Cima
                DrawTexture(flechabaixotexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            }
        }
    }

    // Tela de Game Over
    if (flagAcabou == 2) {
        (*timerAcabou)++;
        if (*timerAcabou < 120) { // Mostra a tela por 2 segundos (120 frames a 60 FPS)
            DrawText("VOCE PERDEU!", LARGURA / 2 - MeasureText("VOCE PERDEU!", 20) / 2, ALTURA / 2 - 10, 20, WHITE);
        } else {
            return 0;
        }
    }

    // Tela de Vitória
    else if (flagAcabou == 1) {
        (*timerAcabou)++;
        if (*timerAcabou < 120) { // Mostra a tela por 2 segundos (120 frames a 60 FPS)
            DrawText("VOCE VENCEU!", LARGURA / 2 - MeasureText("VOCE VENCEU!", 20) / 2, ALTURA / 2 - 10, 20, WHITE);
        } else {
            return 0;
        }
    }

    return 1;
}

