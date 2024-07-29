#include "movimento.h"

int podeMover(int x, int y, int dx, int dy) {
    if ((x + dx < 0) || (x + dx >= MAP_WIDTH) ||
        (y + dy < 0) || (y + dy >= MAP_HEIGHT) ||
        map[y + dy][x + dx] == 'W') {
        return 0; // Não pode mover
    }
    return 1; // Pode mover
}

void move(int *x, int *y, int dx, int dy) {
    *x += dx;
    *y += dy;
}

