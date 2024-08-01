// Versão 1 30/07 22:10 NINO
/*
CHANGELOG
-Sons
-Coleta de recursos
-Buracos
-HUD
*/
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

typedef struct recurso {
    int x;
    int y;
} TYPE_RECURSO;

int podeMover(int xElemento, int yElemento, int dx, int dy) {
    if (map[yElemento + dy][xElemento + dx] == 'W') {
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




int main(void) {

    int recursos = 11;
    int i, j, contador = 0;
    srand(time(NULL));
    //Inicialização dos sons
    InitAudioDevice();
    Sound fxWav = LoadSound("Sons\\Buraco.ogg"); // Load WAV audio file
     Sound fxWav2 = LoadSound("Sons\\Barricada.ogg"); // Load WAV audio file


    int xPersonagem = 11;
    int yPersonagem = 2;

    InitWindow(LARGURA, ALTURA, "Página principal");
     Texture2D barricadatexture = LoadTexture("Texturas\\barricada1.png");


    SetTargetFPS(60);
    TYPE_INIMIGO array_inimigos[NUM_INIMIGOS];
    //Carrega o Mapa
    LoadMap("mapa.txt");


    while (!WindowShouldClose()) {
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

        // Verifica se o jogador encostou em um recurso (R)
        if (map[yPersonagem][xPersonagem] == 'R') {
            recursos++;
            map[yPersonagem][xPersonagem] = '.'; // Remove o recurso do mapa
        }

        //Verifica se o jogador encostou em um buraco (H)
         if (map[yPersonagem][xPersonagem] == 'H')
            {

                PlaySound(fxWav);
                buraco(&xPersonagem, &yPersonagem);

            }






        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawMap();
          //Barricadas
        if(recursos>=2 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed('E'))
        {
            PlaySound(fxWav2);
            map[yPersonagem][xPersonagem] = 'B';

        }


        DrawRectangle(xPersonagem * LADO, yPersonagem * LADO, LADO, LADO, RED);

        // Desenha a HUD
        char positionText[50];
        sprintf(positionText, "Pos: (%d, %d)\nRecursos: %d", xPersonagem, yPersonagem, recursos);
        DrawText(positionText, 10, 10, 20, BLACK);

        EndDrawing();
    }

    UnloadSound(fxWav);
    UnloadSound(fxWav2);     // Unload sound data
    UnloadTexture(barricadatexture);




    CloseAudioDevice();     // Close audio device
    CloseWindow();


    return 0;
}
