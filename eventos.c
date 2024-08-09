#include "hud.h"
#include "main.h"
#include "eventos.h"
#include "mapa.h"

#include <stdio.h>

#define MAP_OFFSET 6

void reiniciarJogo(INIMIGO array_inimigos[MAX_INIMIGOS], JOGADOR *player,
                   BASE *base, BARRICADA barricadas[MAX_BARRICADAS],
                   int *numBarricadas, MINA minas[MAX_MINAS],
                   int *numMinas, ARQUEIRO arqueiros[MAX_ARQUEIROS],
                   int *numArqueiros, FLECHA flechas[MAX_FLECHAS],
                   int *numFlechas, int *numInimigos, int *recursos,
                   int *tempo) {
    // Carrega o mapa novamente
    LoadMap("mapa1.txt");

    DescarregarTexturas();

    // Reinicia os contadores
    *numInimigos = 0;
    *numBarricadas = 0;
    *numMinas = 0;
    *numArqueiros = 0;
    *numFlechas = 0;
    *recursos = 5;

    // Zera as estruturas
    memset(barricadas, 0, sizeof(BARRICADA) * MAX_BARRICADAS);
    memset(flechas, 0, sizeof(FLECHA) * MAX_FLECHAS);
    memset(minas, 0, sizeof(MINA) * MAX_MINAS);
    memset(arqueiros, 0, sizeof(ARQUEIRO) * MAX_ARQUEIROS);
    memset(array_inimigos, 0, sizeof(INIMIGO) * MAX_INIMIGOS);

    // Inicializa inimigos, base e player
    for (int mapY = 0; mapY < ALTURA_GRID; mapY++) {
        for (int mapX = 0; mapX < LARGURA_GRID; mapX++) {
            if (map[mapY][mapX] == 'M' && *numInimigos < MAX_INIMIGOS) {
                array_inimigos[*numInimigos] = (INIMIGO) {
                    .coord = {mapX, mapY + MAP_OFFSET},
                    .ultimaDirecao = {0, 0},
                    .ultimoMovimento = GetTime(),
                    .vida = 3
                };
                (*numInimigos)++;
            } else if (map[mapY][mapX] == 'S') {
                base->coord.x = mapX;
                base->coord.y = mapY + MAP_OFFSET;
                base->vidas = 3;
            } else if (map[mapY][mapX] == 'J') {
                player->coord.x = mapX;
                player->coord.y = mapY + MAP_OFFSET;
                player->vidas = 3;
            }
        }
    }

    *tempo = 0;
}

void salva_dados(char map[ALTURA_GRID][LARGURA_GRID + 1], INIMIGO array_inimigos[MAX_INIMIGOS],
                  JOGADOR *player, BASE *base, BARRICADA barricadas[MAX_BARRICADAS],
                  int numBarricadas, MINA minas[MAX_MINAS], int numMinas,
                  ARQUEIRO arqueiros[MAX_ARQUEIROS], int numArqueiros,
                  FLECHA flechas[MAX_FLECHAS], int numFlechas,
                  int numInimigos, int recursos, int tempo) {
    // Copia o mapa
    memcpy(info.map, map, sizeof(info.map));

    // Copia todas as informações dos inimigos
    memcpy(info.array_inimigos, array_inimigos, sizeof(info.array_inimigos));

    // Copia todas as informações dos posicionáveis
    memcpy(info.barricadas, barricadas, sizeof(info.barricadas));
    memcpy(info.flechas, flechas, sizeof(info.flechas));
    memcpy(info.minas, minas, sizeof(info.minas));
    memcpy(info.arqueiros, arqueiros, sizeof(info.arqueiros));

    // Copia todas as informações de jogador e base
    info.player = *player;
    info.base = *base;

    // Copia outras informações
    info.tempo = tempo;
    info.numArqueiros = numArqueiros;
    info.numBarricadas = numBarricadas;
    info.numFlechas = numFlechas;
    info.numInimigos = numInimigos;
    info.numMinas = numMinas;
    info.recursos = recursos;

    // Flag de que são dados salvos
    info.flag = 1;
}

