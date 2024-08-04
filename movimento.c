#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "movimento.h"
#include "mapa.h"
#include "Load.h"

#define LADO 20
#define LARGURA 1200
#define ALTURA 720
#define MAX_INIMIGOS 4
#define MAP_OFFSET 6  // Número de linhas em branco no topo
#define MAP_WIDTH 60  // Defina a largura do mapa
#define MAP_HEIGHT 40 // Defina a altura do mapa
#define MAX_BARRICADAS 100 // Número máximo de barricadas
#define MAX_MINAS 100 // Número máximo de minas
#define MAX_ARQUEIROS 100 // Número máximo de arqueiros
#define MAX_FLECHAS 100 // Número máximo de flechas

typedef struct {
    int x;
    int y;
    int id;
    int direcaoX;
    int direcaoY;
    int ultimaDirecaoX;
    int ultimaDirecaoY;
    double ultimoMovimento; // Tempo do último movimento
    int vida; // Variável de vida
} Inimigo;

typedef struct {
    int x;
    int y;
    int vida;
    double ultimoDano; // Tempo do último dano recebido
} Barricada;

typedef struct {
    int x;
    int y;
    double ultimoRecurso; // Tempo do último recurso produzido
} Mina;

typedef struct {
    int x;
    int y;
    double ultimoTiro; // Tempo do último tiro
} Arqueiro;

typedef struct {
    int x;
    int y;
    int direcaoX;
    int direcaoY;
    int ativa;
} Flecha;

typedef struct recurso {
    int x;
    int y;
} TYPE_RECURSO;

int podeMover(int xElemento, int yElemento, int dx, int dy) {
    int newX = xElemento + dx;
    int newY = yElemento + dy;

    // Verificar se a nova posição está dentro dos limites do mapa
    if (newX < 0 || newX >= MAP_WIDTH || newY < MAP_OFFSET || newY >= MAP_HEIGHT + MAP_OFFSET) {
        return 0; // Não pode mover
    }

    // Verificar se a nova posição é uma parede ou barricada
    if (map[newY - MAP_OFFSET][newX] == 'W' || map[newY - MAP_OFFSET][newX] == 'B' || map[newY - MAP_OFFSET][newX] == 'M') {
        return 0; // Não pode mover
    }

    return 1; // Pode mover
}

void move(int *x, int *y, int dx, int dy) {
    if (podeMover(*x, *y, dx, dy)) {
        *x += dx;
        *y += dy;
    }
}

void MoverInimigo(Inimigo *inimigo, int objetivoX, int objetivoY, int *novoX, int *novoY) {
    int movimentos[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int melhorDx = 0, melhorDy = 0;
    int menorDistancia = 10000;

    for (int i = 0; i < 4; i++) {
        int dx = movimentos[i][0];
        int dy = movimentos[i][1];
        int nextX = inimigo->x + dx;
        int nextY = inimigo->y + dy;

        if (podeMover(inimigo->x, inimigo->y, dx, dy) &&
            (dx != -inimigo->ultimaDirecaoX || dy != -inimigo->ultimaDirecaoY)) {
            int distancia = abs(objetivoX - nextX) + abs(objetivoY - nextY);
            if (distancia < menorDistancia) {
                menorDistancia = distancia;
                melhorDx = dx;
                melhorDy = dy;
            }
        }
    }

    if (melhorDx != 0 || melhorDy != 0) {
        *novoX = inimigo->x + melhorDx;
        *novoY = inimigo->y + melhorDy;
        inimigo->ultimaDirecaoX = melhorDx;
        inimigo->ultimaDirecaoY = melhorDy;
    } else {
        *novoX = inimigo->x;
        *novoY = inimigo->y;
    }
}

void buraco(int *xPersonagem, int *yPersonagem) {
    // Implementação da função buraco
}

void RemoverInimigo(Inimigo inimigos[], int *numInimigos, int index) {
    for (int i = index; i < *numInimigos - 1; i++) {
        inimigos[i] = inimigos[i + 1];
    }
    (*numInimigos)--;
}

void DrawHealthBar(int x, int y, int vida) {
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, RED);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, GREEN);
}

void DrawBarricadeHealthBar(int x, int y, int vida) {
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, BLUE);
}

int InimigoAoLado(Inimigo inimigos[], int numInimigos, int x, int y) {
    for (int i = 0; i < numInimigos; i++) {
        if ((inimigos[i].x == x + 1 && inimigos[i].y == y) ||
            (inimigos[i].x == x - 1 && inimigos[i].y == y) ||
            (inimigos[i].x == x && inimigos[i].y == y + 1) ||
            (inimigos[i].x == x && inimigos[i].y == y - 1)) {
            return 1;
        }
    }
    return 0;
}

