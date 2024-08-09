#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "mapa.h"
#include "load.h"
#include "hud.h"
#include "eventos.h"

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
    int recursos, tempoAtual = 0;
    int numInimigos, numFlechas, numArqueiros, numMinas, numBarricadas;

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
        // Resetar variáveis antes de carregar
        reiniciarJogo(array_inimigos, &player, &base, barricadas,
                       &numBarricadas, minas, &numMinas,
                       arqueiros, &numArqueiros, flechas,
                       &numFlechas, &numInimigos, &recursos,
                       &tempoAtual);
        if (!carrega_dados("jogo_salvo.bin", map, array_inimigos, &player, &base, barricadas,
                        &numBarricadas, minas, &numMinas, arqueiros,
                        &numArqueiros, flechas, &numFlechas,
                        &numInimigos, &recursos, &tempoAtual)) {
            printf("Falha ao carregar o jogo.\n");
        }
    }

    // Carrega texturas
    CarregarTexturas();

    bool flagTrap = false;
    float velocity = .1; // Movimento a cada quantos segundos
    int i;
    int timerAcabou = 0;
    int flagAcabou = 0;
    int trapTimer = 0;
    int ultimoDano = 0;


    while (!WindowShouldClose() && !paused)
    {
        bool sair = false;

        if (IsKeyPressed(KEY_TAB))
        {
            paused = !paused;
        }

        if (!paused && timerAcabou <= 0) {
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
                velocity = .1;
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

                    char tile = map[novosY[j] - MAP_OFFSET][novosX[j]];

                    // Se for uma barricada
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

                    // Verifica se o inimigo colidiu com o jogador
                    if (novosX[j] == player.coord.x && novosY[j] == player.coord.y)
                    {
                        // Verifica se o cooldown de dano foi respeitado
                        if (tempoAtual - ultimoDano >= 60) // 1 segundo de cooldown
                        {
                            player.vidas--; // Decrementa a vida do jogador
                            ultimoDano = tempoAtual; // Atualiza o tempo do último dano
                        }
                    }
                }
            }
            // -------------------------------------- Inimigos -------------------------------------- //

            if (player.vidas <= 0 || base.vidas <= 0) {
                flagAcabou = 2; // Perdeu
            }

            else if (numInimigos <= 0 ) {
                flagAcabou = 1; // Ganhou
            }
        }

        else if (paused && timerAcabou <= 0) {
            if (!menu_pause(&paused)) {
                break; // Apenas sair do loop se 'sair' for true
            }
        }

        else {

        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawMap();

        // Desenho da HUD
        DrawHud(recursos, tempoAtual);

        // Desenho de coisas gerais. Se houver flag de jogo ganho ou perdido, então sai do loop;
        if(!DrawGeneral(flagAcabou, &timerAcabou, &flechas, numFlechas, &array_inimigos, numInimigos, &barricadas, numBarricadas)) {
            break;
        }

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
