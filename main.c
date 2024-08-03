#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "mapa.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 600
#define TEMPO_MENSAGEM 120 // 2 segundos (a 60 FPS)
#define MAX_INIMIGOS 10 // Número máximo de inimigos

typedef struct
{
    int x;
    int y;
} POSN;

typedef struct
{
    POSN coord;
    int vidas;
    int recursos;
} PLAYER;

typedef struct
{
    int movementStyle;
    int velocity;
    char color[20];
} TYPE_INIMIGO;

typedef struct
{
    POSN coord;
    POSN delta;
    bool isAlive;
    char next;
    TYPE_INIMIGO type;
} INIMIGO;

typedef struct
{
    POSN coord;
    int vidas;
} BASE;


// Função para mover um elemento
void move(int *x, int *y, int dx, int dy)
{
    *x += dx;
    *y += dy;
}

// Função para verificar se é possível mover
int podeMover(int x, int y, int dx, int dy)
{
    // Verifica se a nova posição é uma parede ou se não há movimento
    if (map[y + dy][x + dx] == 'W' || (dx == 0 && dy == 0))
    {
        return 0; // Não pode mover
    }

    return 1; // Pode mover
}

Texture2D baseTexture;
Texture2D chaoTexture;
Texture2D paredeTexture;
Texture2D buracoTexture;
Texture2D recursoTexture;
Texture2D barricadaTexture;

