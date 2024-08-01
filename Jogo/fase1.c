#define MAX_BURACOS 2 // Número máximo de buracos




int buraco(int *xPersonagem, int *yPersonagem)
{

    if(*xPersonagem == 27 && *yPersonagem == 8)
    {
       *xPersonagem = 30;
       *yPersonagem = 8;
    }
    if(*xPersonagem == 29 && *yPersonagem == 8)
    {
       *xPersonagem = 26;
       *yPersonagem = 8;
    }

}

