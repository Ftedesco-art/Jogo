#include "hud.h"
#include "main.h"

void drawBarraDeVidaInimigo(int x, int y, int vida)
{
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, RED);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, GREEN);
}

void drawBarraDeVidaBarricada(int x, int y, int vida)
{
    int barWidth = LADO;
    int barHeight = 5;
    int barX = x * LADO;
    int barY = y * LADO - barHeight - 2;

    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, barWidth * vida / 3, barHeight, BLUE);
}

void hud(void)
{
    //TEXTOS DA HUD
    DrawText(TextFormat("x %d", recursos), 55, 80, 20, BLACK);
    char buffer[50];
    sprintf(buffer, "Tempo: %.2f", GetTime());
    DrawText(buffer, 10, 5, 20, BLACK);
    DrawText("Wave", 15, 30, 20, BLACK);

    //TEXTURAS DA HUD
    DrawTexture(vidahudtexture, 20, 40, WHITE);
    DrawTexture(vidahudtexture, 40, 40, WHITE);
    DrawTexture(vidahudtexture, 60, 40, WHITE);
    DrawTexture(bombahudtexture, 200, 20, WHITE);
    DrawTexture(barricadahudtexture, 300, 20, WHITE);
    DrawTexture(minahudtexture, 400, 20, WHITE);
    DrawTexture(arqueirohudtexture, 500, 20, WHITE);
    DrawTexture(recursohudtexture, 30, 80, WHITE);

}

