#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "mapa.h"
#include "load.h"
#include "hud.h"
#include "inimigo.h"
#include "eventos.h"

#define MAX_INIMIGOS 7
#define MAP_OFFSET 6  // Número de linhas em branco no topo
#define MAX_BARRICADAS 100 // Número máximo de barricadas
#define MAX_MINAS 100 // Número máximo de minas
#define MAX_ARQUEIROS 100 // Número máximo de arqueiros
#define MAX_FLECHAS 100 // Número máximo de flechas

int recursos = 11;

int main(void)
{
    srand(time(NULL));

    // Inicialização dos sons
    InitAudioDevice();
    Sound fxWav = LoadSound("Sons\\Buraco.ogg");
    Sound fxWav2 = LoadSound("Sons\\Barricada.ogg");
    SetSoundVolume(fxWav, 0.1f);
    SetSoundVolume(fxWav2, 0.1f);

    InitWindow(LARGURA, ALTURA, "Página principal");
    CarregarTexturas();
    SetTargetFPS(60);

    LoadMap("mapa1.txt");

    int id = 0, i = 0;
    INIMIGO array_inimigos[MAX_INIMIGOS];
    PLAYER player;

    // Inicializa inimigos e base
    int objetivoX = -1, objetivoY = -1;
    for (int mapY = 0; mapY < ALTURA_GRID; mapY++)
    {
        for (int mapX = 0; mapX < LARGURA_GRID; mapX++)
        {
            if (map[mapY][mapX] == 'M' && i < MAX_INIMIGOS)
            {
                array_inimigos[i] = (INIMIGO){ .coord = {mapX, mapY + MAP_OFFSET}, .ultimaDirecao = {0, 0}, .ultimoMovimento = GetTime(), .vida = 3};
                i++;
            }
            else if (map[mapY][mapX] == 'S')
            {
                objetivoX = mapX;
                objetivoY = mapY + MAP_OFFSET;
            }
            else if (map[mapY][mapX] == 'J')
            {
                player.coord.x = mapX;
                player.coord.y = mapY + MAP_OFFSET;
            }
        }
    }

    int numBarricadas = 0;
    BARRICADA barricadas[MAX_BARRICADAS];

    int numMinas = 0;
    MINA minas[MAX_MINAS];

    int numArqueiros = 0;
    ARQUEIRO arqueiros[MAX_ARQUEIROS];

    int numFlechas = 0;
    FLECHA flechas[MAX_FLECHAS] = {0};

    while (!WindowShouldClose())
    {
        double tempoAtual = GetTime();

        // Movimentação do personagem
        if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)))
        {
            move(&player.coord.x, &player.coord.y, 1, 0);
        }

        if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)))
        {
            move(&player.coord.x, &player.coord.y, -1, 0);
        }

        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
        {
            move(&player.coord.x, &player.coord.y, 0, -1);
        }

        if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)))
        {
            move(&player.coord.x, &player.coord.y, 0, 1);
        }

        int mapX = player.coord.x;
        int mapY = player.coord.y - MAP_OFFSET;

        // Verifica se o jogador encostou em um recurso (R)
        if (map[mapY][mapX] == 'R')
        {
            recursos++;
            map[mapY][mapX] = '.'; // Remove o recurso do mapa
        }

        // Verifica se o jogador encostou em um buraco (H)
        if (map[mapY][mapX] == 'H')
        {
            PlaySound(fxWav);
        }

        // Barricadas
        if (recursos >= 2 && map[mapY][mapX] == '.' && IsKeyPressed('E') && numBarricadas < MAX_BARRICADAS)
        {
            PlaySound(fxWav2);
            map[mapY][mapX] = 'B';
            barricadas[numBarricadas].coord.x = mapX;
            barricadas[numBarricadas].coord.y = mapY + MAP_OFFSET;
            barricadas[numBarricadas].vida = 3; // Inicializa a vida da barricada
            barricadas[numBarricadas].ultimoDano = tempoAtual; // Inicializa o tempo do último dano
            numBarricadas++;
        }

        // Bomba
        if (recursos >= 1 && map[mapY][mapX] == '.' && IsKeyPressed('G'))
        {
            PlaySound(fxWav2);
            map[mapY][mapX] = 'A';
        }

        // Mina
        if (recursos >= 5 && map[mapY][mapX] == '.' && IsKeyPressed('R') && numMinas < MAX_MINAS)
        {
            PlaySound(fxWav2);
            map[mapY][mapX] = '1';
            minas[numMinas].coord.x = mapX;
            minas[numMinas].coord.y = mapY + MAP_OFFSET;
            minas[numMinas].ultimoRecurso = tempoAtual; // Inicializa o tempo do último recurso produzido
            numMinas++;
        }

        // Arqueiro
        if (recursos >= 5 && map[mapY][mapX] == '.' && IsKeyPressed('F') && numArqueiros < MAX_ARQUEIROS)
        {
            PlaySound(fxWav2);
            map[mapY][mapX] = '2';
            arqueiros[numArqueiros].coord.x = mapX;
            arqueiros[numArqueiros].coord.y = mapY + MAP_OFFSET;
            arqueiros[numArqueiros].ultimoTiro = tempoAtual; // Inicializa o tempo do último tiro
            numArqueiros++;
        }

        int novosX[MAX_INIMIGOS];
        int novosY[MAX_INIMIGOS];

        for (int j = 0; j < i; j++)
        {
            if (tempoAtual - array_inimigos[j].ultimoMovimento >= 0.1)
            {
                moverInimigo(&array_inimigos[j], objetivoX, objetivoY, &novosX[j], &novosY[j]);
                array_inimigos[j].ultimoMovimento = tempoAtual; // Atualiza o tempo do último movimento

                char tile = map[novosY[j] - MAP_OFFSET][novosX[j]];
                if (tile == 'B')
                {
                    // Encontra a barricada correspondente
                    for (int k = 0; k < numBarricadas; k++)
                    {
                        if (barricadas[k].coord.x == novosX[j] && barricadas[k].coord.y == novosY[j])
                        {
                            barricadas[k].vida--;
                            if (barricadas[k].vida <= 0)
                            {
                                map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualizar a posição no mapa para '.'
                                for (int l = k; l < numBarricadas - 1; l++)
                                {
                                    barricadas[l] = barricadas[l + 1];
                                }
                                numBarricadas--;
                            }
                            break;
                        }
                    }
                }
                else if ((novosX[j] == objetivoX && novosY[j] == objetivoY) || tile == 'A')
                {
                    // Inimigo chegou à base 'S' ou encontrou uma bomba e deve ser removido
                    map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualizar a posição no mapa para '.'
                    map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpar a posição antiga no mapa
                    removerInimigo(array_inimigos, &i, j);
                    j--; // Ajustar índice após remoção
                }

                else if (array_inimigos[j].vida <= 0)
                {
                    // Inimigo morreu por causa da vida zerada
                    map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpar a posição antiga no mapa
                    removerInimigo(array_inimigos, &i, j);
                    j--; // Ajustar índice após remoção
                }
                else if (tile == '.')
                {
                    map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpar a posição antiga no mapa
                    array_inimigos[j].coord.x = novosX[j];
                    array_inimigos[j].coord.y = novosY[j];
                    map[novosY[j] - MAP_OFFSET][novosX[j]] = 'M'; // Atualizar a posição no mapa
                }
            }
        }

        // Verificar se há inimigos ao lado das barricadas e aplicar dano
        for (int k = 0; k < numBarricadas; k++)
        {
            if (inimigoAoLado(array_inimigos, i, barricadas[k].coord.x, barricadas[k].coord.y))
            {
                if (tempoAtual - barricadas[k].ultimoDano >= 2.0)
                {
                    barricadas[k].vida--;
                    barricadas[k].ultimoDano = tempoAtual;
                    if (barricadas[k].vida <= 0)
                    {
                        map[barricadas[k].coord.y - MAP_OFFSET][barricadas[k].coord.x] = '.'; // Atualizar a posição no mapa para '.'
                        for (int l = k; l < numBarricadas - 1; l++)
                        {
                            barricadas[l] = barricadas[l + 1];
                        }
                        numBarricadas--;
                    }
                }
            }
        }

        // Produzir recursos nas minas a cada 5 segundos
        for (int m = 0; m < numMinas; m++)
        {
            if (tempoAtual - minas[m].ultimoRecurso >= 5.0)
            {
                recursos++;
                minas[m].ultimoRecurso = tempoAtual;
            }
        }

        // Arqueiros atirando flechas
        for (int a = 0; a < numArqueiros; a++)
        {
            if (tempoAtual - arqueiros[a].ultimoTiro >= 5.0)
            {
                atirarFlecha(&arqueiros[a], flechas, &numFlechas, array_inimigos, i);
            }
        }

        // Atualizar flechas
        atualizarFlechas(flechas, &numFlechas, array_inimigos, &i);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawMap();

        // Desenha os inimigos e suas barras de vida
        for (int j = 0; j < i; j++)
        {
            DrawTexture(inimigotexture, array_inimigos[j].coord.x * LADO, array_inimigos[j].coord.y * LADO, WHITE);
            drawBarraDeVidaInimigo(array_inimigos[j].coord.x, array_inimigos[j].coord.y, array_inimigos[j].vida);
        }

        // Desenha as barricadas e suas barras de vida
        for (int k = 0; k < numBarricadas; k++)
        {
            DrawTexture(barricadatexture, barricadas[k].coord.x * LADO, barricadas[k].coord.y * LADO, WHITE);
            drawBarraDeVidaBarricada(barricadas[k].coord.x, barricadas[k].coord.y, barricadas[k].vida);
        }


        // Desenha as flechas
        for (int f = 0; f < numFlechas; f++)
        {
            if (flechas[f].ativa)
            {
                DrawTexture(flechatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
            }
        }

        // Desenho da HUD
        hud();
        DrawRectangle(player.coord.x * LADO, player.coord.y * LADO, LADO, LADO, RED);

        EndDrawing();
    }

    UnloadSound(fxWav);
    UnloadSound(fxWav2); // Unload sound data
    UnloadTexture(inimigotexture); // Descarregar a textura do inimigo
    UnloadTexture(barricadatexture); // Descarregar a textura da barricada
    UnloadTexture(flechatexture); // Descarregar a textura da flecha

    DescarregarTexturas();
    CloseAudioDevice(); // Close audio device
    CloseWindow();

    return 0;
}
