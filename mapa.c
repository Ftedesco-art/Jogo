#include "mapa.h"
#include <stdio.h>
#include "main.h"

char map[MAP_HEIGHT][MAP_WIDTH + 1]; // +1 para o caractere nulo

void LoadMap(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        if (fgets(map[i], MAP_WIDTH + 2, file) == NULL)   // +2 para incluir o caractere de nova linha
        {
            perror("Error reading line");
            fclose(file);
            return;
        }
        // Remove o caractere de nova linha, se presente
        if (map[i][MAP_WIDTH] == '\n')
        {
            map[i][MAP_WIDTH] = '\0';
        }
    }
    fclose(file);
}

void DrawMap()
{
    SetTraceLogLevel(LOG_NONE);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            ///////////////////// DESENHO DO MAPA /////////////////////

            switch (map[y][x])
            {
            case 'W':
                DrawTexture(paredeTexture, x * LADO, y * LADO, WHITE);
                break;
            case '.': // Chao
                DrawTexture(chaoTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'S': // Base
                DrawTexture(baseTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'R': // Recurso
                DrawTexture(recursoTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'H': // Buraco
                DrawTexture(buracoTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'J': // Jogador*
                DrawTexture(chaoTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'E': // Inimigo*
                DrawTexture(chaoTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'T': // Trap*
                DrawTexture(chaoTexture, x * LADO, y * LADO, WHITE);
                break;
            case 'B':
                DrawTexture(barricadaTexture, x * LADO, y * LADO, WHITE);
                break;
            default:
                DrawTexture(chaoTexture, x * LADO, y * LADO, WHITE);
                break;
            }

        }
    }
}

