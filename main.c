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


int recursos;
int numInimigos;
int numFlechas;
int numArqueiros;
int numMinas;
int numBarricadas;
int tempoAtual = 0;

int run()
{
    srand(time(NULL));

    // Inicialização dos sons
    InitAudioDevice();
    Sound fxWav = LoadSound("Sons\\Buraco.ogg");
    Sound fxWav2 = LoadSound("Sons\\Barricada.ogg");
    SetSoundVolume(fxWav, 0.1f);
    SetSoundVolume(fxWav2, 0.1f);

    bool paused = false;

    // Inicializa estruturas
    INIMIGO array_inimigos[MAX_INIMIGOS];
    JOGADOR player;
    BASE base;
    BARRICADA barricadas[MAX_BARRICADAS];
    MINA minas[MAX_MINAS];
    ARQUEIRO arqueiros[MAX_ARQUEIROS];
    FLECHA flechas[MAX_FLECHAS];

    if (info.flag == 0) { // Jogo não foi carregado, é um jogo novo
        reiniciarJogo(array_inimigos, &player, &base, barricadas,
                       &numBarricadas, minas, &numMinas,
                       arqueiros, &numArqueiros, flechas,
                       &numFlechas, &numInimigos, &recursos,
                       &tempoAtual);
    } else { // run() iniciou como um jogo salvo
        carrega_jogo("jogo_salvo.bin", &info);
        carrega_dados(map, array_inimigos, &player, &base, barricadas,
               &numBarricadas, minas, &numMinas, arqueiros,
               &numArqueiros, flechas, &numFlechas,
               &numInimigos, &recursos, &tempoAtual);
    }

    // Carrega texturas
    CarregarTexturas();

    bool flagTrap = false;
    int trapTimer = 0;
    float velocity = 1; // Movimento a cada quantos segundos
    int i;

    while (!WindowShouldClose() && !paused)
    {
        bool sair = false;

        if (IsKeyPressed(KEY_TAB))
        {
            paused = !paused;
        }

        if (!paused) {
            salva_dados(map, array_inimigos, &player, &base, barricadas, numBarricadas, minas,
                        numMinas, arqueiros, numArqueiros, flechas, numFlechas, numInimigos, recursos, tempoAtual);
            tempoAtual++;

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

            int xPersonagem = player.coord.x;
            int yPersonagem = player.coord.y - MAP_OFFSET;

            if (!flagTrap || trapTimer <= 0)
            {
                velocity = 1;
                flagTrap = false;
            }
            else
            {
                trapTimer--;
            }

            switch (map[yPersonagem][xPersonagem]) {
                case 'R': // Verifica se o jogador encostou no recurso
                    recursos++;
                    map[yPersonagem][xPersonagem] = '.'; // Remove o recurso do mapa
                    break;

                case 'H': // Buracos
                    PlaySound(fxWav);
                    break;

                case 'T': // Armadilhas
                    velocity = 0.04;
                    trapTimer = 120;
                    flagTrap = true;
                    break;
                default:
                    break;
            }

            // -------------------------------------- Barricadas -------------------------------------- //
            if (recursos >= 2 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed('E')
                              && numBarricadas < MAX_BARRICADAS && map[yPersonagem][xPersonagem] != '1')
            {
                PlaySound(fxWav2);
                map[yPersonagem][xPersonagem] = '1';
                barricadas[numBarricadas].coord.x = xPersonagem;
                barricadas[numBarricadas].coord.y = yPersonagem + MAP_OFFSET;
                barricadas[numBarricadas].vida = 3; // Inicializa a vida da barricada
                barricadas[numBarricadas].ultimoDano = tempoAtual; // Inicializa o tempo do último dano
                numBarricadas++;
                recursos -= 2;
            }
            // -------------------------------------- Barricadas -------------------------------------- //


            // -------------------------------------- Bombas -------------------------------------- //
            if (recursos >= 2 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed('G') && map[yPersonagem][xPersonagem] != '2')
            {
                PlaySound(fxWav2);
                map[yPersonagem][xPersonagem] = '2';
                recursos -= 2;
            }
            // -------------------------------------- Bombas -------------------------------------- //

            // -------------------------------------- Minas -------------------------------------- //
            if (recursos >= 6 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed('R')
                              && numMinas < MAX_MINAS && map[yPersonagem][xPersonagem] != '3')
            {
                PlaySound(fxWav2);
                map[yPersonagem][xPersonagem] = '3';
                minas[numMinas].coord.x = xPersonagem;
                minas[numMinas].coord.y = yPersonagem + MAP_OFFSET;
                minas[numMinas].ultimoRecurso = tempoAtual; // Inicializa o tempo do último recurso produzido
                numMinas++;
                recursos -= 6;
            }

            // Produzir recursos nas minas a cada 8 segundos
            for (int m = 0; m < numMinas; m++)
            {
                if (tempoAtual - minas[m].ultimoRecurso >= 8 * 60)
                {
                    recursos++;
                    minas[m].ultimoRecurso = tempoAtual;
                }
            }
            // -------------------------------------- Minas -------------------------------------- //

            // -------------------------------------- Arqueiros -------------------------------------- //
            if (recursos >= 4 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed('F')
                              && numArqueiros < MAX_ARQUEIROS && map[yPersonagem][xPersonagem] != '4')
            {
                PlaySound(fxWav2);
                map[yPersonagem][xPersonagem] = '4';
                arqueiros[numArqueiros].coord.x = xPersonagem;
                arqueiros[numArqueiros].coord.y = yPersonagem + MAP_OFFSET;
                arqueiros[numArqueiros].ultimoTiro = tempoAtual; // Inicializa o tempo do último tiro
                numArqueiros++;
                recursos -= 4;
            }

            // Arqueiros atirando flechas
            for (int a = 0; a < numArqueiros; a++)
            {
                if (tempoAtual - arqueiros[a].ultimoTiro >= 2 * 60) // Padrão: 2
                {
                    atirarFlecha(&arqueiros[a], flechas, &numFlechas, array_inimigos, numInimigos);
                    arqueiros[a].ultimoTiro = tempoAtual; // Atualiza o tempo do último tiro
                }
            }

            // Atualizar flechas
            atualizarFlechas(flechas, &numFlechas, array_inimigos, &numInimigos);

            int novosX[MAX_INIMIGOS];
            int novosY[MAX_INIMIGOS];
            // -------------------------------------- Arqueiros -------------------------------------- //

            // -------------------------------------- Inimigos -------------------------------------- //
            for (int j = 0; j < numInimigos; j++)
            {
                int tempoDesdeUltimoMovimento = tempoAtual - array_inimigos[j].ultimoMovimento;
                if (tempoDesdeUltimoMovimento >= velocity * 60)
                {
                    moverInimigo(&array_inimigos[j], base.coord.x, base.coord.y, &novosX[j], &novosY[j]);
                    array_inimigos[j].ultimoMovimento = tempoAtual; // Atualiza o tempo do último movimento

                    // Se for uma barricada
                    char tile = map[novosY[j] - MAP_OFFSET][novosX[j]];
                    if (tile == '1')
                    {
                        // Encontra a barricada correspondente
                        for (int k = 0; k < numBarricadas; k++)
                        {
                            if (barricadas[k].coord.x == novosX[j] && barricadas[k].coord.y == novosY[j])
                            {
                                barricadas[k].vida--;
                                if (barricadas[k].vida <= 0)
                                {
                                    map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualiza a posição no mapa para '.'
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

                    // Inimigo chegou à base 'S'
                    else if ((tile == 'S'))
                    {
                        map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa
                        removerInimigo(array_inimigos, &numInimigos, j);
                        j--; // Ajusta o índice após remoção

                        base.vidas--;
                    }

                    // Inimigo encontrou uma bomba
                    else if (tile == '2')
                    {
                        map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualiza a posição no mapa para '.'
                        map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa
                        removerInimigo(array_inimigos, &numInimigos, j);
                        j--; // Ajusta o índice após remoção
                    }


                    // Inimigo morreu por causa da vida zerada
                    else if (array_inimigos[j].vida <= 0)
                    {
                        map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa
                        removerInimigo(array_inimigos, &numInimigos, j);
                        j--; // Ajusta o índice após remoção
                    }

                    // Movimento normal
                    else if (tile == '.')
                    {
                        map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpar a posição antiga no mapa
                        array_inimigos[j].coord.x = novosX[j];
                        array_inimigos[j].coord.y = novosY[j];
                        map[novosY[j] - MAP_OFFSET][novosX[j]] = 'M'; // Atualiza a posição no mapa
                    }
                }
            }
            // -------------------------------------- Inimigos -------------------------------------- //
        }
        else {
            if (!menu_pause(&paused)) {
                break; // Apenas sair do loop se 'sair' for true
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawMap();

        // Desenha os inimigos e suas barras de vida
        for (int j = 0; j < numInimigos; j++)
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
                if(flechas[f].direcao.x == 1) {// Direita
                    DrawTexture(flechadireitatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
                }
                else if (flechas[f].direcao.x == -1) {// Esquerda
                    DrawTexture(flechaesquerdatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
                }
                else if (flechas[f].direcao.y == 1) {// Baixo
                    DrawTexture(flechacimatexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
                }
                else if (flechas[f].direcao.y == -1) {// Cima
                    DrawTexture(flechabaixotexture, flechas[f].coord.x * LADO, flechas[f].coord.y * LADO, WHITE);
                }
            }
        }

        // Desenho da HUD
        hud();
        DrawRectangle(player.coord.x * LADO, player.coord.y * LADO, LADO, LADO, RED);

        EndDrawing();
    }

    UnloadSound(fxWav);
    UnloadSound(fxWav2);
    DescarregarTexturas();
    CloseAudioDevice();
    return 1;
}

int main(void) {
    InitWindow(LARGURA, ALTURA, "Página principal");
    SetTargetFPS(60);

    while(menu_inicial()) {
    }

    CloseWindow();

    return 0;
}
