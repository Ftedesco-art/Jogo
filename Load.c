#include "load.h"


// TEXTURAS DO MAPA
Texture2D chaotexture;
Texture2D paredetexture;
Texture2D buracotexture;
Texture2D recursotexture;
Texture2D barricadatexture;
Texture2D spawntexture;
Texture2D basetexture;
Texture2D spawninimigotexture;
Texture2D caminho1texture;
Texture2D caminho2texture;
Texture2D caminho3texture;
Texture2D caminho4texture;
Texture2D inimigotexture;
Texture2D minatexture;
Texture2D flechatexture;
Texture2D arqueirotexture;


// TEXTURAS DE HUD
Texture2D bombahudtexture;
Texture2D vidahudtexture;
Texture2D barricadahudtexture;
Texture2D recursohudtexture;
Texture2D arqueirohudtexture;
Texture2D minahudtexture;
// TEXTURAS DE ITENS
Texture2D bombatexture;

// TEXTURAS DE ANIMAÇÃO
Texture2D explosaotexture;

void CarregarTexturas()
{
    arqueirotexture = LoadTexture("Texturas\\Arqueiro.png");
    flechatexture = LoadTexture("Texturas\\Flecha.png");
    minahudtexture = LoadTexture("Texturas\\Minahud.png");
    arqueirohudtexture = LoadTexture("Texturas\\Arqueirohud.png");
    minatexture = LoadTexture("Texturas\\Mina.png");
    recursohudtexture = LoadTexture("Texturas\\Recursohud.png");
    chaotexture = LoadTexture("Texturas\\Chao.png");
    paredetexture = LoadTexture("Texturas\\Parede.png");
    buracotexture = LoadTexture("Texturas\\Buraco.png");
    recursotexture = LoadTexture("Texturas\\Recurso.png");
    barricadatexture = LoadTexture("Texturas\\Barricada.png");
    spawntexture = LoadTexture("Texturas\\Spawn.png");
    basetexture = LoadTexture("Texturas\\Base.png");
    spawninimigotexture = LoadTexture("Texturas\\Spawninimigo.png");
    caminho1texture = LoadTexture("Texturas\\Horizontal.png");
    caminho2texture = LoadTexture("Texturas\\Vertical.png");
    caminho3texture = LoadTexture("Texturas\\Curva1.png");
    caminho4texture = LoadTexture("Texturas\\Curva2.png");
    bombahudtexture = LoadTexture("Texturas\\Bombahud.png");
    vidahudtexture = LoadTexture("Texturas\\Vida.png");
    barricadahudtexture = LoadTexture("Texturas\\Barricadahud.png");
    bombatexture = LoadTexture("Texturas\\Bomba.png");
    explosaotexture = LoadTexture("Texturas\\Explosao.png");
    inimigotexture = LoadTexture("Texturas\\Inimigo.png");
}


void DescarregarTexturas()
{
    // TEXTURAS DO MAPA
    UnloadTexture(chaotexture);
    UnloadTexture(paredetexture);
    UnloadTexture(buracotexture);
    UnloadTexture(recursotexture);
    UnloadTexture(barricadatexture);
    UnloadTexture(spawntexture);
    UnloadTexture(basetexture);
    UnloadTexture(spawninimigotexture);
    UnloadTexture(caminho1texture);
    UnloadTexture(caminho2texture);
    UnloadTexture(caminho3texture);
    UnloadTexture(caminho4texture);
    UnloadTexture(inimigotexture);
    UnloadTexture(minatexture);
    UnloadTexture(arqueirotexture);

    // TEXTURAS DE HUD
    UnloadTexture(bombahudtexture);
    UnloadTexture(vidahudtexture);
    UnloadTexture(barricadahudtexture);
    UnloadTexture(recursohudtexture);
    UnloadTexture(arqueirohudtexture);
    UnloadTexture(minahudtexture);

    // TEXTURAS DE ITENS
    UnloadTexture(bombatexture);
    UnloadTexture(flechatexture);

    // TEXTURAS DE ANIMAÇÃO
    UnloadTexture(explosaotexture);
}

