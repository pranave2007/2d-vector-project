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

void plotCirclePoints(int xc, int yc, int x, int y) {
    int px[8] = {xc+x, xc-x, xc+x, xc-x, xc+y, xc-y, xc+y, xc-y};
    int py[8] = {yc+y, yc+y, yc-y, yc-y, yc+x, yc+x, yc-x, yc-x};
    for (int i = 0; i < 8; i++) {
        if (px[i] >= 0 && px[i] < WIDTH && py[i] >= 0 && py[i] < HEIGHT) canvas[py[i]][px[i]] = '*';
    }
}

void drawCircle(Shape s) {
    int xc = s.x1, yc = s.y1, r = s.x2;
    int x = 0, y = r;
    int d = 3 - 2 * r;
    plotCirclePoints(xc, yc, x, y);
    while (y >= x) {
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; }
        else d = d + 4 * x + 6;
        plotCirclePoints(xc, yc, x, y);
    }
}

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

void drawRectangle(Shape s) {
    for (int y = s.y1; y <= s.y2; y++) {
        for (int x = s.x1; x <= s.x2; x++) {
            if (y == s.y1 || y == s.y2 || x == s.x1 || x == s.x2) {
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) canvas[y][x] = '*';
            }
        }
    }
}

void renderCanvas() {
    clearCanvas();
    for (int i = 0; i < shape_count; i++) {
        if (!shape_list[i].is_active) continue;
        if (shape_list[i].type == RECTANGLE) drawRectangle(shape_list[i]);
        else if (shape_list[i].type == LINE) drawLineOnGrid(shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2);
        else if (shape_list[i].type == TRIANGLE) {
            drawLineOnGrid(shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2);
            drawLineOnGrid(shape_list[i].x2, shape_list[i].y2, shape_list[i].x3, shape_list[i].y3);
            drawLineOnGrid(shape_list[i].x3, shape_list[i].y3, shape_list[i].x1, shape_list[i].y1);
        } else if (shape_list[i].type == CIRCLE) {
            drawCircle(shape_list[i]);
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

void listShapes() {
    printf("\n--- ACTIVE SHAPES DIRECTORY ---\n");
    int found = 0;
    for (int i = 0; i < shape_count; i++) {
        if (shape_list[i].is_active) {
            found = 1;
            printf("ID [%d]: ", shape_list[i].id);
            if (shape_list[i].type == RECTANGLE) printf("Rectangle (TL: %d,%d, BR: %d,%d)\n", shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2);
            else if (shape_list[i].type == LINE) printf("Line (Start: %d,%d, End: %d,%d)\n", shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2);
            else if (shape_list[i].type == TRIANGLE) printf("Triangle (P1: %d,%d, P2: %d,%d, P3: %d,%d)\n", shape_list[i].x1, shape_list[i].y1, shape_list[i].x2, shape_list[i].y2, shape_list[i].x3, shape_list[i].y3);
            else if (shape_list[i].type == CIRCLE) printf("Circle (Center: %d,%d, Radius: %d)\n", shape_list[i].x1, shape_list[i].y1, shape_list[i].x2);
        }
    }
    if (!found) printf("No active shapes on canvas.\n");
    printf("-------------------------------\n");
}

void inputShapeData(Shape *s, int type) {
    s->type = (ShapeType)type;
    if (type == 0) {
        printf("Enter Top-Left X Y & Bottom-Right X Y: ");
        scanf("%d %d %d %d", &s->x1, &s->y1, &s->x2, &s->y2);
    } else if (type == 1) {
        printf("Enter Start X Y & End X Y: ");
        scanf("%d %d %d %d", &s->x1, &s->y1, &s->x2, &s->y2);
    } else if (type == 2) {
        printf("Enter P1 X Y, P2 X Y, P3 X Y: ");
        scanf("%d %d %d %d %d %d", &s->x1, &s->y1, &s->x2, &s->y2, &s->x3, &s->y3);
    } else if (type == 3) {
        printf("Enter Center X Y & Radius: ");
        scanf("%d %d %d", &s->x1, &s->y1, &s->x2);
    }
}

int main() {
    int choice = 0;
    while (1) {
        renderCanvas();
        displayCanvas();
        printf("\n=== 2D VECTOR EDITOR INTERFACE ===\n");
        printf("1. Add Shape\n2. Delete Shape\n3. Modify Shape\n4. List Vector Directory\n0. Exit\n");
        printf("Select command: ");
        scanf("%d", &choice);

        if (choice == 0) break;
        if (choice == 1) {
            if (shape_count >= MAX_SHAPES) { printf("Memory limits reached!\n"); continue; }
            int typeChoice;
            printf("Select Type (0: Rectangle, 1: Line, 2: Triangle, 3: Circle): ");
            scanf("%d", &typeChoice);
            if(typeChoice < 0 || typeChoice > 3) { printf("Invalid type!\n"); continue; }
            
            Shape s;
            s.id = shape_count + 1;
            s.is_active = 1;
            inputShapeData(&s, typeChoice);
            shape_list[shape_count++] = s;
        } 
        else if (choice == 2) {
            listShapes();
            int targetId;
            printf("Enter target ID to drop from execution pipeline: ");
            scanf("%d", &targetId);
            for(int i = 0; i < shape_count; i++) {
                if(shape_list[i].id == targetId) { shape_list[i].is_active = 0; break; }
            }
        } 
        else if (choice == 3) {
            listShapes();
            int targetId;
            printf("Enter target ID to rewrite properties: ");
            scanf("%d", &targetId);
            for(int i = 0; i < shape_count; i++) {
                if(shape_list[i].id == targetId && shape_list[i].is_active) {
                    inputShapeData(&shape_list[i], shape_list[i].type);
                    break;
                }
            }
        } 
        else if (choice == 4) {
            listShapes();
        }
    }
    return 0;
}