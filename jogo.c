#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <conio.h>

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

typedef struct RankingNode {
    char nome[50];
    int score;
    
    struct RankingNode* next;
} RankingNode;

Obstacle obstacles[OBSTACLE_COUNT];
int playerX = WIDTH / 4;
RankingNode* rankingHead = NULL;

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
        if (i < jogador.hp) printf("[ # ]");
        else printf("[ ]");
    }
    printf("\n");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int drawn = 0;
            for (int i = 0; i < OBSTACLE_COUNT; i++) {
                if (obstacles[i].x == x && obstacles[i].y == y) {
                    printf("X");
                    drawn = 1;
                    break;
                }
            }
            if (!drawn && y == HEIGHT - 1 && x == playerX) {
                printf("A");
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
        if (obstacles[i].y == HEIGHT - 1 && obstacles[i].x == playerX) {
            jogador->hp--;
            obstacles[i].y = 0;
            obstacles[i].x = rand() % WIDTH;
        }
        if (obstacles[i].y >= HEIGHT) {
            obstacles[i].y = 0;
            obstacles[i].x = rand() % WIDTH;
            jogador->score++;
        }
    }
}

void inserirRanking(char* nome, int score) {
    RankingNode* novo = (RankingNode*)malloc(sizeof(RankingNode));
    strcpy(novo->nome, nome);
    novo->score = score;
    novo->next = NULL;
    if (rankingHead == NULL || score > rankingHead->score) {
        novo->next = rankingHead;
        rankingHead = novo;
    } else {
        RankingNode* atual = rankingHead;
        while (atual->next != NULL && atual->next->score >= score) {
            atual = atual->next;
        }
        novo->next = atual->next;
        atual->next = novo;
    }
}

void salvarRanking() {
    FILE* arquivo = fopen("ranking.txt", "w");
    if (arquivo == NULL) return;
    RankingNode* atual = rankingHead;
    while (atual != NULL) {
        fprintf(arquivo, "%s %d\n", atual->nome, atual->score);
        atual = atual->next;
    }
    fclose(arquivo);
}

void carregarRanking() {
    FILE* arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) return;
    char nome[50];
    int score;
    while (fscanf(arquivo, "%s %d", nome, &score) != EOF) {
        inserirRanking(nome, score);
    }
    fclose(arquivo);
}

void exibirRanking() {
    printf("\n===== Ranking de Jogadores =====\n");
    RankingNode* atual = rankingHead;
    int posicao = 1;
    while (atual != NULL) {
        printf("%dº - %s: %d pontos\n", posicao, atual->nome, atual->score);
        atual = atual->next;
        posicao++;
    }
}

int main() {
    setlocale(LC_ALL, "");
    system("chcp 65001 > nul");
    player jogador;
    carregarRanking();
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
            if (key == 75 && playerX > 0) playerX--;
            if (key == 77 && playerX < WIDTH - 1) playerX++;
        }
        updateObstacles(&jogador);
        if (jogador.hp <= 0) {
            running = 0;
        }
        _sleep(1)
        ;
    }
    system("cls");
    printf("\n______________Fim de jogo, %s!______________\n", jogador.nome);
    printf("________________Score final: %d________________\n", jogador.score);
    inserirRanking(jogador.nome, jogador.score);
    salvarRanking();
    exibirRanking();
    return 0;
}
