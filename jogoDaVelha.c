#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void printTabuleiro(char tabuleiro[3][3]) {
    printf("\nTabuleiro:\n");
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < 2) printf("|");
        }
        if (i < 2) printf("\n-----------\n");
    }
    printf("\n");
}

int verificarVencedor(char tabuleiro[3][3]) {
	int i;
    for (i = 0; i < 3; i++) {
        if (tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2] && tabuleiro[i][0] != ' ')
            return tabuleiro[i][0];
        if (tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i] && tabuleiro[0][i] != ' ')
            return tabuleiro[0][i];
    }
    if (tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2] && tabuleiro[0][0] != ' ')
        return tabuleiro[0][0];
    if (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0] && tabuleiro[0][2] != ' ')
        return tabuleiro[0][2];

    return 0;
}

int checarEmpate(char tabuleiro[3][3]) {
	int i;
	int j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (tabuleiro[i][j] == ' ')
                return 0;
    return 1;
}

void jogarJogador(char tabuleiro[3][3], char jogador) {
    int escolha;
    printf("\nJogador %c, escolha uma posição: ", jogador);
    scanf("%d", &escolha);

    int linha = (escolha - 1) / 3;
    int coluna = (escolha - 1) % 3;

    if (tabuleiro[linha][coluna] != ' ') {
        printf("Posição inválida! Tente novamente.\n");
        jogarJogador(tabuleiro, jogador);
    } else {
        tabuleiro[linha][coluna] = jogador;
    }
}

void jogarComputadorMedio(char tabuleiro[3][3]) {
    // Estratégia básica: bloquear o jogador
    int i ;
    int j;
    for ( i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') {
                tabuleiro[i][j] = 'O';
                if (verificarVencedor(tabuleiro) == 'O') return;
                tabuleiro[i][j] = 'X';
                if (verificarVencedor(tabuleiro) == 'X') {
                    tabuleiro[i][j] = 'O';
                    return;
                }
                tabuleiro[i][j] = ' ';
            }
        }
    }
    // Caso não haja vitória iminente, jogue aleatoriamente
    int linha, coluna;
    do {
        linha = rand() % 3;
        coluna = rand() % 3;
    } while (tabuleiro[linha][coluna] != ' ');
    tabuleiro[linha][coluna] = 'O';
}

void jogarComputadorDificil(char tabuleiro[3][3]) {
    // Estratégia um pouco mais avançada
    // Verifica se pode vencer
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') {
                tabuleiro[i][j] = 'O';
                if (verificarVencedor(tabuleiro) == 'O') return;
                tabuleiro[i][j] = ' ';
            }
        }
    }
    // Bloqueia o jogador se ele puder vencer

    for ( i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') {
                tabuleiro[i][j] = 'X';
                if (verificarVencedor(tabuleiro) == 'X') {
                    tabuleiro[i][j] = 'O';
                    return;
                }
                tabuleiro[i][j] = ' ';
            }
        }
    }
    // Caso contrário, jogue no centro se estiver livre
    if (tabuleiro[1][1] == ' ') {
        tabuleiro[1][1] = 'O';
        return;
    }
    // Caso contrário, jogue nas extremidades
 
    int cantos[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    for ( i = 0; i < 4; i++) {
        if (tabuleiro[cantos[i][0]][cantos[i][1]] == ' ') {
            tabuleiro[cantos[i][0]][cantos[i][1]] = 'O';
            return;
        }
    }
    // Jogue aleatoriamente se nada acima for possível
    int linha, coluna;
    do {
        linha = rand() % 3;
        coluna = rand() % 3;
    } while (tabuleiro[linha][coluna] != ' ');
    tabuleiro[linha][coluna] = 'O';
}

int minimax(char tabuleiro[3][3], int profundidade, int isMaximizing) {
    int vencedor = verificarVencedor(tabuleiro);
    if (vencedor == 'O') return 10 - profundidade;
    if (vencedor == 'X') return profundidade - 10;
    if (checarEmpate(tabuleiro)) return 0;

    if (isMaximizing) {
        int melhorPontuacao = -1000;
        int i;
        int j;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (tabuleiro[i][j] == ' ') {
                    tabuleiro[i][j] = 'O';
                    int pontuacao = minimax(tabuleiro, profundidade + 1, 0);
                    tabuleiro[i][j] = ' ';
                    if (pontuacao > melhorPontuacao) melhorPontuacao = pontuacao;
                }
            }
        }
        return melhorPontuacao;
    } else {
        int melhorPontuacao = 1000;
        int i;
        int j;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (tabuleiro[i][j] == ' ') {
                    tabuleiro[i][j] = 'X';
                    int pontuacao = minimax(tabuleiro, profundidade + 1, 1);
                    tabuleiro[i][j] = ' ';
                    if (pontuacao < melhorPontuacao) melhorPontuacao = pontuacao;
                }
            }
        }
        return melhorPontuacao;
    }
}

void jogarComputadorImpossivel(char tabuleiro[3][3]) {
    int melhorPontuacao = -1000;
    int melhor_linha = -1, melhor_coluna = -1;
    int i;
    int j ;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') {
                tabuleiro[i][j] = 'O';
                int pontuacao = minimax(tabuleiro, 0, 0);
                tabuleiro[i][j] = ' ';
                if (pontuacao > melhorPontuacao) {
                    melhorPontuacao = pontuacao;
                    melhor_linha = i;
                    melhor_coluna = j;
                }
            }
        }
    }
    tabuleiro[melhor_linha][melhor_coluna] = 'O';
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    char tabuleiro[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    int modoDeJogo, dificuldade;
    int jogadas = 0;

    printf("\033[1;36mBem-vindo ao Jogo da Velha, se você ganhar eu te dou R$1500,00!\033[0m\n");
    printf("Escolha o modo de jogo:\n");
    printf("1. Jogador contra Jogador\n");
    printf("2. Jogador contra Computador\n");
    printf("Opção: ");
    scanf("%d", &modoDeJogo);

    if (modoDeJogo == 2) {
        printf("Escolha a dificuldade:\n");
        printf("1. Médio\n");
        printf("2. Difícil\n");
        printf("3. Impossível\n");
        printf("Opção: ");
        scanf("%d", &dificuldade);
    }

    char jogadorAtual = 'X';
    int vencedor = 0;

    while (!vencedor && !checarEmpate(tabuleiro)) {
        printTabuleiro(tabuleiro);

        if (modoDeJogo == 1 || (modoDeJogo == 2 && jogadorAtual == 'X')) {
            jogarJogador(tabuleiro, jogadorAtual);
        } else {
            switch (dificuldade) {
                case 1:
                    jogarComputadorMedio(tabuleiro);
                    break;
                case 2:
                    jogarComputadorDificil(tabuleiro);
                    break;
                case 3:
                    jogarComputadorImpossivel(tabuleiro);
                    break;
                default:
                    printf("Dificuldade inválida!\n");
                    return 1;
            }
        }

        vencedor = verificarVencedor(tabuleiro);
        jogadorAtual = (jogadorAtual == 'X') ? 'O' : 'X';
        jogadas++;
    }

    printTabuleiro(tabuleiro);

    if (vencedor) {
        printf("\033[1;32mParabéns! O jogador %c venceu!\033[0m\n", vencedor);
    } else {
        printf("\033[1;33mEmpate!\033[0m\n");
    }

    return 0;
}
