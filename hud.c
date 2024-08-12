#include "hud.h"
#include "mapa.h"
#include "load.h"

#include <stdio.h>
#include <string.h>

// Definindo a fonte padrão
Font defaultFont;

int End(int flagAcabou, float *timerAcabou) {
    // Tela de Game Over
    if (flagAcabou == 2) {
        (*timerAcabou)++;
        if (*timerAcabou < 120) { // Mostra a tela por 2 segundos (120 frames a 60 FPS)
            DrawTextEx(defaultFont, "VOCE PERDEU!", (Vector2){LARGURA / 2 - MeasureText("VOCE PERDEU!", 20) / 2, ALTURA / 2 - 10}, 20, 2, WHITE);
        } else {
            return 0;
        }
    }

    // Tela de Vitória
    else if (flagAcabou == 1) {
        (*timerAcabou)++;
        if (*timerAcabou < 120) { // Mostra a tela por 2 segundos (120 frames a 60 FPS)
            DrawTextEx(defaultFont, "VOCE VENCEU!", (Vector2){LARGURA / 2 - MeasureText("VOCE VENCEU!", 20) / 2, ALTURA / 2 - 10}, 20, 2, WHITE);
        } else {
            return 0;
        }
    }
    return 1;
}

void drawBarraDeVidaInimigo(int x, int y, int vida) {
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, RED);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, GREEN);
}

void drawBarraDeVidaBarricada(int x, int y, int vidaAtual, int vidaMax) {
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, barWidth * vidaAtual / vidaMax, barHeight, BLUE);
}

void DrawHud(int recursos, int tempoAtual, BASE base, JOGADOR player) {
    int minutos = tempoAtual / 60;
    int segundos = tempoAtual % 60;

    // Desenho dos recursos
    DrawTextEx(defaultFont, TextFormat("x%d", recursos), (Vector2){120, 70}, 20, 2, BLACK);

    // Desenho do tempo
    char tempoString[20];
    snprintf(tempoString, sizeof(tempoString), "Tempo: %02d:%02d", minutos, segundos);
    DrawTextEx(defaultFont, tempoString, (Vector2){10, 5}, 20, 2, BLACK);
    DrawTextEx(defaultFont, "Wave", (Vector2){15, 30}, 20, 2, BLACK);

    // TEXTURAS DA HUD
    switch(player.vidas) {
        case 3:
            DrawTexture(vidahudtexture, 05, 50, WHITE);
            DrawTexture(vidahudtexture, 25, 50, WHITE);
            DrawTexture(vidahudtexture, 45, 50, WHITE);
            break;
        case 2:
            DrawTexture(vidahudtexture, 05, 50, WHITE);
            DrawTexture(vidahudtexture, 25, 50, WHITE);
            DrawTexture(vidahudtexture, 45, 50, DARKGRAY);
            break;
        case 1:
            DrawTexture(vidahudtexture, 05, 50, WHITE);
            DrawTexture(vidahudtexture, 25, 50, DARKGRAY);
            DrawTexture(vidahudtexture, 45, 50, DARKGRAY);
            break;
    }

        // TEXTURAS DA HUD
    switch(base.vidas) {
        case 3:
            DrawTexture(vidabasehudtexture, 05, 75, WHITE);
            DrawTexture(vidabasehudtexture, 25, 75, WHITE);
            DrawTexture(vidabasehudtexture, 45, 75, WHITE);
            break;
        case 2:
            DrawTexture(vidabasehudtexture, 05, 75, WHITE);
            DrawTexture(vidabasehudtexture, 25, 75, WHITE);
            DrawTexture(vidabasehudtexture, 45, 75, DARKGRAY);
            break;
        case 1:
            DrawTexture(vidabasehudtexture, 05, 75, WHITE);
            DrawTexture(vidabasehudtexture, 25, 75, DARKGRAY);
            DrawTexture(vidabasehudtexture, 45, 75, DARKGRAY);
            break;
    }

    DrawTexture(bombahudtexture, 200, 20, WHITE);
    DrawTexture(barricadahudtexture, 300, 20, WHITE);
    DrawTexture(minahudtexture, 400, 20, WHITE);
    DrawTexture(arqueirohudtexture, 500, 20, WHITE);
    DrawTexture(recursohudtexture, 100, 70, WHITE);
}

