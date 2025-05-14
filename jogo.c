#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <conio.h>  // getch()

#define WIDTH 20 
#define HEIGHT 10 
#define MAX_HP 3 
#define OBSTACLE_COUNT 5

typedef struct {
    char nome[50];
    int score;
    int hp;
} player;

typedef struct {
    int x, y;
} Obstacle;

Obstacle obstacles[OBSTACLE_COUNT];
int playerX = WIDTH / 2;
int hp = MAX_HP;
int score = 0;

void initObstacles() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].x = rand() % WIDTH;
        obstacles[i].y = -(rand() % HEIGHT);
    }
}

void draw(player jogador) {
    system("cls");
    
    printf("Jogador: %s\tScore: %d\tHP: ", jogador.nome, jogador.score);
    for (int i = 0; i < MAX_HP; i++) {
        if (i < jogador.hp) printf("[ HP ]");
        else printf("[  ]");
    }
    printf("\n");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int drawn = 0;

            // Obstáculo
            for (int i = 0; i < OBSTACLE_COUNT; i++) {
                if (obstacles[i].x == x && obstacles[i].y == y) {
                    printf("X");
                    drawn = 1;
                    break;
                }
            }

            // Jogador
            if (!drawn && y == HEIGHT - 1 && x == playerX) {
                printf("A");  // jogador
                drawn = 1;
            }

            if (!drawn) printf(" ");
        }
        printf("\n");
    }
}

void updateObstacles(player* jogador) {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].y++;

        // Colisão com jogador
        if (obstacles[i].y == HEIGHT - 1 && obstacles[i].x == playerX) {
            jogador->hp--;
            obstacles[i].y = 0;
            obstacles[i].x = rand() % WIDTH;
        }

        // Saíram da tela
        if (obstacles[i].y >= HEIGHT) {
            obstacles[i].y = 0;
            obstacles[i].x = rand() % WIDTH;
            jogador->score++;
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
    system("chcp 65001 > nul");

    player jogador;

    printf("Escolha seu Nickname: ");
    fgets(jogador.nome, sizeof(jogador.nome), stdin);
    jogador.nome[strcspn(jogador.nome, "\n")] = '\0';

    jogador.hp = MAX_HP;
    jogador.score = 0;

    srand(time(NULL));
    initObstacles();

    printf("________Sua aventura já vai começar!________\n");
    system("pause");

    int running = 1;
    while (running) {
        draw(jogador);

        if (_kbhit()) {
            char key = _getch();
            if (key == 75 && playerX > 0) playerX--;         // ← esquerda
            if (key == 77 && playerX < WIDTH - 1) playerX++; // → direita
        }

        updateObstacles(&jogador);

        if (jogador.hp <= 0) {
            running = 0;
        }

        _sleep(30);  // pausa entre frames
    }

    system("cls");
    printf("\n💀 ___________Fim de jogo, %s !___________,\n___________Score final: %d___________\n", jogador.nome, jogador.score);

    return 0;
}
