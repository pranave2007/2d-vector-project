#include <stdio.h>
#include <stdlib.h>

#define WIDTH 20
#define HEIGHT 20
#define MAX_SHAPES 50

typedef enum { RECTANGLE, LINE, TRIANGLE, CIRCLE } ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int is_active;
} Shape;

char canvas[HEIGHT][WIDTH];
Shape shape_list[MAX_SHAPES];
int shape_count = 0;

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) canvas[i][j] = '_';
}

// --- NEW DATA ADDED FOR PUSH 2 ---

void drawLineOnGrid(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) canvas[y1][x1] = '*';
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void renderCanvas() {
    clearCanvas();
    for (int i = 0; i < shape_count; i++) {
        if (!shape_list[i].is_active) continue;
        
        if (shape_list[i].type == LINE) {
            drawLineOnGrid(shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2);
        } 
        else if (shape_list[i].type == TRIANGLE) {
            // A triangle is just 3 Bresenham lines connected end-to-end
            drawLineOnGrid(shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2);
            drawLineOnGrid(shape_list[i].x2, shape_list[i].y2, shape_list[i].x3, shape_list[i].y3);
            drawLineOnGrid(shape_list[i].x3, shape_list[i].y3, shape_list[i].x1, shape_list[i].y1);
        }
    }
}

void displayCanvas() {
    printf("\n--- CURRENT CANVAS ---\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) printf("%c ", canvas[i][j]);
        printf("\n");
    }
}

int main() {
    // Basic structural placeholder for validation
    renderCanvas();
    displayCanvas();
    return 0;
}

