#ifndef MAIN_H
#define MAIN_H

#define MAX_INIMIGOS 25
#define MAX_ARQUEIROS 5
#define MAX_BARRICADAS 5
#define MAX_FLECHAS 100
#define MAX_MINAS 5

int podeMover(int x, int y, int dx, int dy);
void move(int *x, int *y, int dx, int dy);
int inimigomove(int *x,int *y);
int main(void);
int run();

#endif
