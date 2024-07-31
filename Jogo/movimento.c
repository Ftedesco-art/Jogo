#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "movimento.h"
#include "mapa.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 600
#define NUM_INIMIGOS 7
#define TEMPO_MENSAGEM 120 // 2 segundos (a 60 FPS)

typedef struct inimigo {
    int x;
    int y;
    int dx;
    int dy;
} TYPE_INIMIGO;

int podeMover(int xElemento, int yElemento, int dx, int dy)
{
    if (map[yElemento + dy][xElemento + dx] == 'W')
    {
        return 0; // Não pode mover
    }
    else
    {
        return 1; // Pode mover
    }
}

void move(int *x, int *y, int dx, int dy)
{
    *x += dx;
    *y += dy;
}

int main(void)
{
    srand(time(NULL));

    int yPersonagem = 2;
    int xPersonagem = 11;
    int i, j, contador = 0;

    TYPE_INIMIGO array_inimigos[NUM_INIMIGOS];

    LoadMap("mapa.txt");

    for (i = 0; i < MAP_WIDTH; i++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            if (map[j][i] == 'E') {
                array_inimigos[contador].x = i;
                array_inimigos[contador].y = j;
                contador++;
            }
        }
    }

    InitWindow(LARGURA, ALTURA, "Página principal");
    SetTargetFPS(60);

    int contadorMovimentos = 0;

    int velocidadeNormal = 1;
    int velocidadeAumentada = 5;
    int tempoTrap = 0;
    int trapAtivada = 0;

    // Variáveis para a mensagem
    int tempoMensagem = 0;
    int mensagemAtivada = 0;

    while (!WindowShouldClose())
    {
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

        // Verifica a armadilha
        if (map[yPersonagem][xPersonagem] == 'T') {
            trapAtivada = 1; // Ativa a trap
            tempoTrap = 0; // Reinicia o contador de tempo

            // Ativa a mensagem
            mensagemAtivada = 1;
            tempoMensagem = 0; // Reinicia o contador da mensagem
        }

        if (trapAtivada) {
            tempoTrap++;
            if (tempoTrap >= 100) {
                trapAtivada = 0; // Desativa a trap após 100 iterações
            }
        }

        // Incrementa o contador de movimentos
        contadorMovimentos++;

        if (contadorMovimentos % (trapAtivada ? 10 / velocidadeAumentada : 10) == 0) {
            for (i = 0; i < NUM_INIMIGOS; i++) {
                array_inimigos[i].dx = rand() % 3 - 1;
                array_inimigos[i].dy = rand() % 3 - 1;

                if (podeMover(array_inimigos[i].x, array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy)) {
                    move(&array_inimigos[i].x, &array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy);
                }
            }
        }

        // Atualiza a lógica da mensagem
        if (mensagemAtivada) {
            tempoMensagem++;
            if (tempoMensagem >= TEMPO_MENSAGEM) {
                mensagemAtivada = 0; // Desativa a mensagem após 3 segundos
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMap(); // Desenha o mapa
        DrawRectangle(xPersonagem * LADO, yPersonagem * LADO, LADO, LADO, YELLOW); // Desenha o personagem

        for (i = 0; i < NUM_INIMIGOS; i++) {
            DrawRectangle(array_inimigos[i].x * LADO, array_inimigos[i].y * LADO, LADO, LADO, RED); // Desenha os inimigos
        }

        char positionText[50];
        sprintf(positionText, "Pos: (%d, %d)", xPersonagem, yPersonagem);
        DrawText(positionText, 5, 5, 20, BLACK); // Desenha o texto da posição do personagem

        // Desenha a mensagem se ativada
        if (mensagemAtivada) {
            const char *mensagem1 = "Voce caiu na armadilha!";
            int larguraMensagem1 = MeasureText(mensagem1, 40);
            DrawText(mensagem1, (LARGURA - larguraMensagem1) / 2, ALTURA / 2 - 30, 40, RED);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
