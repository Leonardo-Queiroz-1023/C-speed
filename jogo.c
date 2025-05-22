#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 450
#define PLAYER_SIZE 25
#define OBSTACLE_COUNT 8
#define MAX_HP 3
#define OBSTACLE_SIZE 30


typedef struct {
    char nome[50];
    int score;
    int hp;
    int x;
    double tempo; // <-- novo campo
} Player;

typedef struct {
    int x, y;
} Obstacle;

typedef struct RankingNode {
    char nome[50];
    int score;
    double tempo; // <-- novo campo
    struct RankingNode* next;
} RankingNode;

Obstacle obstacles[OBSTACLE_COUNT];
RankingNode* rankingHead = NULL;

Texture2D texPlayer, texObstacle, texArvore;

void InitObstacles() {
    int pistaLargura = WIDTH / 2;
    int pistaInicioX = (WIDTH - pistaLargura) / 2;
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        int sobrepoe;
        do {
            sobrepoe = 0;
            obstacles[i].x = GetRandomValue(pistaInicioX, pistaInicioX + pistaLargura - OBSTACLE_SIZE);
            obstacles[i].y = GetRandomValue(0, HEIGHT / 2);
            // Verifica se sobrepõe algum obstáculo já posicionado
            for (int j = 0; j < i; j++) {
                if (abs(obstacles[i].x - obstacles[j].x) < OBSTACLE_SIZE &&
                    abs(obstacles[i].y - obstacles[j].y) < OBSTACLE_SIZE) {
                    sobrepoe = 1;
                    break;
                }
            }
        } while (sobrepoe);
    }
}

void UpdateObstacles(Player *player) {
    int pistaLargura = WIDTH / 2;
    int pistaInicioX = (WIDTH - pistaLargura) / 2;
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].y += 3; // velocidade de descida

        // Detecção de colisão
        int playerY = HEIGHT - PLAYER_SIZE;
        if (obstacles[i].x < player->x + PLAYER_SIZE &&
            obstacles[i].x + OBSTACLE_SIZE > player->x &&
            obstacles[i].y < playerY + PLAYER_SIZE &&
            obstacles[i].y + OBSTACLE_SIZE > playerY) {
            // Colidiu!
            player->hp--;
            // Reposiciona o obstáculo no topo em posição aleatória dentro da pista
            obstacles[i].y = 0;
            obstacles[i].x = GetRandomValue(pistaInicioX, pistaInicioX + pistaLargura - OBSTACLE_SIZE);
        }

        if (obstacles[i].y > HEIGHT) {
            obstacles[i].y = 0;
            obstacles[i].x = GetRandomValue(pistaInicioX, pistaInicioX + pistaLargura - OBSTACLE_SIZE);
            player->score++; // aumenta o score ao desviar
        }
    }
}

void DrawObstacle(Obstacle obst, Texture2D tex) {
    Rectangle sourceRec = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle destRec = { (float)obst.x, (float)obst.y, OBSTACLE_SIZE, OBSTACLE_SIZE };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(tex, sourceRec, destRec, origin, 0.0f, WHITE);
}

void InserirRanking(char* nome, int score, double tempo) {
    RankingNode* novo = malloc(sizeof(RankingNode));
    strcpy(novo->nome, nome);
    novo->score = score;
    novo->tempo = tempo;
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
        fprintf(f, "%s %d %.2f\n", node->nome, node->score, node->tempo);
        node = node->next;
    }
    fclose(f);
}

void CarregarRanking() {
    FILE* f = fopen("ranking.txt", "r");
    if (!f) return;
    char nome[50];
    int score;
    double tempo;
    while (fscanf(f, "%s %d %lf", nome, &score, &tempo) != EOF)
        InserirRanking(nome, score, tempo);
    fclose(f);
}