int carrega_dados(const char* arquivo, char map[ALTURA_GRID][LARGURA_GRID + 1],
                  INIMIGO array_inimigos[MAX_INIMIGOS], JOGADOR *player,
                  BASE *base, BARRICADA barricadas[MAX_BARRICADAS],
                  int *numBarricadas, MINA minas[MAX_MINAS],
                  int *numMinas, ARQUEIRO arqueiros[MAX_ARQUEIROS],
                  int *numArqueiros, FLECHA flechas[MAX_FLECHAS],
                  int *numFlechas, int *numInimigos, int *recursos,
                  int *tempo) {
    FILE *ponteiro = fopen(arquivo, "rb");
    if (ponteiro == NULL) {
        printf("\n\nErro na abertura do arquivo - leitura\n\n");
        return 0;
    }

    // Ler a estrutura INFO do arquivo
    INFO estado;
    size_t read_count = fread(&estado, sizeof(INFO), 1, ponteiro);
    if (read_count != 1) {
        printf("\n\nErro na leitura dos dados do arquivo\n\n");
        fclose(ponteiro);
        return 0;
    }
    fclose(ponteiro);

    // Passar os dados para as variáveis correspondentes
    memcpy(map, estado.map, sizeof(estado.map));
    memcpy(array_inimigos, estado.array_inimigos, sizeof(estado.array_inimigos));

    *player = estado.player;
    *base = estado.base;

    memcpy(barricadas, estado.barricadas, sizeof(estado.barricadas));
    *numBarricadas = estado.numBarricadas;

    memcpy(minas, estado.minas, sizeof(estado.minas));
    *numMinas = estado.numMinas;

    memcpy(arqueiros, estado.arqueiros, sizeof(estado.arqueiros));
    *numArqueiros = estado.numArqueiros;

    memcpy(flechas, estado.flechas, sizeof(estado.flechas));
    *numFlechas = estado.numFlechas;

    *numInimigos = estado.numInimigos;
    *recursos = estado.recursos;
    *tempo = estado.tempo;

    return 1;
}

// Função para salvar o estado do jogo em um arquivo binário
int salva_jogo(const char* arquivo, INFO* estado) {
    FILE *ponteiro = fopen(arquivo, "wb");
    if (ponteiro == NULL) {
        printf("\n\nErro na abertura do arquivo - escrita\n\n");
        return 0;
    }
    size_t written_count = fwrite(estado, sizeof(INFO), 1, ponteiro);
    fclose(ponteiro);

    if (written_count != 1) {
        printf("\n\nErro ao salvar os dados do jogo\n\n");
        return 0;
    }

    printf("\n\nEstado do jogo salvo com sucesso em '%s'.\n\n", arquivo);
    return 1;
}

bool menu_inicial() {
    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("MENU PRINCIPAL", LARGURA / 2 - 70, ALTURA / 2 - 50, 20, BLACK);
        DrawText("Pressione N para Novo Jogo", LARGURA / 2 - 150, ALTURA / 2, 20, BLACK);
        DrawText("Pressione C para Carregar Jogo", LARGURA / 2 - 150, ALTURA / 2 + 30, 20, BLACK);
        DrawText("Pressione Q para Sair", LARGURA / 2 - 150, ALTURA / 2 + 60, 20, BLACK);
        EndDrawing();

        // Verifica as entradas do usuário
        if (IsKeyPressed(KEY_N)) {
            info.flag = 0; // Significa que o jogo é novo
            run();
            return true;
        } else if (IsKeyPressed(KEY_C)) {
            info.flag = 1; // Significa que o jogo é novo
            run();
            return true;
        } else if (IsKeyPressed(KEY_Q)) {
            return false;
        }
    }
}

bool menu_pause(bool* paused) {
    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("MENU PAUSE", LARGURA / 2 - 50, ALTURA / 2 - 50, 20, BLACK);
        DrawText("Pressione C para Continuar", LARGURA / 2 - 150, ALTURA / 2, 20, BLACK);
        DrawText("Pressione L para Carregar Jogo", LARGURA / 2 - 150, ALTURA / 2 + 30, 20, BLACK);
        DrawText("Pressione S para Salvar Jogo", LARGURA / 2 - 150, ALTURA / 2 + 60, 20, BLACK);
        DrawText("Pressione V para Voltar ao Menu Inicial", LARGURA / 2 - 150, ALTURA / 2 + 90, 20, BLACK);
        EndDrawing();

        if (IsKeyPressed(KEY_C)) {
            *paused = false; // Muda o estado de pausa para false
            return true; // Sai do menu de pausa
        } else if (IsKeyPressed(KEY_L)) {
            info.flag = 1;
            run();
            return true;
        } else if (IsKeyPressed(KEY_S)) {
            salva_jogo("jogo_salvo.bin", &info);
            return true;
        } else if (IsKeyPressed(KEY_V)) {
            *paused = false;
            return false; // Retorna ao menu inicial
        }
    }
}

