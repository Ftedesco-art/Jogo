#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "movimento.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 600
// Para LADO = 20, a janela é um grid de 60x30

typedef struct inimigo {
    int x;
    int y;
    int dx;
    int dy;
}TYPE_INIMIGO;

typedef struct recurso {
    int x;
    int y;
}TYPE_RECURSO;

int podeMover(int x, int y, int dx, int dy) {
    if ((x == 59 && dx == 1) || (x == 0 && dx == -1) ||
        (y == 29 && dy == 1) || (y == 0 && dy == -1)) {
        return 0; // Não pode mover
    }
    else {
        return 1; // Pode mover
    }
}

void move(int *x, int *y, int dx, int dy) {
    *x += dx;
    *y += dy;
}

int main(void)
{
    srand(time(NULL));

    int xPersonagem = rand() % 60; // Gera um número entre 0 e 59
    int yPersonagem = rand() % 30; // Gera um número entre 0 e 29

    InitWindow(LARGURA, ALTURA, "Página principal");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Movimentos do elemento
        if (IsKeyPressed(KEY_RIGHT) && podeMover(xPersonagem, yPersonagem, 1, 0)) {
            move(&xPersonagem, &yPersonagem, 1, 0);
        }

        if (IsKeyPressed(KEY_LEFT) && podeMover(xPersonagem, yPersonagem, -1, 0)) {
            move(&xPersonagem, &yPersonagem, -1, 0);
        }

        if (IsKeyPressed(KEY_UP) && podeMover(xPersonagem, yPersonagem, 0, -1)) {
            move(&xPersonagem, &yPersonagem, 0, -1);
        }

        if (IsKeyPressed(KEY_DOWN) && podeMover(xPersonagem, yPersonagem, 0, 1)) {
            move(&xPersonagem, &yPersonagem, 0, 1);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(xPersonagem * LADO, yPersonagem * LADO, LADO, LADO, RED);
        EndDrawing();
        //----------------------------------------------------------------------------------//
    }

    CloseWindow();
    return 0;
}
