#include "raylib.h"
#include <stdio.h>

#define TILE_SIZE 32
#define MAP_WIDTH 34
#define MAP_HEIGHT 14

char map[MAP_HEIGHT][MAP_WIDTH + 1]; // +1 para o caractere nulo
/*CARREGA O MAPA */
void LoadMap(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < MAP_HEIGHT; i++) {
        fgets(map[i], MAP_WIDTH + 1, file); // +1 para ler o caractere nulo
        fgetc(file); // Pular o newline
    }
    fclose(file);
}
/*DESENHA O MAPA */
void DrawMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == '#') {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);
            } else if (map[y][x] == '.') {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
            }
              else if (map[y][x] == 'S')
              {
                  DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
              }
              else if (map[y][x] == 'H')
              {
                  DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BROWN);
              }
              else if (map[y][x] == 'R')
              {
                  DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, PINK);
              }
        }
    }
}

int main(void) {
    // Inicializa a janela
    const int screenWidth = 1200;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib - Map Example");

    // Carrega o mapa do arquivo de texto
    LoadMap("mapa.txt");

    SetTargetFPS(60);

    // Loop do jogo
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawMap();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