void atualizarFlechas(FLECHA flechas[], int *numFlechas, INIMIGO inimigos[], int *numInimigos)
{
    for (int i = 0; i < *numFlechas; i++)
    {
        if (flechas[i].ativa)
        {
            flechas[i].coord.x += flechas[i].direcao.x;
            flechas[i].coord.y += flechas[i].direcao.y;

            int mapX = flechas[i].coord.x;
            int mapY = flechas[i].coord.y - MAP_OFFSET;

            // Verifica se a flecha colidiu com algum inimigo
            for (int j = 0; j < *numInimigos; j++)
            {
                if (flechas[i].coord.x == inimigos[j].coord.x && flechas[i].coord.y == inimigos[j].coord.y)
                {
                    inimigos[j].vida--;
                    if (inimigos[j].vida <= 0)
                    {
                        map[inimigos[j].coord.y - MAP_OFFSET][inimigos[j].coord.x] = '.'; // Atualizar a posição no mapa para '.'
                        removerInimigo(inimigos, numInimigos, j);
                    }
                    flechas[i].ativa = 0;
                    break;
                }
            }

            // Verifica se a flecha colidiu com uma parede (tile 'W')
            if (map[mapY][mapX] == 'W')
            {
                flechas[i].ativa = 0;
            }

            // Verifica se a flecha saiu do mapa
            if (flechas[i].coord.x < 0 || flechas[i].coord.x >= LARGURA_GRID || flechas[i].coord.y < MAP_OFFSET || flechas[i].coord.y >= ALTURA_GRID + MAP_OFFSET)
            {
                flechas[i].ativa = 0;
            }
        }
    }
}

int encontrarInimigoMaisProximo(INIMIGO inimigos[], int numInimigos, int x, int y)
{
    int menorDistancia = 10000;
    int inimigoMaisProximo = -1;
    for (int i = 0; i < numInimigos; i++)
    {
        int distancia = abs(inimigos[i].coord.x - x) + abs(inimigos[i].coord.y - y);
        if (distancia < menorDistancia)
        {
            menorDistancia = distancia;
            inimigoMaisProximo = i;
        }
    }
    return inimigoMaisProximo;
}

void atirarFlecha(ARQUEIRO *arqueiro, FLECHA flechas[], int *numFlechas, INIMIGO inimigos[], int numInimigos)
{
    int inimigoMaisProximo = encontrarInimigoMaisProximo(inimigos, numInimigos, arqueiro->coord.x, arqueiro->coord.y);

    if (inimigoMaisProximo >= 0 && *numFlechas < MAX_FLECHAS)
    {
        int dx = inimigos[inimigoMaisProximo].coord.x - arqueiro->coord.x;
        int dy = inimigos[inimigoMaisProximo].coord.y - arqueiro->coord.y;

        // Calcula a distância do inimigo mais próximo
        int distancia = abs(dx) + abs(dy);

        // Verifica se o inimigo está dentro do alcance de 5 tiles
        if (distancia <= 5)
        {
            FLECHA *flecha = &flechas[*numFlechas];
            flecha->coord.x = arqueiro->coord.x;
            flecha->coord.y = arqueiro->coord.y;
            flecha->ativa = 1;
            flecha->direcao.x = (dx != 0) ? dx / abs(dx) : 0;
            flecha->direcao.y = (dy != 0) ? dy / abs(dy) : 0;
            (*numFlechas)++;
            arqueiro->ultimoTiro = GetTime();
        }
    }
}

int podeMover(int xElemento, int yElemento, int dx, int dy)
{
    int newX = xElemento + dx;
    int newY = yElemento + dy;

    // Verificar se a nova posição está dentro dos limites do mapa
    if (newX < 0 || newX >= LARGURA_GRID || newY < MAP_OFFSET || newY >= ALTURA_GRID + MAP_OFFSET)
    {
        return 0; // Não pode mover
    }

    // Verificar se a nova posição é uma parede ou barricada
    if (map[newY - MAP_OFFSET][newX] == 'W' || map[newY - MAP_OFFSET][newX] == 'B' || map[newY - MAP_OFFSET][newX] == 'M')
    {
        return 0; // Não pode mover
    }

    return 1; // Pode mover
}

void move(int *x, int *y, int dx, int dy)
{
    if (podeMover(*x, *y, dx, dy))
    {
        *x += dx;
        *y += dy;
    }
}

