#include "mapa.h"
#include <stdio.h>
#include "movimento.h"
#include "load.h"

char map[MAP_HEIGHT][MAP_WIDTH + 1]; // +1 para o caractere nulo

void LoadMap(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < MAP_HEIGHT; i++) {
        if (fgets(map[i], MAP_WIDTH + 2, file) == NULL) { // +2 para incluir o caractere de nova linha
            perror("Error reading line");
            fclose(file);
            return;
        }
        // Remove o caractere de nova linha, se presente
        if (map[i][MAP_WIDTH] == '\n') {
            map[i][MAP_WIDTH] = '\0';
        }
    }
    fclose(file);
}

void DrawMap() {
    for (int y = 0; y < MAP_HEIGHT + 6; y++) { // Adiciona 6 linhas em branco
        for (int x = 0; x < MAP_WIDTH; x++) {
            Color tileColor = RAYWHITE;
            Texture2D texture = {0}; // Inicializa como uma textura vazia

            if (y < 6) {
                tileColor = LIGHTGRAY;
            } else {
                char tile = map[y - 6][x];
                switch (tile) {
                    case 'W': texture = paredetexture; break;
                    case '.': texture = chaotexture; break;
                    case 'S': texture = basetexture; break;
                    case 'R': texture = recursotexture; break;
                    case 'H': texture = buracotexture; break;
                    case 'J': texture = spawntexture; break;
                    case 'B': texture = barricadatexture; break;
                    case 'K': texture = spawninimigotexture; break;
                    case 'M': texture = inimigotexture; break;
                    case '1': texture = minatexture; break;
                    case '2': texture = arqueirotexture; break;
                    case '3': texture = caminho3texture; break;
                    case '4': texture = caminho4texture; break;
                    case 'A': texture = bombatexture; break;
                    default: tileColor = GRAY; break;
                }
            }

            if (texture.id > 0) {
                // Se a textura foi corretamente carregada (id > 0), desenhe-a
                DrawTexture(texture, x * LADO, y * LADO, WHITE);
            } else {
                // Caso contrário, desenhe um retângulo com a cor de fundo
                DrawRectangle(x * LADO, y * LADO, LADO, LADO, tileColor);
            }
        }
    }
}