void MostrarRanking() {
    int y = 200;
    RankingNode* node = rankingHead;
    DrawText("Ranking:", 10, y - 30, 25, DARKBLUE);
    while (node) {
        int min = (int)(node->tempo / 60);
        int seg = (int)node->tempo % 60;
        DrawText(TextFormat("%s - %d pts - %02d:%02d", node->nome, node->score, min, seg), 10, y, 20, BLACK);
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
    if (texPlayer.id == 0) {
        printf("Erro ao carregar player.png\n");
    }
    texObstacle = LoadTexture("assets/obstaculo.png");
    if (texObstacle.id == 0) {
        printf("Erro ao carregar obstaculo.png\n");
    }
    texArvore = LoadTexture("assets/arvore.png");
    if (texArvore.id == 0) {
        printf("Erro ao carregar arvore.png\n");
    }

    Player jogador = {0};
    jogador.hp = MAX_HP;
    jogador.score = 0;
    jogador.x = WIDTH / 2 - PLAYER_SIZE / 2; // Centraliza horizontalmente

    CarregarRanking();

    char nome[50];
    printf("Digite seu nickname: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;
    strcpy(jogador.nome, nome);

    InitObstacles();

    // Tela de menu inicial
    int naTelaMenu = 1;
    while (!WindowShouldClose() && naTelaMenu) {
        BeginDrawing();
        ClearBackground(DARKGREEN);

        DrawText("JOGO DE ESQUIVA", WIDTH / 2 - 160, HEIGHT / 2 - 100, 40, WHITE);
        DrawText("Pressione ENTER para jogar", WIDTH / 2 - 180, HEIGHT / 2, 25, YELLOW);
        DrawText("Setas <- e -> para mover", WIDTH / 2 - 160, HEIGHT / 2 + 40, 20, LIGHTGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            naTelaMenu = 0;
        }
    }

    // Cronômetro de 3 segundos antes de iniciar o jogo
    double startTime = GetTime();
    double tempoInicio = GetTime();
    int countdown = 3;
    while (countdown > 0 && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("O jogo começa em: %d", countdown), WIDTH/2 - 120, HEIGHT/2 - 20, 30, RED);
        EndDrawing();

        if ((int)(GetTime() - startTime) >= 1) {
            countdown--;
            startTime = GetTime();
        }
    }

    int gameOver = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- CENÁRIO ---
        int pistaLargura = WIDTH / 2;
        int pistaInicioX = (WIDTH - pistaLargura) / 2;
        int pistaFimX = pistaInicioX + pistaLargura;

        // Primeiro desenhe a pista e as áreas verdes
        DrawRectangle(pistaInicioX, 0, pistaLargura, HEIGHT, DARKGRAY);
        DrawRectangle(0, 0, pistaInicioX, HEIGHT, DARKGREEN);
        DrawRectangle(pistaFimX, 0, (WIDTH - pistaLargura) / 2, HEIGHT, DARKGREEN);

        // Depois desenhe os pontilhados
        for (int y = 0; y < HEIGHT; y += 40) {
            DrawRectangle(WIDTH/2 - 5, y, 10, 20, YELLOW);
        }

        // --- RESTANTE DO JOGO ---
        if (!gameOver) {
            // Lógica
            if (IsKeyDown(KEY_LEFT) && jogador.x > pistaInicioX)
                jogador.x -= 6;
            if (IsKeyDown(KEY_RIGHT) && jogador.x < pistaFimX - PLAYER_SIZE)
                jogador.x += 6;

            UpdateObstacles(&jogador);

            jogador.tempo = GetTime() - tempoInicio;

            if (jogador.hp <= 0) {
                gameOver = 1;
                InserirRanking(jogador.nome, jogador.score, jogador.tempo);
                SalvarRanking();
            }

            // Desenho do HUD, player e obstáculos
            DrawText(TextFormat("Jogador: %s", jogador.nome), 10, 10, 20, BLACK);
            DrawText(TextFormat("Score: %d", jogador.score), 10, 35, 20, DARKGREEN);
            DrawText("HP: ", 10, 60, 20, RED);
            for (int i = 0; i < MAX_HP; i++)
                DrawRectangle(60 + i * 25, 60, 20, 20, (i < jogador.hp) ? RED : LIGHTGRAY);

            double tempoAtual = GetTime() - tempoInicio;
            int minutos = (int)(tempoAtual / 60);
            int segundos = (int)tempoAtual % 60;
            DrawText(TextFormat("Tempo: %02d:%02d", minutos, segundos), 10, 85, 20, BLACK);

            // Player com textura
            Rectangle src = {0, 0, (float)texPlayer.width, (float)texPlayer.height};
            Rectangle dest = {(float)jogador.x, (float)(HEIGHT - PLAYER_SIZE), PLAYER_SIZE, PLAYER_SIZE};
            Vector2 origin = {0, 0};
            DrawTexturePro(texPlayer, src, dest, origin, 0, WHITE);

            // Obstáculos com textura
            for (int i = 0; i < OBSTACLE_COUNT; i++) {
                DrawObstacle(obstacles[i], texObstacle);
            }
        } else {
            DrawText("Game Over!", 300, 100, 40, RED);
            DrawText(TextFormat("Score: %d", jogador.score), 330, 160, 25, BLACK);
            MostrarRanking();
        }

        EndDrawing();
    }

    UnloadTexture(texPlayer);
    UnloadTexture(texObstacle);
    UnloadTexture(texArvore);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
