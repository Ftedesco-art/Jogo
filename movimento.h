#ifndef MOVIMENTO_H
#define MOVIMENTO_H

int podeMover(int x, int y, int dx, int dy);
void move(int *x, int *y, int dx, int dy);
int inimigomove(int *x,int *y);
int main(void);

extern xPersonagem;
extern yPersonagem;
extern spawnx;
extern spawny;


#endif