int EncontrarInimigoMaisProximo(Inimigo inimigos[], int numInimigos, int x, int y) {
    int menorDistancia = 10000;
    int inimigoMaisProximo = -1;
    for (int i = 0; i < numInimigos; i++) {
        int distancia = abs(inimigos[i].x - x) + abs(inimigos[i].y - y);
        if (distancia < menorDistancia) {
            menorDistancia = distancia;
            inimigoMaisProximo = i;
        }
    }
    return inimigoMaisProximo;
}

void AtirarFlecha(Arqueiro *arqueiro, Flecha flechas[], int *numFlechas, Inimigo inimigos[], int numInimigos) {
    int inimigoMaisProximo = EncontrarInimigoMaisProximo(inimigos, numInimigos, arqueiro->x, arqueiro->y);
    if (inimigoMaisProximo != -1 && *numFlechas < MAX_FLECHAS) {
        Flecha *flecha = &flechas[*numFlechas];
        flecha->x = arqueiro->x;
        flecha->y = arqueiro->y;
        flecha->ativa = 1;
        int dx = inimigos[inimigoMaisProximo].x - arqueiro->x;
        int dy = inimigos[inimigoMaisProximo].y - arqueiro->y;
        if (dx != 0) flecha->direcaoX = dx / abs(dx);
        if (dy != 0) flecha->direcaoY = dy / abs(dy);
        (*numFlechas)++;
        arqueiro->ultimoTiro = GetTime();
    }
}

void AtualizarFlechas(Flecha flechas[], int *numFlechas, Inimigo inimigos[], int *numInimigos) {
    for (int i = 0; i < *numFlechas; i++) {
        if (flechas[i].ativa) {
            flechas[i].x += flechas[i].direcaoX;
            flechas[i].y += flechas[i].direcaoY;

            // Verifica se a flecha colidiu com algum inimigo
            for (int j = 0; j < *numInimigos; j++) {
                if (flechas[i].x == inimigos[j].x && flechas[i].y == inimigos[j].y) {
                    inimigos[j].vida--;
                    if (inimigos[j].vida <= 0) {
                        map[inimigos[j].y - MAP_OFFSET][inimigos[j].x] = '.'; // Atualizar a posição no mapa para '.'
                        RemoverInimigo(inimigos, numInimigos, j);
                    }
                    flechas[i].ativa = 0;
                    break;
                }
            }

            // Verifica se a flecha saiu do mapa
            if (flechas[i].x < 0 || flechas[i].x >= MAP_WIDTH || flechas[i].y < MAP_OFFSET || flechas[i].y >= MAP_HEIGHT + MAP_OFFSET) {
                flechas[i].ativa = 0;
            }
        }
    }
}

