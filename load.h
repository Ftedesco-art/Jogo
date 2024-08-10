//=== LOAD.H ===
//Relacionado ao carregamento/descarregamento de arquivos.
#ifndef LOAD_H
#define LOAD_H

#include "raylib.h"

// TEXTURAS DO MAPA
extern Texture2D chaotexture;         // Textura do chão
extern Texture2D paredetexture;       // Textura da parede
extern Texture2D buracotexture;       // Textura do buraco
extern Texture2D recursotexture;      // Textura do recurso (gema)
extern Texture2D barricadatexture;    // Textura da barricada
extern Texture2D spawntexture;        // Textura da posição inicial do jogador (spawn)
extern Texture2D basetexture;         // Textura da base do jogador
extern Texture2D inimigotexture;      // Textura do inimigo
extern Texture2D minatexture;         //Textura da Mina

// TEXTURAS DE HUD
extern Texture2D bombahudtexture;     // Textura do ícone da bomba
extern Texture2D vidahudtexture;
extern Texture2D vidabasehudtexture;       // Textura do ícone da vida do jogador
extern Texture2D barricadahudtexture; // Textura do ícone da barricada
extern Texture2D recursohudtexture;
extern Texture2D arqueirohudtexture;
extern Texture2D minahudtexture;
extern Texture2D flechadireitatexture;
extern Texture2D flechaesquerdatexture;
extern Texture2D flechacimatexture;
extern Texture2D flechabaixotexture;
extern Texture2D arqueirotexture;

// TEXTURAS DE ITENS
extern Texture2D bombatexture;        // Textura da bomba no mapa

// TEXTURAS DE ANIMAÇÃO
extern Texture2D explosaotexture;     // Sprites relacionados à animação de explosão

void CarregarTexturas();
void DescarregarTexturas();

#endif // LOAD_H
