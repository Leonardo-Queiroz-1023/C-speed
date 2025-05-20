#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 450
#define PLAYER_SIZE 40
#define OBSTACLE_COUNT 5
#define MAX_HP 3

typedef struct {
    char nome[50];
    int score;
    int hp;
    int x;
} Player;

typedef struct {
    int x, y;
} Obstacle;

typedef struct RankingNode {
    char nome[50];
    int score;
    struct RankingNode* next;
} RankingNode;

Obstacle obstacles[OBSTACLE_COUNT];
RankingNode* rankingHead = NULL;

Texture2D texPlayer, texObstacle;

void InitObstacles() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].x = GetRandomValue(0, WIDTH - PLAYER_SIZE);
        obstacles[i].y = -GetRandomValue(0, HEIGHT);
    }
}

void UpdateObstacles(Player *player) {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].y += 5;

        if (obstacles[i].y >= HEIGHT - PLAYER_SIZE &&
            obstacles[i].x + PLAYER_SIZE > player->x &&
            obstacles[i].x < player->x + PLAYER_SIZE) {
            player->hp--;
            obstacles[i].y = -GetRandomValue(0, HEIGHT);
            obstacles[i].x = GetRandomValue(0, WIDTH - PLAYER_SIZE);
        }

        if (obstacles[i].y > HEIGHT) {
            player->score++;
            obstacles[i].y = -GetRandomValue(0, HEIGHT);
            obstacles[i].x = GetRandomValue(0, WIDTH - PLAYER_SIZE);
        }
    }
}

void InserirRanking(char* nome, int score) {
    RankingNode* novo = malloc(sizeof(RankingNode));
    strcpy(novo->nome, nome);
    novo->score = score;
    novo->next = NULL;

    if (rankingHead == NULL || score > rankingHead->score) {
        novo->next = rankingHead;
        rankingHead = novo;
    } else {
        RankingNode* atual = rankingHead;
        while (atual->next && atual->next->score >= score)
            atual = atual->next;
        novo->next = atual->next;
        atual->next = novo;
    }
}

void SalvarRanking() {
    FILE* f = fopen("ranking.txt", "w");
    if (!f) return;
    RankingNode* node = rankingHead;
    while (node) {
        fprintf(f, "%s %d\n", node->nome, node->score);
        node = node->next;
    }
    fclose(f);
}

void CarregarRanking() {
    FILE* f = fopen("ranking.txt", "r");
    if (!f) return;
    char nome[50];
    int score;
    while (fscanf(f, "%s %d", nome, &score) != EOF)
        InserirRanking(nome, score);
    fclose(f);
}

void MostrarRanking() {
    ClearBackground(RAYWHITE);
    DrawText("=== Ranking ===", 280, 50, 30, DARKGRAY);
    int y = 100, pos = 1;
    RankingNode* node = rankingHead;
    while (node) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%dº - %s: %d", pos++, node->nome, node->score);
        DrawText(buffer, 200, y, 20, BLACK);
        y += 30;
        node = node->next;
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Jogo de Esquiva com Ranking");
    SetTargetFPS(60);
    InitAudioDevice();
    srand(time(NULL));

    texPlayer = LoadTexture("assets/player.png");
    texObstacle = LoadTexture("assets/obstacle.png");

    Player jogador = {0};
    jogador.hp = MAX_HP;
    jogador.score = 0;
    jogador.x = WIDTH / 2 - PLAYER_SIZE / 2;

    CarregarRanking();

    char nome[50];
    printf("Digite seu nickname: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;
    strcpy(jogador.nome, nome);

    InitObstacles();

    int gameOver = 0;

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Lógica
            if (IsKeyDown(KEY_LEFT) && jogador.x > 0)
                jogador.x -= 6;
            if (IsKeyDown(KEY_RIGHT) && jogador.x < WIDTH - PLAYER_SIZE)
                jogador.x += 6;

            UpdateObstacles(&jogador);

            if (jogador.hp <= 0) {
                gameOver = 1;
                InserirRanking(jogador.nome, jogador.score);
                SalvarRanking();
            }
        }

        // Desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameOver) {
            DrawText(TextFormat("Jogador: %s", jogador.nome), 10, 10, 20, BLACK);
            DrawText(TextFormat("Score: %d", jogador.score), 10, 35, 20, DARKGREEN);
            DrawText("HP: ", 10, 60, 20, RED);
            for (int i = 0; i < MAX_HP; i++)
                DrawRectangle(60 + i * 25, 60, 20, 20, (i < jogador.hp) ? RED : LIGHTGRAY);

            // Player
            DrawTexture(texPlayer, jogador.x, HEIGHT - PLAYER_SIZE, WHITE);

            // Obstacles
            for (int i = 0; i < OBSTACLE_COUNT; i++)
                DrawTexture(texObstacle, obstacles[i].x, obstacles[i].y, WHITE);
        } else {
            DrawText("Game Over!", 300, 100, 40, RED);
            DrawText(TextFormat("Score: %d", jogador.score), 330, 160, 25, BLACK);
            MostrarRanking();
        }

        EndDrawing();
    }

    UnloadTexture(texPlayer);
    UnloadTexture(texObstacle);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