int main(void) {
    int recursos = 11;
    int xPersonagem = spawnx;
    int yPersonagem = spawny;

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

    LoadMap("mapa.txt");

    // Carregar a textura do inimigo
    Texture2D inimigotexture = LoadTexture("caminho/para/inimigotexture.png");
    // Carregar a textura da barricada
    Texture2D barricadatexture = LoadTexture("caminho/para/barricadatexture.png");
    // Carregar a textura da flecha
    Texture2D flechatexture = LoadTexture("caminho/para/flechatexture.png");

    int id = 0, i = 0;
    Inimigo Listainimigos[MAX_INIMIGOS];

    int objetivoX = -1, objetivoY = -1;
    for (int mapY = 0; mapY < MAP_HEIGHT; mapY++) {
        for (int mapX = 0; mapX < MAP_WIDTH; mapX++) {
            if (map[mapY][mapX] == 'M' && i < MAX_INIMIGOS) {
                Listainimigos[i].id = i;
                Listainimigos[i].x = mapX;
                Listainimigos[i].y = mapY + MAP_OFFSET;
                Listainimigos[i].ultimaDirecaoX = 0;
                Listainimigos[i].ultimaDirecaoY = 0;
                Listainimigos[i].ultimoMovimento = GetTime();
                Listainimigos[i].vida = 3; // Inicializa a vida
                printf("\nInimigo encontrado:\nid: %d\nx: %d\ny: %d\n", Listainimigos[i].id, Listainimigos[i].x, Listainimigos[i].y);
                i++;
            }
            if (map[mapY][mapX] == 'S') {
                objetivoX = mapX;
                objetivoY = mapY + MAP_OFFSET;
            }
        }
    }

    int numBarricadas = 0;
    Barricada barricadas[MAX_BARRICADAS];

    int numMinas = 0;
    Mina minas[MAX_MINAS];

    int numArqueiros = 0;
    Arqueiro arqueiros[MAX_ARQUEIROS];

    int numFlechas = 0;
    Flecha flechas[MAX_FLECHAS] = {0};

    while (!WindowShouldClose()) {
        double tempoAtual = GetTime();

        // Movimentos do personagem
        if (IsKeyPressed(KEY_RIGHT) && podeMover(xPersonagem, yPersonagem, 1, 0)) {
            move(&xPersonagem, &yPersonagem, 1, 0);
        }

        if (IsKeyPressed(KEY_LEFT) && podeMover(xPersonagem, yPersonagem, -1, 0)) {
            move(&xPersonagem, &yPersonagem, -1, 0);
        }

        if (IsKeyPressed(KEY_UP) && podeMover(xPersonagem, yPersonagem, 0, -1)) {
            move(&xPersonagem, &yPersonagem, 0, -1);
        }

        if (IsKeyPressed(KEY_DOWN) && podeMover(xPersonagem, yPersonagem, 0, 1)) {
            move(&xPersonagem, &yPersonagem, 0, 1);
        }

        int mapX = xPersonagem;
        int mapY = yPersonagem - MAP_OFFSET;

        // Verifica se o jogador encostou em um recurso (R)
        if (map[mapY][mapX] == 'R') {
            recursos++;
            map[mapY][mapX] = '.'; // Remove o recurso do mapa
        }

        // Verifica se o jogador encostou em um buraco (H)
        if (map[mapY][mapX] == 'H') {
            PlaySound(fxWav);
            buraco(&xPersonagem, &yPersonagem);
        }

        // Barricadas
        if (recursos >= 2 && map[mapY][mapX] == '.' && IsKeyPressed('E') && numBarricadas < MAX_BARRICADAS) {
            PlaySound(fxWav2);
            map[mapY][mapX] = 'B';
            barricadas[numBarricadas].x = mapX;
            barricadas[numBarricadas].y = mapY + MAP_OFFSET;
            barricadas[numBarricadas].vida = 3; // Inicializa a vida da barricada
            barricadas[numBarricadas].ultimoDano = tempoAtual; // Inicializa o tempo do último dano
            numBarricadas++;
        }

        // Bomba
        if (recursos >= 1 && map[mapY][mapX] == '.' && IsKeyPressed('G')) {
            PlaySound(fxWav2);
            map[mapY][mapX] = 'A';
        }

        // Mina
        if (recursos >= 5 && map[mapY][mapX] == '.' && IsKeyPressed('R') && numMinas < MAX_MINAS) {
            PlaySound(fxWav2);
            map[mapY][mapX] = '1';
            minas[numMinas].x = mapX;
            minas[numMinas].y = mapY + MAP_OFFSET;
            minas[numMinas].ultimoRecurso = tempoAtual; // Inicializa o tempo do último recurso produzido
            numMinas++;
        }

        // Arqueiro
        if (recursos >= 5 && map[mapY][mapX] == '.' && IsKeyPressed('F') && numArqueiros < MAX_ARQUEIROS) {
            PlaySound(fxWav2);
            map[mapY][mapX] = '2';
            arqueiros[numArqueiros].x = mapX;
            arqueiros[numArqueiros].y = mapY + MAP_OFFSET;
            arqueiros[numArqueiros].ultimoTiro = tempoAtual; // Inicializa o tempo do último tiro
            numArqueiros++;
        }

        int novosX[MAX_INIMIGOS];
        int novosY[MAX_INIMIGOS];

        for (int j = 0; j < i; j++) {
            if (tempoAtual - Listainimigos[j].ultimoMovimento >= 2.0) {
                MoverInimigo(&Listainimigos[j], objetivoX, objetivoY, &novosX[j], &novosY[j]);
                Listainimigos[j].ultimoMovimento = tempoAtual; // Atualiza o tempo do último movimento

                char tile = map[novosY[j] - MAP_OFFSET][novosX[j]];
                if (tile == 'B') {
                    // Encontra a barricada correspondente
                    for (int k = 0; k < numBarricadas; k++) {
                        if (barricadas[k].x == novosX[j] && barricadas[k].y == novosY[j]) {
                            barricadas[k].vida--;
                            if (barricadas[k].vida <= 0) {
                                map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualizar a posição no mapa para '.'
                                for (int l = k; l < numBarricadas - 1; l++) {
                                    barricadas[l] = barricadas[l + 1];
                                }
                                numBarricadas--;
                            }
                            break;
                        }
                    }
                } else if (novosX[j] == objetivoX && novosY[j] == objetivoY || tile == 'A') {
                    // Inimigo chegou à base 'S' ou encontrou uma bomba e deve ser removido
                    map[novosY[j] - MAP_OFFSET][novosX[j]] = '.'; // Atualizar a posição no mapa para '.'
                    map[Listainimigos[j].y - MAP_OFFSET][Listainimigos[j].x] = '.'; // Limpar a posição antiga no mapa
                    RemoverInimigo(Listainimigos, &i, j);
                    j--; // Ajustar índice após remoção
                } else if (Listainimigos[j].vida <= 0) {
                    // Inimigo morreu por causa da vida zerada
                    map[Listainimigos[j].y - MAP_OFFSET][Listainimigos[j].x] = '.'; // Limpar a posição antiga no mapa
                    RemoverInimigo(Listainimigos, &i, j);
                    j--; // Ajustar índice após remoção
                } else if (tile == '.') {
                    map[Listainimigos[j].y - MAP_OFFSET][Listainimigos[j].x] = '.'; // Limpar a posição antiga no mapa
                    Listainimigos[j].x = novosX[j];
                    Listainimigos[j].y = novosY[j];
                    map[novosY[j] - MAP_OFFSET][novosX[j]] = 'M'; // Atualizar a posição no mapa
                }
            }
        }

        // Verificar se há inimigos ao lado das barricadas e aplicar dano
        for (int k = 0; k < numBarricadas; k++) {
            if (InimigoAoLado(Listainimigos, i, barricadas[k].x, barricadas[k].y)) {
                if (tempoAtual - barricadas[k].ultimoDano >= 2.0) {
                    barricadas[k].vida--;
                    barricadas[k].ultimoDano = tempoAtual;
                    if (barricadas[k].vida <= 0) {
                        map[barricadas[k].y - MAP_OFFSET][barricadas[k].x] = '.'; // Atualizar a posição no mapa para '.'
                        for (int l = k; l < numBarricadas - 1; l++) {
                            barricadas[l] = barricadas[l + 1];
                        }
                        numBarricadas--;
                    }
                }
            }
        }

        // Produzir recursos nas minas a cada 5 segundos
        for (int m = 0; m < numMinas; m++) {
            if (tempoAtual - minas[m].ultimoRecurso >= 5.0) {
                recursos++;
                minas[m].ultimoRecurso = tempoAtual;
            }
        }

        // Arqueiros atirando flechas
        for (int a = 0; a < numArqueiros; a++) {
            if (tempoAtual - arqueiros[a].ultimoTiro >= 5.0) {
                AtirarFlecha(&arqueiros[a], flechas, &numFlechas, Listainimigos, i);
            }
        }

        // Atualizar flechas
        AtualizarFlechas(flechas, &numFlechas, Listainimigos, &i);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawMap();

        // Desenha os inimigos e suas barras de vida
        for (int j = 0; j < i; j++) {
            DrawTexture(inimigotexture, Listainimigos[j].x * LADO, Listainimigos[j].y * LADO, WHITE);
            DrawHealthBar(Listainimigos[j].x, Listainimigos[j].y, Listainimigos[j].vida);
        }

        // Desenha as barricadas e suas barras de vida
        for (int k = 0; k < numBarricadas; k++) {
            DrawTexture(barricadatexture, barricadas[k].x * LADO, barricadas[k].y * LADO, WHITE);
            DrawBarricadeHealthBar(barricadas[k].x, barricadas[k].y, barricadas[k].vida);
        }



        // Desenha as flechas
        for (int f = 0; f < numFlechas; f++) {
            if (flechas[f].ativa) {
                DrawTexture(flechatexture, flechas[f].x * LADO, flechas[f].y * LADO, WHITE);
            }
        }

        // Desenho da HUD
        DrawTexture(vidahudtexture, 20, 40, WHITE);
        DrawTexture(vidahudtexture, 40, 40, WHITE);
        DrawTexture(vidahudtexture, 60, 40, WHITE);
        DrawTexture(bombahudtexture, 200, 20, WHITE);
        DrawTexture(barricadahudtexture, 300, 20, WHITE);
        DrawTexture(minahudtexture, 400, 20, WHITE);
        DrawTexture(arqueirohudtexture, 500, 20, WHITE);
        DrawTexture(recursohudtexture, 30, 80, WHITE);
        DrawRectangle(xPersonagem * LADO, yPersonagem * LADO, LADO, LADO, RED);
        DrawText(TextFormat("x %d", recursos), 55, 80, 20, BLACK);
        DrawText("Wave", 15, 30, 20, BLACK);

        // Desenha o contador de segundos no canto da tela
        char buffer[50];
        sprintf(buffer, "Tempo: %.2f", GetTime());
        DrawText(buffer, 10, 5, 20, BLACK);

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
