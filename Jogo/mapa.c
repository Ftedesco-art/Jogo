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
    Texture2D barricadatexture = LoadTexture("Texturas\\barricada1.png");
    Texture2D paredetexture = LoadTexture("Texturas\\Parede.png");

    int flag = 0, flag2 = 0;
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
                tileColor = LIGHTGRAY;
                break;
            case 'S':
                tileColor = BLUE;
                break;
            case 'R':
                tileColor = PINK;
                break;
            case 'H':
                tileColor = BROWN;
                break;
            case 'J':
                tileColor = YELLOW;

                break;
            case 'B':
                flag = 1;


                break;
            default:
                tileColor = RAYWHITE;
                break;
            }
            if(flag==0)
            {
                DrawRectangle(x * LADO, y * LADO, LADO, LADO, tileColor);
            }
            else if(flag==2)
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
