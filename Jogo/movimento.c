#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "movimento.h"
#include "mapa.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 600
#define NUM_INIMIGOS 7
#define TEMPO_MENSAGEM 120 // 2 segundos (a 60 FPS)

typedef struct inimigo
{
    int x;
    int y;
    int dx;
    int dy;
} TYPE_INIMIGO;

void reiniciaJogo(int *xPersonagem, int *yPersonagem, int *recursos, int *vidaDaBase, int *vidaJogador, TYPE_INIMIGO array_inimigos[])
{
    int i, j, contador = 0;

    // Recarrega o mapa (zerado)
    LoadMap("mapa.txt");

    // Reinicializa as variaveis
    *xPersonagem = 4;
    *yPersonagem = 2;
    *recursos = 0;
    *vidaDaBase = 3;
    *vidaJogador = 3;

    // Reinicializa os inimigos
    for (i = 0; i < MAP_WIDTH; i++)
    {
        for (j = 0; j < MAP_HEIGHT; j++)
        {
            if (map[j][i] == 'E')
            {
                array_inimigos[contador].x = i;
                array_inimigos[contador].y = j;
                array_inimigos[contador].dx = 0;
                array_inimigos[contador].dy = 0;
                contador++;
            }
        }
    }
}

int podeMover(int xElemento, int yElemento, int dx, int dy)
{
    if (map[yElemento + dy][xElemento + dx] == 'W' || (dx == 0 && dy == 0))
    {
        return 0; // Nao pode mover
    }
    else
    {
        return 1; // Pode mover
    }
}

void move(int *x, int *y, int dx, int dy)
{
    *x += dx;
    *y += dy;
}

// Inicializa as texturas
Texture2D barricadatexture;
Texture2D paredetexture;

