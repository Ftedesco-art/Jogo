#ifndef UTIL_H
#define UTIL_H

#include "raylib.h"

extern Texture2D vidahudtexture;
extern Texture2D bombahudtexture;
extern Texture2D barricadahudtexture;
extern Texture2D minahudtexture;
extern Texture2D arqueirohudtexture;
extern Texture2D recursohudtexture;
extern int recursos;
extern int xPersonagem;
extern int yPersonagem;
#define LADO 20

void DrawHealthBar(int x, int y, int vida);
void DrawBarricadeHealthBar(int x, int y, int vida);
void hud(void);

#endif // UTIL_H

