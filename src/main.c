/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

/*
MOLE MANIA GRID: 7x8 (ixj)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/screen.h"
#include "include/keyboard.h"
#include "include/timer.h"

int overgroundLayout[8][17];
int undergroundLayout[8][17];

int overgroundLayout2[8][17];
int undergroundLayout2[8][17];

int overgroundLayout3[8][17];
int undergroundLayout3[8][17];

int overgroundLayout4[8][17];
int undergroundLayout4[8][17];

int offsetX = 2;
int offsetY = 2;

int collisionHappened = 0;
int ballCounter = 0;
int selectedBall = 0;
int ballMoving = 0;
int levelComplete = 0;

int level = 1;

char exitPos;

int currentLayout[8][17];
int currentOverground[8][17];
int currentUnderground[8][17];

typedef struct enemy {
    char sprite;
    char direction;
    int x;
    int y;
    int modifier;
    int cooldown;
    int destroyed;
    int complete;
} Enemy;

typedef struct ball {
    char sprite;
    int initialX;
    int initialY;
    int x;
    int y;
    int onSight;
    int holding;
    int counter;
    char holdAxis;
    int activated;
    char direction;
    int destroyed;
    struct ball *next;
} Ball;

typedef struct {
    char sprite;
    int x;
    int y;
    int lives;
    int underground;
    int cantDig;
    int cantDigTimer;
    Ball *targetBall;
} Player;

typedef struct obstacle {
    char sprite;
    int x;
    int y;
    struct obstacle *next;
} Obstacle;

typedef struct hole {
    char sprite;
    int x;
    int y;
    struct hole *next;
} Hole;

typedef struct {
    char sprite;
    int x;
    int y;
    int destroyed;
    int destroyCounter;
    int complete;
} Barrier;


typedef struct {
    int size;
    Ball *head;
    Ball *tail;
} BallList;

typedef struct {
    int size;
    Obstacle *head;
} ObstacleList;

typedef struct {
    int size;
    Hole *head;
    Hole *tail;
} HoleList;


BallList *ballList;
HoleList *holeList;

Player *player;
Barrier *barrier;


BallList *createBallList() {
    BallList *ballList = (BallList *) malloc(sizeof(BallList));
    if (ballList == NULL) return NULL;

    ballList->size = 0;
    ballList->head = NULL;
    ballList->tail = NULL;

    return ballList;
}

void addBalltoList(int x, int y) {
    Ball *ball = (Ball *) malloc(sizeof(Ball));
    if (ball == NULL) return;

    ball->sprite = 'O';
    ball->initialX = x;
    ball->initialY = y;
    ball->x = x;
    ball->y = y;
    ball->onSight = 0;
    ball->holding = 0;
        ball->counter = 0;
    ball->holdAxis = ' ';
    ball->activated = 0;
    ball->direction = ' ';
    ball->destroyed = 0;
    ball->next = NULL;

    if (ballList->head == NULL) ballList->head = ball;
    if (ballList->tail != NULL) ballList->tail->next = ball;

    ballList->tail = ball;

    ballList->size++;
}

void freeBalls(Ball *ball) {
    if (ball == NULL) return;

    freeBalls(ball->next);
    free(ball);
    ball = NULL;
}

void destroyBalls(Ball *ball) {
    if (ball == NULL) return;

    destroyBalls(ball->next);

    ball->sprite = '%';
}

// ObstacleList *createObstacleList() {
//     ObstacleList *obstacleList = (ObstacleList *) malloc(sizeof(ObstacleList));
//     if (obstacleList == NULL) return NULL;

//     Obstacle *header = (Obstacle *) malloc(sizeof(Obstacle));
//     if (header == NULL) return NULL;

//     obstacleList->size = 0;
//     obstacleList->head = header;
//     obstacleList->tail = header;

//     return obstacleList;
// }

HoleList *createHoleList() {
    HoleList *holeList = (HoleList *) malloc(sizeof(HoleList));
    if (holeList == NULL) return NULL;

    holeList->size = 0;
    holeList->head = NULL;
    holeList->tail = NULL;

    return holeList;
}

void addHoletoList(int x, int y) {
    Hole *hole = (Hole *) malloc(sizeof(Hole));
    if (hole == NULL) return;

    hole->sprite = '@';
    hole->x = x;
    hole->y = y;
    hole->next = NULL;

    if (holeList->head == NULL) holeList->head = hole;
    if (holeList->tail != NULL) holeList->tail->next = hole;

    holeList->tail = hole;

    holeList->size++;
}

void freeHoles(Hole *hole) {
    if (hole == NULL) return;

    freeHoles(hole->next);
    free(hole);
    hole = NULL;
}

void getLayouts() {
    FILE *arq = fopen("layout.txt", "r");
    if (arq == NULL) return;

    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            overgroundLayout[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }
    fgetc(arq);
    fgetc(arq);
    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            undergroundLayout[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }

    for (int i=0; i<6; i++) fgetc(arq);

    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            overgroundLayout2[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }
    fgetc(arq);
    fgetc(arq);
    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            undergroundLayout2[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }

    for (int i=0; i<6; i++) fgetc(arq);

    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            overgroundLayout3[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }
    fgetc(arq);
    fgetc(arq);
    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            undergroundLayout3[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }

    for (int i=0; i<6; i++) fgetc(arq);

    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            overgroundLayout4[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }
    fgetc(arq);
    fgetc(arq);
    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            undergroundLayout4[i][j] = fgetc(arq);
        }
        fgetc(arq);
        fgetc(arq);
    }

    fclose(arq);
}


void finishLevel() {
    freeHoles(holeList->head);
    holeList->head = NULL;
    holeList->tail = NULL;
    holeList->size = 0;

    freeBalls(ballList->head);
    ballList->head = NULL;
    ballList->tail = NULL;
    ballList->size = 0;

    levelComplete = 0;
}

void loadLevel() {
    // freeHoles(holeList->head);
    // freeBalls(ballList->head);
    // holeList->size = 0;
    // ballList->size = 0;

    if (level == 1) {
        exitPos = '>';
        for (int i=0; i<8; i++) {
            for (int j=0; j<17; j++) {
                currentOverground[i][j] = overgroundLayout[i][j];
                currentUnderground[i][j] = undergroundLayout[i][j];
            }
        }
    } else if (level == 2) {
        exitPos = '>';
        for (int i=0; i<8; i++) {
            for (int j=0; j<17; j++) {
                currentOverground[i][j] = overgroundLayout2[i][j];
                currentUnderground[i][j] = undergroundLayout2[i][j];
            }
        }
    } else if (level == 3) {
        exitPos = '^';
        for (int i=0; i<8; i++) {
            for (int j=0; j<17; j++) {
                currentOverground[i][j] = overgroundLayout3[i][j];
                currentUnderground[i][j] = undergroundLayout3[i][j];
            }
        }
    } else if (level == 4) {
        exitPos = '^';
        for (int i=0; i<8; i++) {
            for (int j=0; j<17; j++) {
                currentOverground[i][j] = overgroundLayout4[i][j];
                currentUnderground[i][j] = undergroundLayout4[i][j];
            }
        }
        barrier->destroyed = 0;
        barrier->destroyCounter = 0;
        barrier->complete = 0;
    }

    for (int i=0; i<8; i++) {
        for (int j=0; j<17; j++) {
            if (currentOverground[i][j] == 'o') {
                addBalltoList(j + offsetX, i + offsetY);
            }
            else if (currentOverground[i][j] == 'E') {
                barrier->sprite = 'E';
                barrier->x = j + offsetX;
                barrier->y = i + offsetY;
                barrier->destroyed = 0;
                barrier->destroyCounter = 0;
                barrier->complete = 0;
                currentOverground[i][j] = ':';
            }
            else if (currentOverground[i][j] == '<' || currentOverground[i][j] == '^' || currentOverground[i][j] == '>' || currentOverground[i][j] == 'v') {
                player->sprite = currentOverground[i][j];
                player->x = j + offsetX;
                player->y = i + offsetY;
                player->underground = 0;
                player->cantDig = 0;
                player->cantDigTimer = 0;
                player->targetBall = NULL;
                currentOverground[i][j] = ':';
            }
            currentLayout[i][j] = currentOverground[i][j];
        }
    }
}



void ballVsHoles(Ball *ball, Hole *hole) {
    if (hole == NULL) return;

    if (ball->x == hole->x && ball->y == hole->y) {
        ball->activated = 0;
        ball->x = ball->initialX;
        ball->y = ball->initialY;
        ballMoving = 0;
    } else ballVsHoles(ball, hole->next);
}

void ballVsBalls(Ball *ball, Ball *ball2, int nextX, int nextY) {
    if (ball2 == NULL) return;

    if (ball != ball2 && (nextX == ball2->x && nextY == ball2->y)) {
        collisionHappened = 1;
    } else ballVsBalls(ball, ball2->next, nextX, nextY);
}

int checkLayoutObstaclesPlayer(int x, int y) {
    if (currentLayout[y][x] == '#' ||
        currentLayout[y][x] == '~')
        return 1;
    return 0;
}

int checkLayoutObstaclesBall(int x, int y) {
    if (currentOverground[y][x] == '#') return 1;
    return 0;
}

void enemyVsWalls(Enemy *enemy) {
    if (enemy->direction == '<') {
        if (enemy->x == offsetX) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(enemy->x - offsetX - 1, enemy->y - offsetY);
        }
    }
    else if (enemy->direction == '^') {
        if (enemy->y == offsetY) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(enemy->x - offsetX, enemy->y - offsetY - 1);
        }
    }
    else if (enemy->direction == '>') {
        if (enemy->x == 16 + offsetX) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(enemy->x - offsetX + 1, enemy->y - offsetY);
        }
    }
    else if (enemy->direction == 'v') {
        if (enemy->y == 7 + offsetY) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(enemy->x - offsetX, enemy->y - offsetY + 1);
        }
    }
}

void playerVsWalls(Player *player) {
    if (player->sprite == '<') {
        if (player->x == offsetX) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesPlayer(player->x - offsetX - 1, player->y - offsetY);
        }
    }
    else if (player->sprite == '^') {
        if (player->y == offsetY) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesPlayer(player->x - offsetX, player->y - offsetY - 1);
        }
    }
    else if (player->sprite == '>') {
        if (player->x >= 16 + offsetX) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesPlayer(player->x - offsetX + 1, player->y - offsetY);
        }
    }
    else if (player->sprite == 'v') {
        if (player->y >= 7 + offsetY) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesPlayer(player->x - offsetX, player->y - offsetY + 1);
        }
    }
}

void ballVsWalls(Ball *ball) {
    if (ball->direction == '<') {
        if (ball->x == offsetX) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(ball->x - offsetX - 1, ball->y - offsetY);
        }
    }
    else if (ball->direction == '^') {
        if (ball->y == offsetY) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(ball->x - offsetX, ball->y - offsetY - 1);
        }
    }
    else if (ball->direction == '>') {
        if (ball->x == 16 + offsetX) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(ball->x - offsetX + 1, ball->y - offsetY);
        }
    }
    else if (ball->direction == 'v') {
        if (ball->y == 7 + offsetY) collisionHappened = 1;
        else {
            collisionHappened = checkLayoutObstaclesBall(ball->x - offsetX, ball->y - offsetY + 1);
        }
    }
}

void playerVsBall(Player *player, Ball *ball) {
    if (ball == NULL || barrier->complete) return;

    if (player->sprite == '<') {
        if (player->y == ball->y && player->x - 1 == ball->x) {
            collisionHappened = 1;
            player->targetBall = ball;
            ball->onSight = 1;
            return;
        }
    }
    else if (player->sprite == '^') {
        if (player->x == ball->x && player->y - 1 == ball->y) {
            collisionHappened = 1;
            player->targetBall = ball;
            ball->onSight = 1;
            return;
        }
    }
    else if (player->sprite == '>') {
        if (player->y == ball->y && player->x + 1 == ball->x) {
            collisionHappened = 1;
            player->targetBall = ball;
            ball->onSight = 1;
            return;
        }
    }
    else if (player->sprite == 'v') {
        if (player->x == ball->x && player->y + 1 == ball->y) {
            collisionHappened = 1;
            player->targetBall = ball;
            ball->onSight = 1;
            return;
        }
    }
    playerVsBall(player, ball->next);
}

void playerVsBarrier(Player *player, Barrier *barrier) {
    if (barrier == NULL) return;
    if (barrier->complete) {
        if (player->x == barrier->x && player->y == barrier->y && player->sprite == exitPos) {
            collisionHappened = 1;
            finishLevel();
            level++;
            loadLevel();
        }
        return;
    }

    if (player->sprite == '<') {
        if (player->y == barrier->y && player->x - 1 == barrier->x) {
            collisionHappened = 1;
            return;
        }
    }
    else if (player->sprite == '^') {
        if (player->x == barrier->x && player->y - 1 == barrier->y) {
            collisionHappened = 1;
            return;
        }
    }
    else if (player->sprite == '>') {
        if (player->y == barrier->y && player->x + 1 == barrier->x) {
            collisionHappened = 1;
            return;
        }
    }
    else if (player->sprite == 'v') {
        if (player->x == barrier->x && player->y + 1 == barrier->y) {
            collisionHappened = 1;
            return;
        }
    }
}

void ballCollisions(Ball *ball, Barrier *barrier) {
    if (ball == NULL) return;

    if (ball->holding == 1) {
        ball->counter++;
        if (ball->counter == 2) {
            if (ball->sprite == 'O') ball->sprite = 'o';
            else if (ball->sprite == 'o') ball->sprite = 'O';
            ball->counter = 0;
        }
    } else if (!barrier->destroyed) {
        ball->counter = 0;
        ball->sprite = 'O';
    }

    if (ball->activated)
        ballVsHoles(ball, holeList->head);

    if (ball->activated) {
        if (!barrier->complete && (ball->x == barrier->x && ball->y == barrier->y)) {
            levelComplete = 1;
            ball->activated = 0;
        }
        else {
            int nextX = ball->x, nextY = ball->y;
            if (ball->direction == '<') nextX -= 1;
            else if (ball->direction == '^') nextY -= 1;
            else if (ball->direction == '>') nextX += 1;
            else if (ball->direction == 'v') nextY += 1;
            ballVsWalls(ball);
            ballVsBalls(ball, ballList->head, nextX, nextY);
            if (!collisionHappened) {
                if (ball->direction == '<') ball->x -= 1;
                else if (ball->direction == '^') ball->y -= 1;
                else if (ball->direction == '>') ball->x += 1;
                else if (ball->direction == 'v') ball->y += 1;
            }
            else {
                ball->activated = 0;
                ballMoving = 0;
            }
        }
    }
    
    ballCollisions(ball->next, barrier);
}

// void printHello(int nextX, int nextY)
// {
//     screenSetColor(CYAN, DARKGRAY);
//     screenGotoxy(x, y);
//     printf("           ");
//     x = nextX;
//     y = nextY;
//     screenGotoxy(x, y);
//     printf("Hello World");
// }

void backThrowBall(Player *player, Ball *ball) {
    if (ball->direction == '<') {
        if (currentOverground[player->y - offsetY][player->x - 1 - offsetX] != '#') {
            ball->x = player->x - 1;
            ballMoving = 1;
        }
        else ball->activated = 0;
    }
    else if (ball->direction == '^') {
        if (currentOverground[player->y - 1 - offsetY][player->x - offsetX] != '#') {
            ball->y = player->y - 1;
            ballMoving = 1;
        }
        else ball->activated = 0;
    }
    else if (ball->direction == '>') {
        if (currentOverground[player->y - offsetY][player->x + 1 - offsetX] != '#') {
            ball->x = player->x + 1;
            ballMoving = 1;
        }
        else ball->activated = 0;
    }
    else if (ball->direction == 'v') {
        if (currentOverground[player->y + 1 - offsetY][player->x - offsetX] != '#') {
            ball->y = player->y + 1;
            ballMoving = 1;
        }
        else ball->activated = 0;
    }
}

void throwBall(Player *player, Ball *ball) {
    if (ball->holdAxis == 'y' && (player->sprite == '^' || player->sprite == 'v')) {
        ball->activated = 1;
        ball->direction = player->sprite;

        if ((player->sprite == '^' && ball->y == player->y + 1) || (player->sprite == 'v' && ball->y == player->y - 1)) {
            backThrowBall(player, player->targetBall);
        }
        collisionHappened = 1;
    }
    else if (ball->holdAxis == 'x' && (player->sprite == '<' || player->sprite == '>')) {
        ball->activated = 1;
        ball->direction = player->sprite;

        if ((player->sprite == '<' && ball->x == player->x + 1) || (player->sprite == '>' && ball->x == player->x - 1)) {
            backThrowBall(player, player->targetBall);
        }
        collisionHappened = 1;
    }
    ball->holding = 0;
    selectedBall = 0;
}

void printTitle() {
    screenSetColor(LIGHTGREEN, DARKGRAY);

    screenGotoxy(3, 0);
    printf("RATAZANA");
    screenGotoxy(13, 0);
    printf("MANIA");
}

void printInfo(Player *player) {
    screenSetColor(LIGHTRED, DARKGRAY);

    screenGotoxy(offsetX, 8+offsetY);
    printf("                ");

    screenSetColor(LIGHTGREEN, DARKGRAY);

    screenGotoxy(offsetX, 9+offsetY);
    if (level == 1 && !levelComplete) {
        if (player->underground) printf("[Espaco] -> subir");
        else if (player->y >= 5 + offsetY && player->x < 5 + offsetX) printf("[WASD] -> mover  ");
        else if (player->x < 5 + offsetX) printf("[Espaco] -> cavar");
        else if (player->targetBall && player->targetBall->onSight && !player->targetBall->holding && !ballMoving) printf("[Z] -> segurar   ");
        else if (selectedBall) printf("[WASD] -> jogar  ");
        else {
            screenSetColor(LIGHTGRAY, DARKGRAY);
            printf("/ / / / / / / / /");
        }
    }
    else if (level == 2) {
        printf("[R] -> reload    ");
    }
    else if (level == 4) {
        printf("[Enter] -> sair  ");
    }
    else {
        screenSetColor(LIGHTGRAY, DARKGRAY);
        printf("/ / / / / / / / /");
    }
    
}

void printLayout() {
    if (!player->underground) screenSetColor(LIGHTGRAY, DARKGRAY);
    else screenSetColor(DARKGRAY, DARKGRAY);

    for (int i=0; i<8; i++) {
        for(int j=0; j<17; j++) {
            screenGotoxy(j+offsetX, i+offsetY);
            if (currentLayout[i][j] != '@')
                printf("%c", currentLayout[i][j]);
            else printf(" ");
        }
    }

    // screenGotoxy(exitLevelX + offsetX, exitLevelY + offsetY);
    // printf("g");
}

void printBalls(Ball *ball, Barrier *barrier) {
    if (ball == NULL || barrier->complete) return;
    if (player->underground) return;

    screenSetColor(LIGHTGREEN, DARKGRAY);

    if (!(ball->x == barrier->x && ball->y == barrier->y)) {
        screenGotoxy(ball->x, ball->y);
        printf("%c", ball->sprite);
    }

    printBalls(ball->next, barrier);
}

void printHoles(Hole *hole) {
    if (hole == NULL) return;

    if (!player->underground) screenSetColor(DARKGRAY, DARKGRAY);
    else screenSetColor(LIGHTGRAY, DARKGRAY);

    screenGotoxy(hole->x, hole->y);
    printf("%c", hole->sprite);

    printHoles(hole->next);
}

void printPlayer(Player *player) {
    screenSetColor(BROWN, DARKGRAY);

    screenGotoxy(player->x, player->y);
    if (!player->cantDig) printf("%c", player->sprite);
    else printf("#");
}

void printBarrier(Player *player, Barrier *barrier)
{
    if(barrier == NULL) return;
    if (!player->underground) {
        if (barrier != NULL && !barrier->complete) {
            screenSetColor(LIGHTGRAY, DARKGRAY);

            screenGotoxy(barrier->x, barrier->y);
            printf("%c", barrier->sprite);
        }
    }
}


int main() 
{
    static int ch = 0;
    static long timer = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    getLayouts();

    ballList = createBallList();
    holeList = createHoleList();

    player = (Player *) malloc(sizeof(Player));
    barrier = (Barrier *) malloc(sizeof(Barrier));

    loadLevel();

    //printHello(x, y);
    printTitle();
    printInfo(player);
    screenUpdate();

    while (ch != 10) //enter or 5s
    {
        // Handle user input
        if (keyhit() && !player->cantDig) 
        {
            if (player->targetBall != NULL) player->targetBall->onSight = 0;
            collisionHappened = 0;
            ch = readch();

            if(ch == 'w' || ch == 'W') {
                playerVsBall(player, ballList->head);
                collisionHappened = 0;
                player->sprite = '^';
                if (player->targetBall && player->targetBall->holding) {
                    throwBall(player, player->targetBall);
                }
                if(!collisionHappened) {
                    playerVsWalls(player);
                    if (!player->underground) {
                        if (!collisionHappened) playerVsBall(player, ballList->head);
                        playerVsBarrier(player, barrier);
                    }
                    if (!collisionHappened) {
                        player->y -= 1;
                    }
                }
            }
            else if(ch == 'a' || ch == 'A') {
                playerVsBall(player, ballList->head);
                collisionHappened = 0;
                player->sprite = '<';
                if (player->targetBall && player->targetBall->holding) {
                    throwBall(player, player->targetBall);
                }
                if(!collisionHappened) {
                    playerVsWalls(player);
                    if (!player->underground) {
                        if (!collisionHappened) playerVsBall(player, ballList->head);
                        playerVsBarrier(player, barrier);
                    }
                    if (!collisionHappened) {
                        player->x -= 1;
                    }
                }
            }
            else if(ch == 's' || ch == 'S') {
                playerVsBall(player, ballList->head);
                collisionHappened = 0;
                player->sprite = 'v';
                if (player->targetBall && player->targetBall->holding) {
                    throwBall(player, player->targetBall);
                }
                if(!collisionHappened) {
                    playerVsWalls(player);
                    if (!player->underground) {
                        if (!collisionHappened) playerVsBall(player, ballList->head);
                        playerVsBarrier(player, barrier);
                    }
                    if (!collisionHappened) {
                        player->y += 1;
                    }
                }
            }
            else if(ch == 'd' || ch == 'D') {
                playerVsBall(player, ballList->head);
                collisionHappened = 0;
                player->sprite = '>';
                if (player->targetBall && player->targetBall->holding) {
                    throwBall(player, player->targetBall);
                }
                if(!collisionHappened) {
                    playerVsWalls(player);
                    if (!player->underground) {
                        if (!collisionHappened) playerVsBall(player, ballList->head);
                        playerVsBarrier(player, barrier);
                    }
                    if (!collisionHappened) {
                        player->x += 1;
                    }
                }
            }

            else if (ch == 'z' || ch == 'Z') {
                playerVsBall(player, ballList->head);
                if (player->targetBall && collisionHappened) {
                    player->targetBall->holding = !player->targetBall->holding;
                    if (player->targetBall->holding) {
                        selectedBall = 1;
                        if (player->sprite == '<' || player->sprite == '>') player->targetBall->holdAxis = 'x';
                        else player->targetBall->holdAxis = 'y';
                    } 
                    player->targetBall->direction = player->sprite;
                }
            }

            else if (ch == 32) {
                if (!((currentOverground[player->y - offsetY][player->x - offsetX]) == 'o') &&
                    !((currentOverground[player->y - offsetY][player->x - offsetX]) == ':') &&
                    !((currentOverground[player->y - offsetY][player->x - offsetX]) == '~') &&
                    !((currentOverground[player->y - offsetY][player->x - offsetX]) == '#'))
                {
                    if (currentLayout[player->y - offsetY][player->x - offsetX] != '@')
                        addHoletoList(player->x, player->y);
                    currentOverground[player->y - offsetY][player->x - offsetX] = '@';
                    currentUnderground[player->y - offsetY][player->x - offsetX] = '@';
                    if (player->underground == 0) {
                        for (int i=0; i<8; i++) {
                            for (int j=0; j<17; j++) {
                                currentLayout[i][j] = currentUnderground[i][j];
                            }
                        }
                        player->underground = 1;
                    } else {
                        for (int i=0; i<8; i++) {
                            for (int j=0; j<17; j++) {
                                currentLayout[i][j] = currentOverground[i][j];
                            }
                        }
                        player->underground = 0;
                    }
                }
                else {
                    player->cantDig = 1;
                }
            }

            else if(ch == 'r' || ch == 'R') {
                finishLevel();
                loadLevel();
            }

            if (!selectedBall) {
                if (player->targetBall != NULL) player->targetBall->onSight = 0;
                playerVsBall(player, ballList->head);
            }

            printInfo(player);

            // printLayout(currentLayout);
            // if (holeList->size != 0) printHoles(holeList->head, currentLayout);
            // printPlayer(player);
            // printBarrier(player, barrier);
            // printBalls(ballList->head, currentLayout, barrier);
            // printInfo(currentLayout, player);
            // screenUpdate();
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            if (player->cantDig) {
                player->cantDigTimer++;
                if (player->cantDigTimer == 5) {
                    player->cantDigTimer = 0;
                    player->cantDig = 0;
                }
            }

            ballCollisions(ballList->head, barrier);

            if (levelComplete && barrier != NULL) {
                if (!barrier->destroyed) {
                    barrier->destroyed = 1;
                    barrier->sprite = '%';
                    destroyBalls(ballList->head);
                }
                barrier->destroyCounter++;
                if (barrier->destroyCounter == 6) {
                    ballList->size = 0;
                    barrier->complete = 1;
                    // free(barrier);
                    // barrier = NULL;
                }
            }

            if (!selectedBall) {
                if (player->targetBall != NULL) player->targetBall->onSight = 0;
                playerVsBall(player, ballList->head);
            }

            printInfo(player);
            printLayout();
            printHoles(holeList->head);
            printBarrier(player, barrier);
            printBalls(ballList->head, barrier);
            printPlayer(player);

            screenUpdate();
            timer++;
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    free(player);
    free(barrier);

    if (holeList->size != 0) freeHoles(holeList->head);
    if (ballList->size != 0) freeBalls(ballList->head);
    free(holeList);
    free(ballList);
    holeList = NULL;

    return 0;
}