int runJogo(void)
{
    srand(time(NULL));

    // Inicializa�ao de variaveis
    int xPersonagem = 4;
    int yPersonagem = 2;
    int i, j, contador = 0, recursos = 0;

    // Inicializa a lista de inimigos
    TYPE_INIMIGO array_inimigos[NUM_INIMIGOS];

    // Inicializa os sons
    InitAudioDevice();
    Sound fxWav = LoadSound("Sons\\Buraco.ogg");
    Sound fxWav2 = LoadSound("Sons\\Barricada.ogg");
    SetSoundVolume(fxWav, 0.1f);
    SetSoundVolume(fxWav2, 0.1f);

    // Carrega o arquivo do mapa
    LoadMap("mapa.txt");

    // Inicializa os inimigos
    for (i = 0; i < MAP_WIDTH; i++)
    {
        for (j = 0; j < MAP_HEIGHT; j++)
        {
            if (map[j][i] == 'E')
            {
                array_inimigos[contador].x = i;
                array_inimigos[contador].y = j;
                array_inimigos[contador].dx = 0;
                array_inimigos[contador].dy = 0;
                contador++;
            }
        }
    }

    InitWindow(LARGURA, ALTURA, "Pagina principal - Fase 1");
    SetTargetFPS(60);

    // Inicializa variavel de pause
    bool isPaused = false;

    // Inicializa as texturas
    barricadatexture = LoadTexture("Texturas\\barricada1.png");
    paredetexture = LoadTexture("Texturas\\Parede.png");

    // Inicializa variaveis para a trap
    int contadorMovimentos = 0;
    int velocidadeAumentada = 5;
    int tempoTrap = 0;
    int trapAtivada = 0;

    // Inicializa variaveis para a mensagem
    int tempoMensagem = 0;
    int mensagemAtivada = 0;

    // Inicializa variaveis para o Game Over
    int vidaJogador = 3;
    int vidaDaBase = 3; // Vida inicial da base
    bool flagGameOver = false; // Indica se o jogo acabou
    int tempoGameOver = 0; // Tempo para exibir Game Over

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            isPaused = !isPaused;
        }

        if (!isPaused && !flagGameOver)
        {
            // Movimenta��o do personagem
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && podeMover(xPersonagem, yPersonagem, 1, 0))
            {
                move(&xPersonagem, &yPersonagem, 1, 0);
            }

            if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && podeMover(xPersonagem, yPersonagem, -1, 0))
            {
                move(&xPersonagem, &yPersonagem, -1, 0);
            }

            if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && podeMover(xPersonagem, yPersonagem, 0, -1))
            {
                move(&xPersonagem, &yPersonagem, 0, -1);
            }

            if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && podeMover(xPersonagem, yPersonagem, 0, 1))
            {
                move(&xPersonagem, &yPersonagem, 0, 1);
            }

            // Verifica se o jogador encostou em um recurso (R)
            if (map[yPersonagem][xPersonagem] == 'R')
            {
                recursos++;
                map[yPersonagem][xPersonagem] = '.'; // Remove o recurso do mapa
            }

            // Verifica se o jogador encostou em um buraco (H)
            if (map[yPersonagem][xPersonagem] == 'H')
            {
                PlaySound(fxWav);
            }

            // Verifica se o jogador encostou em uma armadilha (T)
            if (map[yPersonagem][xPersonagem] == 'T')
            {
                trapAtivada = 1; // Ativa a trap
                tempoTrap = 0; // Reinicia o contador de tempo
                mensagemAtivada = 1; // Ativa a mensagem
                tempoMensagem = 0; // Reinicia o contador da mensagem
            }

            if (trapAtivada)
            {
                tempoTrap++;
                if (tempoTrap >= 100)
                {
                    trapAtivada = 0; // Desativa a trap ap�s 100 itera��es
                }
            }

            // Incrementa o contador de movimentos
            contadorMovimentos++;

            if (contadorMovimentos % (trapAtivada ? 10 / velocidadeAumentada : 10) == 0)
            {
                for (i = 0; i < NUM_INIMIGOS; i++)
                {
                    // Verifica se o inimigo pode se mover na dire��o atual
                    if (!podeMover(array_inimigos[i].x, array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy))
                    {
                        // Se n�o puder mover, gera novos valores para dx e dy
                        array_inimigos[i].dx = rand() % 3 - 1; // Pode ser -1, 0, ou 1
                        array_inimigos[i].dy = rand() % 3 - 1; // Pode ser -1, 0, ou 1
                    }
                    // Tenta mover o inimigo na dire��o atual
                    if (podeMover(array_inimigos[i].x, array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy))
                    {
                        move(&array_inimigos[i].x, &array_inimigos[i].y, array_inimigos[i].dx, array_inimigos[i].dy);
                    }
                }
            }

            // Atualiza a l�gica da mensagem
            if (mensagemAtivada)
            {
                tempoMensagem++;
                if (tempoMensagem >= TEMPO_MENSAGEM)
                {
                    mensagemAtivada = 0; // Desativa a mensagem ap�s 2 segundos
                }
            }

            for(i = 0; i < NUM_INIMIGOS; i++) {
                // Quando o inimigo toca na base
                if (map[array_inimigos[i].y][array_inimigos[i].x] == 'S') {
                    // 'Mata' o inimigo
                    array_inimigos[i].x = -1;
                    array_inimigos[i].y = -1;

                    // Tira vida da base
                    vidaDaBase --;
                }

                // Quando o inimigo toca no jogador
                if (array_inimigos[i].y == yPersonagem && array_inimigos[i].x == xPersonagem) {
                    // Tira vida do jogador
                    vidaJogador --;
                }
            }


            // Verifica se a vida da base chegou a zero
            if (vidaDaBase <= 0 || vidaJogador <= 0)
            {
                flagGameOver = true; // Ativa o estado de Game Over
                tempoGameOver = 0; // Reinicia o contador de tempo do Game Over
            }
        }



        // Se o jogo est� em estado de Game Over
        if (flagGameOver)
        {
            tempoGameOver++;
            if (tempoGameOver >= 120) // Mostra a mensagem por 2 segundos
            {
                CloseWindow(); // Fecha a janela ap�s 2 segundos
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenho do mapa
        DrawMap();

        // Desenho do personagem
        DrawRectangle(xPersonagem * LADO, yPersonagem * LADO, LADO, LADO, WHITE);

        // Desenho das barricadas
        if(recursos >= 2 && map[yPersonagem][xPersonagem] == '.' && IsKeyPressed(KEY_B)) {
            PlaySound(fxWav2);
            map[yPersonagem][xPersonagem] = 'W'; // Constr�i uma barricada no local do personagem
            recursos -= 2; // Reduz recursos
        }

        // Desenho dos inimigos
        for (i = 0; i < NUM_INIMIGOS; i++)
        {
            DrawRectangle(array_inimigos[i].x * LADO, array_inimigos[i].y * LADO, LADO, LADO, RED);
        }

        // Exibe a mensagem de Game Over
        if (flagGameOver)
        {
            // Desenha a sombra
            DrawText("GAME OVER!", LARGURA / 2 - MeasureText("GAME OVER!", 40) / 2 + 2, ALTURA / 2 - 10 + 2, 40, BLACK);
            // Desenha o texto principal
            DrawText("GAME OVER!", LARGURA / 2 - MeasureText("GAME OVER!", 40) / 2, ALTURA / 2 - 10, 40, RED);
        }

        // Exibe a mensagem sobre a armadilha
        if (mensagemAtivada)
        {
            DrawText("ARMADILHA ATIVADA!", LARGURA / 2 - MeasureText("ARMADILHA ATIVADA!", 20) / 2, ALTURA / 2 + 20, 20, BLACK);
        }

        if (isPaused)
        {
            // Define a cor desejada
            Color textColor = DARKBLUE; // Mude para a cor que preferir
            int spacing = 30; // Aumente o espa�amento entre os textos

            // Desenha os textos centralizados
            DrawText("PAUSADO", LARGURA / 2 - MeasureText("PAUSADO", 40) / 2, ALTURA / 2 - 20 - spacing, 40, textColor);
            DrawText("Pressione N para Novo Jogo", LARGURA / 2 - MeasureText("Pressione N para Novo Jogo", 20) / 2, ALTURA / 2 + 10, 20, textColor);
            DrawText("Pressione Q para Sair", LARGURA / 2 - MeasureText("Pressione Q para Sair", 20) / 2, ALTURA / 2 + 10 + spacing, 20, textColor);

            // Checa se o usu�rio pressiona N ou Q
            if (IsKeyPressed('N'))
            {
                reiniciaJogo(&xPersonagem, &yPersonagem, &recursos, &vidaDaBase, &vidaJogador, array_inimigos);
                isPaused = !isPaused;
            }
            else if (IsKeyPressed('Q'))
            {
                return 0;
            }
        }

        // Desenho da posi��o do personagem
        char posText[50];
        sprintf(posText, "Pos: (%d, %d)", xPersonagem, yPersonagem);
        DrawText(posText, 5, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(posText, 5 + 1, 1, 20, (Color){200, 200, 200, 255}); // Texto cinza

        // Desenho das vidas do jogador
        char vidaJogadorText[50];
        sprintf(vidaJogadorText, "Vidas do jogador: %d", vidaJogador);
        DrawText(vidaJogadorText, 130, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(vidaJogadorText, 130 + 1, 1, 20, (Color){255, 255, 255, 255}); // Texto branco

        // Desenho   das vidas da base
        char vidaBaseText[50];
        sprintf(vidaBaseText, "Vidas da base: %d", vidaDaBase);
        DrawText(vidaBaseText, 350, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(vidaBaseText, 350 + 1, 1, 20, (Color){255, 255, 200, 255}); // Texto amarelo clara

        // Desenho do n�mero de recursos
        char recursosText[50];
        sprintf(recursosText, "Recursos: %d", recursos);
        DrawText(recursosText, 545, 0, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(recursosText, 545 + 1, 1, 20, (Color){200, 255, 255, 255}); // Texto azul clara

        EndDrawing();
    }

    // Libera os recursos
    UnloadTexture(barricadatexture);
    UnloadTexture(paredetexture);
    CloseAudioDevice();

    return 0;
}

int main(void)
{
    runJogo();
    return 0;
}
