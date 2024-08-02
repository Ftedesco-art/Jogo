#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "mapa.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 600
#define TEMPO_MENSAGEM 120 // 2 segundos (a 60 FPS)
#define MAX_INIMIGOS 10 // Número máximo de inimigos

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    bool isAlive;
} TYPE_INIMIGO;

// Função para verificar se é possível mover
int podeMover(int xElemento, int yElemento, int dx, int dy)
{
    if (map[yElemento + dy][xElemento + dx] == 'W' || (dx == 0 && dy == 0))
    {
        return 0; // Nao pode mover
    }
    else
    {
        return 1; // Pode mover
    }
}

// Função para mover um elemento
void move(int *x, int *y, int dx, int dy) {
    *x += dx;
    *y += dy;
}

Texture2D baseTexture;
Texture2D chaoTexture;
Texture2D paredeTexture;
Texture2D buracoTexture;
Texture2D recursoTexture;
Texture2D barricadaTexture;

// Função principal do jogo
int runJogo(char mapa[50], int xPersonagem, int yPersonagem, int NUM_INIMIGOS,
            char texturaBase[50], char texturaChao[50], char texturaParede[50], char texturaBuraco[50],
            char texturaRecurso[50], char texturaBarricada[50]) {
    srand(time(NULL));

    int i, recursos = 0;
    TYPE_INIMIGO array_inimigos[MAX_INIMIGOS];

    // Inicializa os sons
    InitAudioDevice();
    Sound fxWav = LoadSound("Sons\\Buraco.ogg");
    Sound fxWav2 = LoadSound("Sons\\Barricada.ogg");
    SetSoundVolume(fxWav, 0.1f);
    SetSoundVolume(fxWav2, 0.1f);

    LoadMap(mapa);

    // Inicializa os inimigos
    int contador = 0;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            if (map[j][i] == 'E' && contador < NUM_INIMIGOS) {
                array_inimigos[contador].x = i;
                array_inimigos[contador].y = j;
                array_inimigos[contador].dx = 0;
                array_inimigos[contador].dy = 0;
                array_inimigos[contador].isAlive = true;
                contador++;
            }
        }
    }

    InitWindow(LARGURA, ALTURA, "Pagina principal - Fase 1");
    SetTargetFPS(60);

    // Inicializa variavel de pause
    bool isPaused = false;

    // Inicializa texturas
    baseTexture = LoadTexture(texturaBase);
    chaoTexture = LoadTexture(texturaChao);
    paredeTexture = LoadTexture(texturaParede);
    buracoTexture = LoadTexture(texturaBuraco);
    recursoTexture = LoadTexture(texturaRecurso);
    barricadaTexture = LoadTexture(texturaBarricada);

    // Inicializa variaveis para a trap
    int contadorMovimentos = 0;
    int velocidadeAumentada = 5;
    int tempoTrap = 0;
    int trapAtivada = 0;

    // Inicializa variaveis para a mensagem
    int tempoMensagem = 0;
    int mensagemAtivada = 0;

    // Inicializa variaveis para o Game Over
    int vidaJogador = 3;
    int vidaDaBase = 3; // Vida inicial da base
    bool flagGameOver = false; // Indica se o jogo acabou
    int tempoGameOver = 0; // Tempo para exibir Game Over

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            isPaused = !isPaused;
        }

        if (!isPaused && !flagGameOver)
        {
            // Movimentação do personagem
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && podeMover(xPersonagem, yPersonagem, 1, 0))
            {
                move(&xPersonagem, &yPersonagem, 1, 0);
            }

            if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && podeMover(xPersonagem, yPersonagem, -1, 0))
            {
                move(&xPersonagem, &yPersonagem, -1, 0);
            }

            if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && podeMover(xPersonagem, yPersonagem, 0, -1))
            {
                move(&xPersonagem, &yPersonagem, 0, -1);
            }

            if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && podeMover(xPersonagem, yPersonagem, 0, 1))
            {
                move(&xPersonagem, &yPersonagem, 0, 1);
            }

            // Verifica se o jogador encostou em um recurso (R)
            if (map[yPersonagem][xPersonagem] == 'R')
            {
                recursos++;
                map[yPersonagem][xPersonagem] = '.'; // Remove o recurso do mapa
            }

            // Verifica se o jogador encostou em um buraco (H)
            if (map[yPersonagem][xPersonagem] == 'H')
            {
                PlaySound(fxWav);
            }

            // Verifica se o jogador encostou em uma armadilha (T)
            if (map[yPersonagem][xPersonagem] == 'T')
            {
                trapAtivada = 1; // Ativa a trap
                tempoTrap = 0; // Reinicia o contador de tempo
                mensagemAtivada = 1; // Ativa a mensagem
                tempoMensagem = 0; // Reinicia o contador da mensagem
            }

            if (trapAtivada)
            {
                tempoTrap++;
                if (tempoTrap >= 100)
                {
                    trapAtivada = 0; // Desativa a trap após 100 iterações
                }
            }

            // Incrementa o contador de movimentos
            contadorMovimentos++;

            if (contadorMovimentos % (trapAtivada ? 10 / velocidadeAumentada : 10) == 0)
            {
                for (i = 0; i < NUM_INIMIGOS; i++)
                {
                    // Verifica se o inimigo pode se mover na direção atual
                    if (!podeMover(array_inimigos[i].x, array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy) && array_inimigos[i].isAlive == true)
                    {
                        // Se não puder mover, gera novos valores para dx e dy
                        array_inimigos[i].dx = rand() % 3 - 1; // Pode ser -1, 0, ou 1
                        array_inimigos[i].dy = rand() % 3 - 1; // Pode ser -1, 0, ou 1
                    }
                    // Tenta mover o inimigo na direção atual
                    if (podeMover(array_inimigos[i].x, array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy) && array_inimigos[i].isAlive == true)
                    {
                        move(&array_inimigos[i].x, &array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy);
                    }
                }
            }

            // Atualiza a lógica da mensagem
            if (mensagemAtivada)
            {
                tempoMensagem++;
                if (tempoMensagem >= TEMPO_MENSAGEM)
                {
                    mensagemAtivada = 0; // Desativa a mensagem após 2 segundos
                }
            }

            for(i = 0; i < NUM_INIMIGOS; i++) {
                // Quando o inimigo toca na base
                if (map[array_inimigos[i].y][array_inimigos[i].x] == 'S' && array_inimigos[i].isAlive == true) {
                    // Mata o inimigo

                    array_inimigos[i].x = -1;
                    array_inimigos[i].y = -1;
                    array_inimigos[i].isAlive = false;

                    // Tira vida da base
                    vidaDaBase --;
                }

                // Quando o inimigo toca no jogador
                if (array_inimigos[i].y == yPersonagem && array_inimigos[i].x == xPersonagem && array_inimigos[i].isAlive == true) {
                    // Tira vida do jogador
                    vidaJogador --;
                }
            }

            // Verifica se a vida da base chegou a zero
            if (vidaDaBase <= 0 || vidaJogador <= 0)
            {
                flagGameOver = true; // Ativa o estado de Game Over
                tempoGameOver = 0; // Reinicia o contador de tempo do Game Over
            }
        }

        // Se o jogo está em estado de Game Over
        if (flagGameOver)
        {
            tempoGameOver++;
            if (tempoGameOver >= 120) // Mostra a mensagem por 2 segundos
            {
                CloseWindow(); // Fecha a janela após 2 segundos
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenho do mapa
        DrawMap();

        // Desenho do personagem
        DrawRectangle(xPersonagem * LADO, yPersonagem * LADO, LADO, LADO, WHITE);

        // Desenho das barricadas
        if(recursos >= 2 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed(KEY_B)) {
            PlaySound(fxWav2);
            map[yPersonagem][xPersonagem] = 'B'; // Constrói uma barricada no local do personagem
            recursos -= 2; // Reduz recursos
        }

        // Desenho dos inimigos
        for (i = 0; i < NUM_INIMIGOS; i++)
        {
            DrawRectangle(array_inimigos[i].x * LADO, array_inimigos[i].y * LADO, LADO, LADO, RED);
        }

        // Exibe a mensagem de Game Over
        if (flagGameOver)
        {
            // Desenha a sombra
            DrawText("GAME OVER!", LARGURA / 2 - MeasureText("GAME OVER!", 40) / 2 + 2, ALTURA / 2 - 10 + 2, 40, BLACK);
            // Desenha o texto principal
            DrawText("GAME OVER!", LARGURA / 2 - MeasureText("GAME OVER!", 40) / 2, ALTURA / 2 - 10, 40, RED);
        }

        // Exibe a mensagem sobre a armadilha
        if (mensagemAtivada)
        {
            DrawText("ARMADILHA ATIVADA!", LARGURA / 2 - MeasureText("ARMADILHA ATIVADA!", 20) / 2, ALTURA / 2 + 20, 20, BLACK);
        }

        if (isPaused)
        {
            Color textColor = DARKBLUE;

            // Desenha os textos centralizados
            DrawText("PAUSADO", LARGURA / 2 - MeasureText("PAUSADO", 40) / 2, ALTURA / 2 - 20 - 30, 40, textColor);
            DrawText("Pressione Q para Sair", LARGURA / 2 - MeasureText("Pressione Q para Sair", 20) / 2, ALTURA / 2 + 10 + 30, 20, textColor);

            // Se o usuário pressiona Q, sai do jogo
            if (IsKeyPressed('Q'))
            {
                return 0;
            }
        }

        // Desenho da posição do personagem
        char posText[50];
        sprintf(posText, "Pos: (%d, %d)", xPersonagem, yPersonagem);
        DrawText(posText, 5, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(posText, 5 + 1, 1, 20, (Color){200, 200, 200, 255}); // Texto cinza

        // Desenho das vidas do jogador
        char vidaJogadorText[50];
        sprintf(vidaJogadorText, "Vidas do jogador: %d", vidaJogador);
        DrawText(vidaJogadorText, 130, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(vidaJogadorText, 130 + 1, 1, 20, (Color){255, 255, 255, 255}); // Texto branco

        // Desenho   das vidas da base
        char vidaBaseText[50];
        sprintf(vidaBaseText, "Vidas da base: %d", vidaDaBase);
        DrawText(vidaBaseText, 350, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(vidaBaseText, 350 + 1, 1, 20, (Color){255, 255, 200, 255}); // Texto amarelo clara

        // Desenho do número de recursos
        char recursosText[50];
        sprintf(recursosText, "Recursos: %d", recursos);
        DrawText(recursosText, 545, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(recursosText, 545 + 1, 1, 20, (Color){200, 255, 255, 255}); // Texto azul clara

        EndDrawing();
    }

    // Libera as texturas
    UnloadTexture(baseTexture);
    UnloadTexture(chaoTexture);
    UnloadTexture(paredeTexture);
    UnloadTexture(buracoTexture);
    UnloadTexture(recursoTexture);
    UnloadTexture(barricadaTexture);
    CloseAudioDevice();

    return 0;
}

int main() {
    runJogo("mapa1.txt", 2, 2, 7, "Texturas\\Base1.png", "Texturas\\Chao1.png", "Texturas\\Parede1.png", "Texturas\\Buraco1.png", "Texturas\\Recurso1.png", "Texturas\\Barricada1.png"); // Fase 1

}
