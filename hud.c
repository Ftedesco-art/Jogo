#include "hud.h"
#include "main.h"
#include "eventos.h"
#include "mapa.h"
#include "inimigo.h"
#include "load.h"

#include <stdio.h>
#include <string.h>

void reiniciarJogo(INIMIGO array_inimigos[MAX_INIMIGOS], JOGADOR *player,
                   BASE *base, BARRICADA barricadas[MAX_BARRICADAS],
                   int *numBarricadas, MINA minas[MAX_MINAS],
                   int *numMinas, ARQUEIRO arqueiros[MAX_ARQUEIROS],
                   int *numArqueiros, FLECHA flechas[MAX_FLECHAS],
                   int *numFlechas, int *numInimigos, int *recursos,
                   double *tempo) {
    // Carrega o mapa novamente
    LoadMap("mapa1.txt");

    // Reinicia os contadores
    *numInimigos = 0;
    *numBarricadas = 0;
    *numMinas = 0;
    *numArqueiros = 0;
    *numFlechas = 0;
    *recursos = 50;

    // Zera as estruturas
    for (int i = 0; i < MAX_BARRICADAS; i++) {
        memset(&barricadas[i], 0, sizeof(BARRICADA));
    }
    for (int i = 0; i < MAX_FLECHAS; i++) {
        memset(&flechas[i], 0, sizeof(FLECHA));
    }
    for (int i = 0; i < MAX_MINAS; i++) {
        memset(&minas[i], 0, sizeof(MINA));
    }
    for (int i = 0; i < MAX_ARQUEIROS; i++) {
        memset(&arqueiros[i], 0, sizeof(ARQUEIRO));
    }
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        memset(&array_inimigos[i], 0, sizeof(INIMIGO));
    }

    // Inicializa inimigos, base e player
    for (int mapY = 0; mapY < ALTURA_GRID; mapY++) {
        for (int mapX = 0; mapX < LARGURA_GRID; mapX++) {
            if (map[mapY][mapX] == 'M' && *numInimigos < MAX_INIMIGOS) {
                array_inimigos[*numInimigos] = (INIMIGO) {
                    .coord = {mapX, mapY + MAP_OFFSET},
                    .ultimaDirecao = {0, 0},
                    .ultimoMovimento = GetTime(),
                    .vida = 3
                };
                (*numInimigos)++; // Incrementa o número de inimigos
            } else if (map[mapY][mapX] == 'S') {
                base->coord.x = mapX;
                base->coord.y = mapY + MAP_OFFSET;
                base->vidas = 3;
            } else if (map[mapY][mapX] == 'J') {
                player->coord.x = mapX;
                player->coord.y = mapY + MAP_OFFSET;
                player->vidas = 3;
            }
        }
    }

    *tempo = 0;
}

void salva_dados(char mapa[ALTURA_GRID][LARGURA_GRID + 1], INIMIGO array_inimigos[MAX_INIMIGOS], JOGADOR *player,
                   BASE *base, BARRICADA barricadas[MAX_BARRICADAS], int numBarricadas,
                   MINA minas[MAX_MINAS], int numMinas, ARQUEIRO arqueiros[MAX_ARQUEIROS],
                   int numArqueiros, FLECHA flechas[MAX_FLECHAS], int numFlechas, int numInimigos, int recursos, double tempo) {
    int i;

    // Copia o mapa
    memcpy(info.map, mapa, sizeof(info.map));

    // Copia todas as informações dos inimigos
    for(i = 0; i < MAX_INIMIGOS; i++) {
        info.array_inimigos[i] = array_inimigos[i];
    }
    // Copia todas as informações dos posicionáveis
    for(i = 0; i < MAX_BARRICADAS; i++) {
            info.barricadas[i] = barricadas[i];
    }
    for(i = 0; i < MAX_FLECHAS; i++) {
            info.flechas[i] = flechas[i];
    }
    for(i = 0; i < MAX_MINAS; i++) {
            info.minas[i] = minas[i];
    }
    for(i = 0; i < MAX_ARQUEIROS; i++) {
            info.arqueiros[i] = arqueiros[i];
    }

    // Copia todas as informações de jogador
    info.player = *player;

    // Copia outras informações
    info.tempo = tempo;
    info.numArqueiros = numArqueiros;
    info.numBarricadas = numBarricadas;
    info.numFlechas = numFlechas;
    info.numInimigos = numInimigos;
    info.numMinas = numMinas;
    info.recursos = recursos;

    // Flag de que são dados salvos
    info.flag = 1;
}

