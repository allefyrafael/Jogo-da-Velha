#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define JOGADOR 'X'
#define COMPUTADOR 'O'

// Função para imprimir o tabuleiro do jogo
void printTabuleiro(char tabuleiro[3][3]) {
    printf("\nTabuleiro:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < 2) printf("|");
        }
        if (i < 2) printf("\n-----------\n");
    }
    printf("\n");
}

// Função auxiliar para verificar se três posições têm o mesmo valor (não vazio)
int verificarLinha(char a, char b, char c) {
    return (a == b && b == c && a != ' ') ? a : 0;
}

// Função para verificar o vencedor do jogo
int verificarVencedor(char tabuleiro[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (verificarLinha(tabuleiro[i][0], tabuleiro[i][1], tabuleiro[i][2])) return tabuleiro[i][0];
        if (verificarLinha(tabuleiro[0][i], tabuleiro[1][i], tabuleiro[2][i])) return tabuleiro[0][i];
    }
    if (verificarLinha(tabuleiro[0][0], tabuleiro[1][1], tabuleiro[2][2])) return tabuleiro[0][0];
    if (verificarLinha(tabuleiro[0][2], tabuleiro[1][1], tabuleiro[2][0])) return tabuleiro[0][2];
    return 0;
}

// Função para verificar se o jogo terminou em empate
int checarEmpate(char tabuleiro[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (tabuleiro[i][j] == ' ')
                return 0;
    return 1;
}

// Função para garantir que a entrada do jogador seja válida
void jogarJogador(char tabuleiro[3][3], char jogador) {
    int escolha;
    do {
        printf("\nJogador %c, escolha uma posição (1 a 9): ", jogador);
        if (scanf("%d", &escolha) != 1 || escolha < 1 || escolha > 9) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n');  // Limpa o buffer
        } else {
            int linha = (escolha - 1) / 3;
            int coluna = (escolha - 1) % 3;
            if (tabuleiro[linha][coluna] != ' ') {
                printf("Posição já ocupada! Escolha outra.\n");
            } else {
                tabuleiro[linha][coluna] = jogador;
                break;
            }
        }
    } while (1);
}

// Função para a estratégia do computador (bloquear ou vencer)
int computadorBloquearOuVencer(char tabuleiro[3][3], char marcador) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') {
                tabuleiro[i][j] = marcador;
                if (verificarVencedor(tabuleiro) == marcador) return 1;
                tabuleiro[i][j] = ' ';
            }
        }
    }
    return 0;
}

// Estratégia do computador no nível Médio
void jogarComputadorMedio(char tabuleiro[3][3]) {
    if (computadorBloquearOuVencer(tabuleiro, COMPUTADOR)) return;
    if (computadorBloquearOuVencer(tabuleiro, JOGADOR)) return;

    // Jogada aleatória caso não haja vitória iminente
    int linha, coluna;
    do {
        linha = rand() % 3;
        coluna = rand() % 3;
    } while (tabuleiro[linha][coluna] != ' ');
    tabuleiro[linha][coluna] = COMPUTADOR;
}

// Estratégia do computador no nível Difícil
void jogarComputadorDificil(char tabuleiro[3][3]) {
    // Tenta vencer
    if (computadorBloquearOuVencer(tabuleiro, COMPUTADOR)) return;
    // Bloqueia o jogador
    if (computadorBloquearOuVencer(tabuleiro, JOGADOR)) return;
    // Joga no centro se estiver livre
    if (tabuleiro[1][1] == ' ') {
        tabuleiro[1][1] = COMPUTADOR;
        return;
    }
    // Joga nas extremidades livres
    int cantos[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    for (int i = 0; i < 4; i++) {
        if (tabuleiro[cantos[i][0]][cantos[i][1]] == ' ') {
            tabuleiro[cantos[i][0]][cantos[i][1]] = COMPUTADOR;
            return;
        }
    }
    // Joga aleatoriamente caso não haja outra opção
    int linha, coluna;
    do {
        linha = rand() % 3;
        coluna = rand() % 3;
    } while (tabuleiro[linha][coluna] != ' ');
    tabuleiro[linha][coluna] = COMPUTADOR;
}

// Função de minimax para o nível Impossível do computador
int minimax(char tabuleiro[3][3], int profundidade, int isMaximizing) {
    int vencedor = verificarVencedor(tabuleiro);
    if (vencedor == COMPUTADOR) return 10 - profundidade;
    if (vencedor == JOGADOR) return profundidade - 10;
    if (checarEmpate(tabuleiro)) return 0;

    if (isMaximizing) {
        int melhorPontuacao = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tabuleiro[i][j] == ' ') {
                    tabuleiro[i][j] = COMPUTADOR;
                    int pontuacao = minimax(tabuleiro, profundidade + 1, 0);
                    tabuleiro[i][j] = ' ';
                    if (pontuacao > melhorPontuacao) melhorPontuacao = pontuacao;
                }
            }
        }
        return melhorPontuacao;
    } else {
        int melhorPontuacao = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tabuleiro[i][j] == ' ') {
                    tabuleiro[i][j] = JOGADOR;
                    int pontuacao = minimax(tabuleiro, profundidade + 1, 1);
                    tabuleiro[i][j] = ' ';
                    if (pontuacao < melhorPontuacao) melhorPontuacao = pontuacao;
                }
            }
        }
        return melhorPontuacao;
    }
}

// Função para estratégia do computador Impossível
void jogarComputadorImpossivel(char tabuleiro[3][3]) {
    int melhorPontuacao = -1000;
    int melhor_linha = -1, melhor_coluna = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') {
                tabuleiro[i][j] = COMPUTADOR;
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
    tabuleiro[melhor_linha][melhor_coluna] = COMPUTADOR;
}

// Função de configuração inicial do jogo
void configurarJogo(int *modoDeJogo, int *dificuldade) {
    printf("\033[1;36mBem-vindo ao Jogo da Velha!\033[0m\n");
    printf("Escolha o modo de jogo:\n1. Jogador contra Jogador\n2. Jogador contra Computador\nOpção: ");
    scanf("%d", modoDeJogo);
    if (*modoDeJogo == 2) {
        printf("Escolha a dificuldade:\n1. Médio\n2. Difícil\n3. Impossível\nOpção: ");
        scanf("%d", dificuldade);
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    char tabuleiro[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    int modoDeJogo, dificuldade = 1;

    configurarJogo(&modoDeJogo, &dificuldade);

    char jogadorAtual = JOGADOR;
    int vencedor = 0;

    while (!vencedor && !checarEmpate(tabuleiro)) {
        printTabuleiro(tabuleiro);

        if (modoDeJogo == 1 || (modoDeJogo == 2 && jogadorAtual == JOGADOR)) {
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
        jogadorAtual = (jogadorAtual == JOGADOR) ? COMPUTADOR : JOGADOR;
    }

    printTabuleiro(tabuleiro);

    if (vencedor) {
        printf("\033[1;32mParabéns! O jogador %c venceu!\033[0m\n", vencedor);
    } else {
        printf("\033[1;33mEmpate!\033[0m\n");
    }

    return 0;
}
