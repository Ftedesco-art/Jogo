#include "mapa.h"
#include <stdio.h>
#include "movimento.h"

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

    int flag = 0;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            Color tileColor;
            switch (map[y][x])
            {
            case 'W':
                flag = 2;
                tileColor = GRAY;
                break;
            case '.':
                tileColor = (Color)
                {
                    47, 21, 54, 255
                }; // Espaço em branco
                break;
            case 'S':
                tileColor = YELLOW; // Base
                break;
            case 'R':
                tileColor = (Color)
                {
                    33, 179, 181, 255
                }; // Recurso
                break;
            case 'H':
                tileColor = GREEN; // Buraco
                break;
            case 'J':
                tileColor = (Color)
                {
                    47, 21, 54, 255
                }; // Jogador*
                break;
            case 'E':
                tileColor = (Color)
                {
                    47, 21, 54, 255
                }; // Inimigo*
                break;
            case 'T':
                tileColor = (Color)
                {
                    47, 21, 54, 255
                }; // Trap*
                break;
            case 'B':
                flag = 1;
                break;
            default:
                tileColor = (Color)
                {
                    47, 21, 54, 255
                };
                break;
            }

            ///////////////////// DESENHO DO MAPA /////////////////////

            if(flag == 0)
            {
                DrawRectangle(x * LADO, y * LADO, LADO, LADO, tileColor);
            }

            else if(flag == 2)
            {
                DrawTexture(paredetexture, x * LADO, y * LADO, WHITE);

            }

            else
            {
                DrawTexture(barricadatexture, x * LADO, y * LADO, WHITE);
            }

            flag = 0;
        }
    }
}