// Função principal do jogo
int runJogo(char mapa[50], int xPersonagem, int yPersonagem, int NUM_INIMIGOS,
            char texturaBase[50], char texturaChao[50], char texturaParede[50],
            char texturaBuraco[50], char texturaRecurso[50], char texturaBarricada[50])
{
    srand(time(NULL));

    // Inicializa variáveis básicas
    PLAYER player;
    BASE base;
    INIMIGO array_inimigos[MAX_INIMIGOS];
    TYPE_INIMIGO type;
    int contador = 0, i;

    // Inicializa o jogador
    player.coord.x = xPersonagem;
    player.coord.y = yPersonagem;
    player.vidas = 3;
    player.recursos = 0;

    // Inicializa os sons
    InitAudioDevice();
    Sound fxWav = LoadSound("Sons\\Buraco1.ogg");
    Sound fxWav2 = LoadSound("Sons\\Barricada1.ogg");
    SetSoundVolume(fxWav, 0.1f);
    SetSoundVolume(fxWav2, 0.1f);

    // Carrega o mapa
    LoadMap(mapa);

    // Inicializa inimigos e base
    for (int j = 0; j < MAP_HEIGHT; j++)
    {
        for (int i = 0; i < MAP_WIDTH; i++)
        {
            if (map[j][i] == 'E' && contador < NUM_INIMIGOS)
            {
                array_inimigos[contador] = (INIMIGO){ .coord = {i, j}, .isAlive = true, .delta = {0, 0}, .next = '.' };
                contador++;
            }
            else if (map[j][i] == 'S')
            {
                base.coord.x = i;
                base.coord.y = j;
                base.vidas = 3;
            }
        }
    }

    InitWindow(LARGURA, ALTURA, "Pagina principal - Fase 1");
    SetTargetFPS(60);

    bool isPaused = false;
    int contadorMovimentos = 0;
    int tempoTrap = 0, trapAtivada = 0;
    int tempoMensagem = 0, mensagemAtivada = 0;
    bool flagGameOver = false;
    int cooldownVida = 0;
    int tempoGameOver = 0;
    int velocidadeAumentada = 5;
    int cooldownInimigos = 0; // Cooldown padrão dos inimigos

    // Inicializa texturas
    baseTexture = LoadTexture(texturaBase);
    chaoTexture = LoadTexture(texturaChao);
    paredeTexture = LoadTexture(texturaParede);
    buracoTexture = LoadTexture(texturaBuraco);
    recursoTexture = LoadTexture(texturaRecurso);
    barricadaTexture = LoadTexture(texturaBarricada);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            isPaused = !isPaused;
        }

        if (!isPaused && !flagGameOver)
        {
            // Movimentação do personagem
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && podeMover(player.coord.x, player.coord.y, 1, 0))
            {
                move(&player.coord.x, &player.coord.y, 1, 0);
            }

            if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && podeMover(player.coord.x, player.coord.y, -1, 0))
            {
                move(&player.coord.x, &player.coord.y, -1, 0);
            }

            if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && podeMover(player.coord.x, player.coord.y, 0, -1))
            {
                move(&player.coord.x, &player.coord.y, 0, -1);
            }

            if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && podeMover(player.coord.x, player.coord.y, 0, 1))
            {
                move(&player.coord.x, &player.coord.y, 0, 1);
            }

            // Interações do jogador
            switch (map[player.coord.y][player.coord.x])
            {
                case 'R':
                    player.recursos++;
                    map[player.coord.y][player.coord.x] = '.'; // Remove o recurso
                    break;
                case 'H':
                    PlaySound(fxWav);
                    break;
                case 'T':
                    trapAtivada = 1; // Ativa a trap
                    tempoTrap = 0; // Reinicia o contador de tempo
                    mensagemAtivada = 1; // Ativa a mensagem
                    tempoMensagem = 0; // Reinicia o contador da mensagem
                    break;
            }

            if (trapAtivada)
            {
                tempoTrap++;
                if (tempoTrap >= 100)
                {
                    trapAtivada = 0; // Desativa a trap após 100 iterações
                }
            }

            contadorMovimentos ++;
            if (contadorMovimentos % (trapAtivada ? 10 / velocidadeAumentada : 10) == 0)
            {
                for (i = 0; i < NUM_INIMIGOS; i++)
                {
                    if (array_inimigos[i].isAlive)
                    {
                        while(!podeMover(array_inimigos[i].coord.x, array_inimigos[i].coord.y, array_inimigos[i].delta.x, array_inimigos[i].delta.y)) {
                            // Gera novos valores para delta.x e delta.y
                            array_inimigos[i].delta.x = rand() % 3 - 1; // Pode ser -1, 0 ou 1
                            array_inimigos[i].delta.y = rand() % 3 - 1; // Pode ser -1, 0 ou 1
                        }
                         map[array_inimigos[i].coord.y][array_inimigos[i].coord.x] = array_inimigos[i].next;

                        // Guarda o char do próximo movimento do inimigo
                        array_inimigos[i].next = map[array_inimigos[i].coord.y + array_inimigos[i].delta.y][array_inimigos[i].coord.x + array_inimigos[i].delta.x];

                        // Muda o char do próximo
                        map[array_inimigos[i].coord.y + array_inimigos[i].delta.y][array_inimigos[i].coord.x + array_inimigos[i].delta.x] = 'E';

                        // Move o inimigo
                        move(&array_inimigos[i].coord.x, &array_inimigos[i].coord.y, array_inimigos[i].delta.x, array_inimigos[i].delta.y);
                    }
                }
            }

            // Atualiza a lógica da mensagem
            if (mensagemAtivada)
            {
                tempoMensagem++;
                if (tempoMensagem >= TEMPO_MENSAGEM)
                {
                    mensagemAtivada = 0; // Desativa a mensagem após 2 segundos
                }
            }

            for (int i = 0; i < NUM_INIMIGOS; i++)
            {
                // Quando o inimigo toca na base
                if (array_inimigos[i].next == 'S' && array_inimigos[i].isAlive)
                {
                    // Tira vida da base
                    base.vidas--;

                    // Mata o inimigo
                    map[array_inimigos[i].coord.y][array_inimigos[i].coord.x] = array_inimigos[i].next;
                    array_inimigos[i].coord.x = -1;
                    array_inimigos[i].coord.y = -1;
                    array_inimigos[i].isAlive = false;
                }

                // Quando o inimigo toca no jogador
                if (array_inimigos[i].coord.y == player.coord.y && array_inimigos[i].coord.x == player.coord.x && array_inimigos[i].isAlive && cooldownVida <= 0)
                {
                    // Tira vida do jogador
                    player.vidas--;
                    cooldownVida = 60;
                }
            }

            cooldownVida--;

            // Verifica se a vida da base chegou a zero
            if (base.vidas <= 0 || player.vidas <= 0)
            {
                flagGameOver = true; // Ativa o estado de GameOver
                tempoGameOver = 0; // Reinicia o contador de tempo do GameOver
            }
        }

        // Estado de GameOver
        if (flagGameOver)
        {
            tempoGameOver++;
            if (tempoGameOver >= 120) // Mostra a mensagem por 2 segundos
            {
                CloseWindow(); // Fecha a janela após 2 segundos
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenho do mapa
        DrawMap();

        // Desenho do personagem
        DrawRectangle(player.coord.x * LADO, player.coord.y * LADO, LADO, LADO, WHITE);

        // Desenho das barricadas
        if(player.recursos >= 2 && map[player.coord.y][player.coord.x] == '.' && IsKeyPressed(KEY_B))
        {
            // Som
            PlaySound(fxWav2);

            // Constroi uma barricada no local do personagem
            map[player.coord.y][player.coord.x] = 'B';

            // Reduz recursos
            player.recursos -= 2;
        }

        // Desenho dos inimigos
        for (i = 0; i < NUM_INIMIGOS; i++)
        {
            if(array_inimigos[i].isAlive)
            {
                DrawRectangle(array_inimigos[i].coord.x * LADO, array_inimigos[i].coord.y * LADO, LADO, LADO, RED);
            }
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

        // Pausa do jogo
        if (isPaused)
        {
            Color textColor = DARKBLUE;

            // Desenha os textos centralizados
            DrawText("PAUSADO", LARGURA / 2 - MeasureText("PAUSADO", 40) / 2 + 2, ALTURA / 2 - 20 - 30 + 2, 40, BLACK);
            DrawText("PAUSADO", LARGURA / 2 - MeasureText("PAUSADO", 40) / 2, ALTURA / 2 - 20 - 30, 40, textColor);
            DrawText("Pressione Q para Sair", LARGURA / 2 - MeasureText("Pressione Q para Sair", 20) / 2 + 2, ALTURA / 2 - 15 + 30 + 2, 20, BLACK);
            DrawText("Pressione Q para Sair", LARGURA / 2 - MeasureText("Pressione Q para Sair", 20) / 2, ALTURA / 2 - 15 + 30, 20, textColor);

            // Se o usuário pressiona Q, sai do jogo
            if (IsKeyPressed('Q'))
            {
                return 0;
            }
        }

        // Desenho da posição do personagem
        char posText[50];
        sprintf(posText, "Pos: (%d, %d)", player.coord.x, player.coord.y);
        DrawText(posText, 5 + 1, 1, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(posText, 5, 0, 20, (Color){200, 200, 200, 255}); // Texto cinza

        // Desenho das vidas do jogador
        char vidaJogadorText[50];
        sprintf(vidaJogadorText, "Vidas do jogador: %d", player.vidas);
        DrawText(vidaJogadorText, 130 + 1, 1, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(vidaJogadorText, 130, 0, 20, (Color){255, 255, 255, 255}); // Texto branco

        // Desenho das vidas da base
        char vidaBaseText[50];
        sprintf(vidaBaseText, "Vidas da base: %d", base.vidas);
        DrawText(vidaBaseText, 350 + 1, 1, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(vidaBaseText, 350, 0, 20, (Color){255, 255, 200, 255}); // Texto amarelo claro

        // Desenho do número de recursos
        char recursosText[50];
        sprintf(recursosText, "Recursos: %d", player.recursos);
        DrawText(recursosText, 545 + 1, 1, 20, (Color){0, 0, 0, 255}); // Sombra preta
        DrawText(recursosText, 545, 0, 20, (Color){200, 255, 255, 255}); // Texto azul claro

        EndDrawing();
    }

    // Libera as texturas
    UnloadTexture(baseTexture);
    UnloadTexture(chaoTexture);
    UnloadTexture(paredeTexture);
    UnloadTexture(buracoTexture);
    UnloadTexture(recursoTexture);
    UnloadTexture(barricadaTexture);
    CloseAudioDevice();

    return 0;
}

int main()
{
    runJogo("mapa1.txt", 2, 2, 7, "Texturas\\Base1.png", "Texturas\\Chao1.png", "Texturas\\Parede1.png", "Texturas\\Buraco1.png", "Texturas\\Recurso1.png", "Texturas\\Barricada1.png"); // Fase 1
}