void carrega_dados(char mapa[ALTURA_GRID][LARGURA_GRID + 1], INIMIGO array_inimigos[MAX_INIMIGOS], JOGADOR *player,
                   BASE *base, BARRICADA barricadas[MAX_BARRICADAS], int *numBarricadas,
                   MINA minas[MAX_MINAS], int *numMinas, ARQUEIRO arqueiros[MAX_ARQUEIROS],
                   int *numArqueiros, FLECHA flechas[MAX_FLECHAS], int *numFlechas,
                   int *numInimigos, int *recursos, double *tempo) {
    int i;

    // Copia o mapa
    memcpy(mapa, info.map, sizeof(mapa));

    // Copia todas as informações dos inimigos
    for(i = 0; i < MAX_INIMIGOS; i++) {
        array_inimigos[i] = info.array_inimigos[i];
    }

    // Copia todas as informações das features
    for(i = 0; i < 20; i++) {
        barricadas[i] = info.barricadas[i];
        flechas[i] = info.flechas[i];
        minas[i] = info.minas[i];
        arqueiros[i] = info.arqueiros[i];
    }

    // Copia todas as informações de jogador
    *player = info.player; // Alterado para usar o ponteiro

    *tempo = info.tempo; // Alterado para usar o ponteiro
    *numArqueiros = info.numArqueiros; // Alterado para usar o ponteiro
    *numBarricadas = info.numBarricadas; // Alterado para usar o ponteiro
    *numFlechas = info.numFlechas; // Alterado para usar o ponteiro
    *numInimigos = info.numInimigos; // Alterado para usar o ponteiro
    *numMinas = info.numMinas; // Alterado para usar o ponteiro
    *recursos = info.recursos; // Alterado para usar o ponteiro

    // Flag de que são dados salvos
    info.flag = 1;
}

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

void hud(void)
{
    int minutos = tempoAtual / 60; // 60 segundos em um minuto
    int segundos = tempoAtual % 60; // Resto para obter os segundos

    //TEXTOS DA HUD
    DrawText(TextFormat("x %d", recursos), 55, 80, 20, BLACK);
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


bool menu_inicial() {
    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("MENU PRINCIPAL", LARGURA / 2 - 70, ALTURA / 2 - 50, 20, BLACK);
        DrawText("Pressione N para Novo Jogo", LARGURA / 2 - 150, ALTURA / 2, 20, BLACK);
        DrawText("Pressione C para Carregar Jogo", LARGURA / 2 - 150, ALTURA / 2 + 30, 20, BLACK);
        DrawText("Pressione Q para Sair", LARGURA / 2 - 150, ALTURA / 2 + 60, 20, BLACK);
        EndDrawing();

        // Verifica as entradas do usuário
        if (IsKeyPressed(KEY_N)) {
            info.flag = 0; // Significa que o jogo é novo
            run();
            return true;
        } else if (IsKeyPressed(KEY_C)) {
            info.flag = 1; // Significa que o jogo é novo
            run();
            return true;
        } else if (IsKeyPressed(KEY_Q)) {
            return false;
        }
    }
}

bool menu_pause(bool* paused) {
    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("MENU PAUSE", LARGURA / 2 - 50, ALTURA / 2 - 50, 20, BLACK);
        DrawText("Pressione C para Continuar", LARGURA / 2 - 150, ALTURA / 2, 20, BLACK);
        DrawText("Pressione L para Carregar Jogo", LARGURA / 2 - 150, ALTURA / 2 + 30, 20, BLACK);
        DrawText("Pressione S para Salvar Jogo", LARGURA / 2 - 150, ALTURA / 2 + 60, 20, BLACK);
        DrawText("Pressione V para Voltar ao Menu Inicial", LARGURA / 2 - 150, ALTURA / 2 + 90, 20, BLACK);
        EndDrawing();

        if (IsKeyPressed(KEY_C)) {
            *paused = false; // Muda o estado de pausa para false
            return true; // Sai do menu de pausa
        } else if (IsKeyPressed(KEY_L)) {
            info.flag = 1;
            run();
            return true;
        } else if (IsKeyPressed(KEY_S)) {
            salva_jogo("jogo_salvo.bin", &info);
            return true;
        } else if (IsKeyPressed(KEY_V)) {
            *paused = false;
            return false; // Retorna ao menu inicial
        }
    }
}

// Função para salvar o estado do jogo em um arquivo binário
int salva_jogo(const char* arquivo, INFO* estado) {
    FILE *ponteiro = fopen(arquivo, "wb");
    if (ponteiro == NULL) {
        printf("\n\nErro na abertura do arquivo - escrita\n\n");
        return 0;
    }
    fwrite(estado, sizeof(INFO), 1, ponteiro);
    fclose(ponteiro);
    printf("\n\nEstado do jogo salvo com sucesso em '%s'.\n\n", arquivo);
    return 1;
}

// Função para ler o estado do jogo de um arquivo binário
int carrega_jogo(const char* arquivo, INFO* estado) {
    FILE *ponteiro = fopen(arquivo, "rb");
    if (ponteiro == NULL) {
        printf("\n\nErro na abertura do arquivo - leitura\n\n");
        return 0;
    }
    fread(estado, sizeof(INFO), 1, ponteiro);
    fclose(ponteiro);

    return 1;
}


