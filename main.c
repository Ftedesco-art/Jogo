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
    Sound fxWav = LoadSound("Sons/Buraco.ogg");
    Sound fxWav2 = LoadSound("Sons/Barricada.ogg");
    SetSoundVolume(fxWav, 0.1f);
    SetSoundVolume(fxWav2, 0.1f);

    bool paused = false;
    int recursos, tempoAtual = 0;
    int numInimigos, numFlechas, numArqueiros, numMinas, numBarricadas, numBombas;

    // Inicializa estruturas
    INIMIGO array_inimigos[MAX_INIMIGOS];
    JOGADOR player;
    BASE base;
    BARRICADA barricadas[MAX_BARRICADAS];
    MINA minas[MAX_MINAS];
    ARQUEIRO arqueiros[MAX_ARQUEIROS];
    FLECHA flechas[MAX_FLECHAS];
    BURACO buracos[MAX_BURACOS];
    BOMBA bombas[MAX_BOMBAS];
    ROUND info_rounds[MAX_ROUNDS];

    // Resetar variáveis antes de carregar
    reiniciarJogo(array_inimigos, &player, &base, barricadas,
                   &numBarricadas, minas, &numMinas,
                   arqueiros, &numArqueiros, flechas,
                   &numFlechas, &numInimigos, &recursos,
                   &tempoAtual, &numBombas, &bombas, info_rounds);

    if (info.flag == 1) { // Jogo salvo
        if (!carrega_dados("jogo_salvo.bin", map, array_inimigos, &player, &base, barricadas,
                        &numBarricadas, minas, &numMinas, arqueiros,
                        &numArqueiros, flechas, &numFlechas,
                        &numInimigos, &recursos, &tempoAtual, &numBombas, &bombas)) {
            printf("Falha ao carregar o jogo.\n");
        }
    }

    // Carrega texturas
    CarregarTexturas();
    CarregaFont();

    bool flagTrap = false;
    float velocity = .1; // Movimento a cada quantos segundos
    int i;
    int flagAcabou = 0;
    int trapTimer = 0;
    int ultimoDano = 0;
    int timerBuraco = 0;
    int timerAcabou = 0;
    int flagMenu = 0;
    int freqInimigos = 180;
    int round = 0;
    int flagCriacao = 0;

    achaSpawnInimigos(info_rounds);

    info_rounds[0].qtddInimigos = 7;
    info_rounds[1].qtddInimigos = 10;
    info_rounds[2].qtddInimigos = 15;

    while (!WindowShouldClose() && !paused)
    {
        bool sair = false;

        if (IsKeyPressed(KEY_TAB))
        {
            paused = !paused;
        }

        if (!paused && timerAcabou <= 0) {
            salva_dados(map, array_inimigos, &player, &base, barricadas, numBarricadas, minas,
                        numMinas, arqueiros, numArqueiros, flechas, numFlechas, numInimigos, recursos, tempoAtual, numBombas, bombas);
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

            // Verifica se o inimigo colidiu com o jogador
            if (map[yPersonagem][xPersonagem] == 'M')
            {
                // Verifica se o cooldown de dano foi respeitado
                if (tempoAtual - ultimoDano >= 60) // 1 segundo de cooldown
                {
                    player.vidas--; // Decrementa a vida do jogador
                    ultimoDano = tempoAtual; // Atualiza o tempo do último dano
                }
            }

            if (!flagTrap || trapTimer <= 0)
            {
                velocity = 1;
                flagTrap = false;
            }
            else
            {
                trapTimer--;
            }

            int id = 0;
            int contadorId = 0;

            // -------------------------------------- Buracos -------------------------------------- //
            for (int mapY = 0; mapY < ALTURA_GRID + MAP_OFFSET; mapY++)
            {
                for (int mapX = 0; mapX < LARGURA_GRID; mapX++)
                {
                    if(map[mapY][mapX] == 'H' && contadorId < MAX_BURACOS) {
                        buracos[contadorId].coord.x = mapX;
                        buracos[contadorId].coord.y = mapY;
                        buracos[contadorId].id = id;

                        contadorId++;
                        if (contadorId % 2 == 0) {
                            id++;
                        }
                    }
                }
            }
            // -------------------------------------- Buracos -------------------------------------- //

            switch (map[yPersonagem][xPersonagem]) {
                case 'R': // Verifica se o jogador encostou no recurso
                    recursos++;
                    map[yPersonagem][xPersonagem] = '.';
                    break;

                case 'H': // Buracos
                    if (timerBuraco <= 0) {
                        entraBuraco(&player.coord.x, &player.coord.y, buracos);
                        PlaySound(fxWav);
                        timerBuraco = 120;
                    }
                    break;

                case 'T': // Armadilhas
                    velocity = 1;
                    trapTimer = 180;
                    flagTrap = true;
                    break;
                default:
                    break;
            }
            timerBuraco--;


            // -------------------------------------- Barricadas -------------------------------------- //
            if (recursos >= 2 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed('E')
                              && numBarricadas < MAX_BARRICADAS && map[yPersonagem][xPersonagem] != '1')
            {
                PlaySound(fxWav2);
                map[yPersonagem][xPersonagem] = '1';
                barricadas[numBarricadas].coord.x = xPersonagem;
                barricadas[numBarricadas].coord.y = yPersonagem + MAP_OFFSET;
                barricadas[numBarricadas].vidaAtual = 5;
                barricadas[numBarricadas].vidaMax = 5;
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
                bombas[numBombas].coord.x = xPersonagem;
                bombas[numBombas].coord.y = yPersonagem + MAP_OFFSET;
                bombas[numBombas].flagDuracao = 0;
                numBombas++;
                recursos -= 2;
            }

            for (i = 0; i < numBombas; i++) {
                if (bombas[i].flagDuracao == 1) {
                   bombas[i].timerBomba++;
                   if (bombas[i].timerBomba >= 300) { // Timer em segundos * 60
                        map[bombas[i].coord.y - MAP_OFFSET][bombas[i].coord.x] = '.';
                    }
                }
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
                minas[numMinas].freq = 8;
                minas[numMinas].ultimoRecurso = tempoAtual; // Inicializa o tempo do último recurso produzido
                numMinas++;
                recursos -= 6;
            }

            // Produzir recursos nas minas a um periodo (freq) de segundos
            for (int m = 0; m < numMinas; m++)
            {
                if (tempoAtual - minas[m].ultimoRecurso >= minas[m].freq * 60)
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
                arqueiros[numArqueiros].freq = 2;
                arqueiros[numArqueiros].ultimoTiro = tempoAtual; // Inicializa o tempo do último tiro
                numArqueiros++;
                recursos -= 4;
            }

            // Arqueiros atirando flechas
            for (int a = 0; a < numArqueiros; a++)
            {
                if (tempoAtual - arqueiros[a].ultimoTiro >= arqueiros[a].freq * 60) // Padrão: 2, upgrade: 1
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
            if(freqInimigos >= 180 && flagCriacao == 1) {
                cria_inimigo(info_rounds[round].spawn.x, info_rounds[round].spawn.y, array_inimigos, &numInimigos);

                freqInimigos = 0;
                info_rounds[round].qtddInimigos--;

                if (info_rounds[round].qtddInimigos == 0) {
                    flagCriacao = 0;
                }
            }
            freqInimigos++;

            for (int j = 0; j < numInimigos; j++)
            {
                int tempoDesdeUltimoMovimento = tempoAtual - array_inimigos[j].ultimoMovimento;
                if (tempoDesdeUltimoMovimento >= velocity * 60)
                {
                    moverInimigo(&array_inimigos[j], base.coord.x, base.coord.y, &novosX[j], &novosY[j]);
                    array_inimigos[j].ultimoMovimento = tempoAtual; // Atualiza o tempo do último movimento

                    char tile = map[novosY[j] - MAP_OFFSET][novosX[j]];

                    // Inimigo morreu por vida zerada
                    if (array_inimigos[j].vida <= 0)
                    {

                        map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa
                        removerInimigo(array_inimigos, &numInimigos, j);
                        j--;
                    }

                    else {
                        switch (tile) {
                            case '1': // Inimigo encontrou uma barricada
                                for (i = 0; i < numBarricadas; i++)
                                {
                                    if (barricadas[i].coord.x == novosX[j] && barricadas[i].coord.y == novosY[j])
                                    {
                                        barricadas[i].vidaAtual--;
                                        if (barricadas[i].vidaAtual <= 0)
                                        {
                                            map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualiza a posição da barricada no mapa para '.'
                                            for (int k = i; k < numBarricadas - 1; k++)
                                            {
                                                barricadas[k] = barricadas[k + 1];
                                            }
                                            numBarricadas--;
                                        }
                                        break;
                                    }
                                }
                                break;

                            case 'S': // Inimigo chegou à base
                                map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa
                                base.vidas--;
                                removerInimigo(array_inimigos, &numInimigos, j);
                                j--;
                                break;
                            case '2': // Inimigo encontrou uma bomba
                                map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa

                                for (i = 0; i < numBombas; i++) {
                                    if (bombas[i].coord.x == novosX[j] && bombas[i].coord.y == novosY[j]) {
                                        if (bombas[i].flagDuracao == 0) {
                                            map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualiza a posição da bomba no mapa para '.'

                                            for (int k = i; k < numBombas - 1; k++) {
                                                bombas[k] = bombas[k + 1]; // Move as bombas para preencher a posição
                                            }
                                        numBombas--; // Decrementa o número de bombas
                                        }
                                    }
                                }
                                removerInimigo(array_inimigos, &numInimigos, j);
                                j--;
                                break;
                            case '.': // Movimento normal
                                map[array_inimigos[j].coord.y - MAP_OFFSET][array_inimigos[j].coord.x] = '.'; // Limpa a posição antiga no mapa
                                array_inimigos[j].coord.x = novosX[j];
                                array_inimigos[j].coord.y = novosY[j];
                                map[novosY[j] - MAP_OFFSET][novosX[j]] = 'M'; // Atualiza a posição no mapa para 'M'
                                break;
                            case '4': // Inimigo encontrou um arqueiro
                                for (i = 0; i < numArqueiros; i++)
                                {
                                    if (arqueiros[i].coord.x == novosX[j] && arqueiros[i].coord.y == novosY[j])
                                    {
                                        map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualiza a posição do arqueiro no mapa para '.'
                                        for (int k = i; k < numArqueiros - 1; k++)
                                        {
                                            arqueiros[k] = arqueiros[k + 1];
                                        }
                                        numArqueiros--;
                                        break;
                                    }
                                }
                                break;

                            default:
                                break;
                        }
                    }
                }
            }
            // -------------------------------------- Inimigos -------------------------------------- //

            if (player.vidas <= 0 || base.vidas <= 0) {
                flagAcabou = 2; // Perdeu
            }

            else if (numInimigos <= 0 && round == 2) {
                flagAcabou = 1; // Ganhou
            }

            else if (numInimigos <= 0 && round < MAX_ROUNDS - 1) {
                round++;
                flagCriacao = 1;
            }
        }

        else if (paused && flagAcabou <= 0) {
            if (!menu_pause(&paused)) {
                break; // Apenas sair do loop se 'sair' for true
            }
        }

        else {

        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawMap();

        int xClique, yClique, xCliqueNovo, yCliqueNovo;

        // Verifica se o botão do mouse foi pressionado - primeira vez
        if (!flagMenu) {
            if (IsMouseButtonPressed(0)) {
                xClique = GetMouseX() / 20; // Função para obter a coordenada X do mouse
                yClique = GetMouseY() / 20 - MAP_OFFSET; // Função para obter a coordenada Y do mouse

                char tile = map[yClique][xClique];

                if (tile == '1' || tile == '2' || tile == '3' || tile == '4') {
                    flagMenu = 1; // Marca que o menu está aberto
                }
            }
        } else {
            if (IsMouseButtonPressed(0)) {
                xCliqueNovo = GetMouseX(); // Função para obter a coordenada X do mouse
                yCliqueNovo = GetMouseY(); // Função para obter a coordenada Y do mouse

                // Chama a função de upgradeMenu e verifica se o menu deve ser fechado
                int resultado = upgradeMenu(xClique, yClique, xCliqueNovo, yCliqueNovo, numBarricadas, numBombas, numMinas, numArqueiros, barricadas, bombas, minas, arqueiros, &recursos);

                if (resultado == 0) {
                    flagMenu = 0; // Fecha o menu se upgradeMenu retornar 0
                }
            }
        }

        // Desenho do menu se estiver aberto
        if (flagMenu) {
            DrawRectangle(1000, 20, 80, 80, RED);   // Botão de fechar
            DrawRectangle(1080, 20, 80, 80, GREEN); // Botão de compra
        }

        // Desenho da HUD
        DrawHud(recursos, tempoAtual, base, player);

        // Desenho de coisas gerais
        DrawGeneral(&flechas, numFlechas, &array_inimigos, numInimigos, &barricadas, numBarricadas);

        if(!End(flagAcabou, &timerAcabou)) {
            break;
        }

        DrawRectangle(player.coord.x * LADO, player.coord.y * LADO, LADO, LADO, RED);

        EndDrawing();
    }

    UnloadSound(fxWav);
    UnloadSound(fxWav2);
    DescarregaFont();
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
