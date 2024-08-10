#include <stdio.h>

#include "hud.h"
#include "main.h"
#include "eventos.h"
#include "mapa.h"
#include "load.h"

char map[ALTURA_GRID][LARGURA_GRID + 1]; // +1 para o caractere nulo

void LoadMap(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < ALTURA_GRID; i++)
    {
        if (fgets(map[i], LARGURA_GRID + 2, file) == NULL)   // +2 para incluir o caractere de nova linha
        {
            perror("Error reading line");
            fclose(file);
            return;
        }
        // Remove o caractere de nova linha, se presente
        if (map[i][LARGURA_GRID] == '\n')
        {
            map[i][LARGURA_GRID] = '\0';
        }
    }
    fclose(file);
}

void DrawMap()
{
    for (int y = 0; y < ALTURA_GRID + MAP_OFFSET; y++)   // Adiciona 6 linhas em branco
    {
        for (int x = 0; x < LARGURA_GRID; x++)
        {
            Color tileColor = RAYWHITE;
            Texture2D texture = {0}; // Inicializa como uma textura vazia

            if (y < 6)
            {
                tileColor = LIGHTGRAY;
            }
            else
            {
                char tile = map[y - 6][x];
                switch (tile)
                {
                case 'W':
                    texture = paredetexture;
                    break;
                case '.':
                    texture = chaotexture;
                    break;
                case 'S':
                    texture = basetexture;
                    break;
                case 'R':
                    texture = recursotexture;
                    break;
                case 'H':
                    texture = buracotexture;
                    break;
                case 'J':
                    texture = spawntexture;
                    break;
                case 'M':
                    texture = inimigotexture;
                    break;
                case '1':
                    texture = barricadatexture;
                    break;
                case '2':
                    texture = bombatexture;
                    break;
                case '3':
                    texture = minatexture;
                    break;
                case '4':
                    texture = arqueirotexture;
                    break;
                default:
                    texture = chaotexture;
                    break;
                }
            }

            if (texture.id > 0)
            {
                // Se a textura foi corretamente carregada (id > 0), desenhe-a
                DrawTexture(texture, x * LADO, y * LADO, WHITE);
            }
            else
            {
                // Caso contrário, desenhe um retângulo com a cor de fundo
                DrawRectangle(x * LADO, y * LADO, LADO, LADO, tileColor);
            }
        }
    }
}
