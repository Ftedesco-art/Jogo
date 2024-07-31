#include "mapa.h"
#include <stdio.h>

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
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            Color tileColor;
            switch (map[y][x])
            {
            case 'W':
                tileColor = GRAY; // Parede
                break;
            case '.':
                tileColor = LIGHTGRAY; // Espaço em branco
                break;
            case 'S':
                tileColor = BLUE; // Base
                break;
            case 'R':
                tileColor = PINK; // Recurso
                break;
            case 'H':
                tileColor = GREEN; // Buraco
                break;
            case 'J':
                tileColor = LIGHTGRAY; // Jogador*
                break;
            case 'E':
                tileColor = LIGHTGRAY; // Inimigo*
                break;
            case 'T':
                tileColor = LIGHTGRAY; // Trap
                break;
            default:
                tileColor = RAYWHITE;
                break;
            }
            DrawRectangle(x * LADO, y * LADO, LADO, LADO, tileColor);
        }
    }
}