void DrawGeneral(FLECHA *flechas, int numFlechas,
                INIMIGO *array_inimigos, int numInimigos, BARRICADA *barricadas, int numBarricadas) {

    // Desenha os inimigos e suas barras de vida
    for (int j = 0; j < numInimigos; j++) {
        DrawTexture(inimigotexture, array_inimigos[j].coord.x * LADO, array_inimigos[j].coord.y * LADO, WHITE);
        drawBarraDeVidaInimigo(array_inimigos[j].coord.x, array_inimigos[j].coord.y, array_inimigos[j].vida);
    }

    // Desenha as barricadas e suas barras de vida
    for (int k = 0; k < numBarricadas; k++) {
        DrawTexture(barricadatexture, barricadas[k].coord.x * LADO, barricadas[k].coord.y * LADO, WHITE);
        drawBarraDeVidaBarricada(barricadas[k].coord.x, barricadas[k].coord.y, barricadas[k].vidaAtual, barricadas[k].vidaMax);
    }

    // Desenha as flechas
    for (int f = 0; f < numFlechas; f++) {
        if (flechas[f].ativa) {
            if (flechas[f].direcao.x == 1) { // Direita
                DrawTexture(flechadireitatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            } else if (flechas[f].direcao.x == -1) { // Esquerda
                DrawTexture(flechaesquerdatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            } else if (flechas[f].direcao.y == 1) { // Baixo
                DrawTexture(flechacimatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            } else if (flechas[f].direcao.y == -1) { // Cima
                DrawTexture(flechabaixotexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            }
        }
    }
}

void CarregaFont() {
    defaultFont = LoadFont("Fonts/FantasqueSansMono-Regular.otf");
}

void DescarregaFont() {
    UnloadFont(defaultFont); // Liberar a fonte
}

int upgradeMenu(int xClique, int yClique, int xCliqueNovo, int yCliqueNovo, int numBarricadas, int numBombas, int numMinas, int numArqueiros,
                BARRICADA barricadas[MAX_BARRICADAS], BOMBA bombas[MAX_BOMBAS], MINA minas[MAX_MINAS], ARQUEIRO arqueiros[MAX_ARQUEIROS], int *recursos) {

    char tile = map[yClique][xClique];
    int i;

    // Definindo as coordenadas dos botões
    int botaoFecharX1 = 1000; // Vértice superior esquerdo X do botão de fechar
    int botaoFecharY1 = 20;    // Vértice superior esquerdo Y do botão de fechar
    int botaoFecharX2 = 1080;  // Vértice inferior direito X do botão de fechar
    int botaoFecharY2 = 100;    // Vértice inferior direito Y do botão de fechar

    int botaoCompraX1 = 1080;   // Vértice superior esquerdo X do botão de compra
    int botaoCompraY1 = 20;      // Vértice superior esquerdo Y do botão de compra
    int botaoCompraX2 = 1160;    // Vértice inferior direito X do botão de compra
    int botaoCompraY2 = 100;      // Vértice inferior direito Y do botão de compra

    printf("Clique 1 em: %d %d, tile %c\n", xClique, yClique, tile); // Mensagem de clique
    printf("Clique 2 em: %d %d\n", xCliqueNovo, yCliqueNovo); // Mensagem de clique

    // Verifica se o clique foi no botão de compra
    if (xCliqueNovo >= botaoCompraX1 && xCliqueNovo <= botaoCompraX2 &&
        yCliqueNovo >= botaoCompraY1 && yCliqueNovo <= botaoCompraY2) {

        switch(tile) {
            case '1':
                if(*recursos >= 3) {
                    for(i = 0; i < numBarricadas; i++) {
                        if (barricadas[i].coord.x == xClique && barricadas[i].coord.y == yClique + MAP_OFFSET && barricadas[i].vidaMax != 15) {
                            printf("Atualizando barricada na posição (%d, %d)\n", xClique, yClique);
                            barricadas[i].vidaMax = 15;
                            barricadas[i].vidaAtual = 15;
                            *recursos -= 3;
                            printf("Recursos restantes: %d\n", *recursos);
                            return 1; // Retorna 1 para fechar o menu
                        }
                    }
                }
                break;
            case '2':
                if(*recursos >= 2) {
                    for(i = 0; i < numBombas; i++) {
                        if (bombas[i].coord.x == xClique && bombas[i].coord.y == yClique + MAP_OFFSET&& bombas[i].flagDuracao != 1) {
                            printf("Atualizando bomba na posição (%d, %d)\n", xClique, yClique);
                            bombas[i].flagDuracao = 1;
                            *recursos -= 2;
                            printf("Recursos restantes: %d\n", *recursos);
                            return 1; // Retorna 1 para fechar o menu
                        }
                    }
                }
                break;
            case '3':
                if(*recursos >= 6) {
                    for(i = 0; i < numMinas; i++) {
                        if (minas[i].coord.x == xClique && minas[i].coord.y == yClique + MAP_OFFSET && minas[i].freq != 5) {
                            printf("Atualizando mina na posição (%d, %d)\n", xClique, yClique);
                            minas[i].freq = 5;
                            *recursos -= 6;
                            printf("Recursos restantes: %d\n", *recursos);
                            return 1; // Retorna 1 para fechar o menu
                        }
                    }
                }
                break;
            case '4':
                if(*recursos >= 3) {
                    for(i = 0; i < numArqueiros; i++) {
                        if (arqueiros[i].coord.x == xClique && arqueiros[i].coord.y == yClique + MAP_OFFSET && arqueiros[i].freq != 1) {
                            printf("Atualizando arqueiro na posição (%d, %d)\n", xClique, yClique);
                            arqueiros[i].freq = 1;
                            *recursos -= 3;
                            printf("Recursos restantes: %d\n", *recursos);
                            return 1; // Retorna 1 para fechar o menu
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    else if (xCliqueNovo >= botaoFecharX1 && xCliqueNovo <= botaoFecharX2 &&
        yCliqueNovo >= botaoFecharY1 && yCliqueNovo <= botaoFecharY2) {
        printf("Clique no botão de fechar!\n"); // Mensagem de clique no botão de fechar
        return 0; // Retorna 0 para fechar o menu
    }

    return -1; // Retorna -1 para indicar que o menu deve permanecer aberto
}
