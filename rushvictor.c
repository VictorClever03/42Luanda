#include <unistd.h> // Para write
#include <stdlib.h> // Para malloc e free

#define SIZE 4

// Função para verificar visibilidade
int check_visibility(int *line, int expected) {
    int max_height = 0, visible_count = 0;
    int i = 0;

    while (i < SIZE) {
        if (line[i] > max_height) {
            max_height = line[i];
            visible_count++;
        }
        i++;
    }
    return visible_count == expected;
}

// Função para verificar se o tabuleiro está correto
int is_valid(int **board, int *views) {
    int i = 0;
    while (i < SIZE) {
        if (!check_visibility(board[i], views[i])) return 0;
        i++;
    }
    
    int j = 0;
    while (j < SIZE) {
        int column[SIZE];
        int k = 0;
        while (k < SIZE) {
            column[k] = board[k][j];
            k++;
        }
        if (!check_visibility(column, views[SIZE + j])) return 0;
        j++;
    }
    return 1;
}

// Função de backtracking
int solve(int **board, int row, int col, int *views) {
    if (row == SIZE) {
        return is_valid(board, views);
    }
    
    int num = 1;
    while (num <= SIZE) {
        board[row][col] = num;
        if (col == SIZE - 1) {
            if (solve(board, row + 1, 0, views)) return 1;
        } else {
            if (solve(board, row, col + 1, views)) return 1;
        }
        board[row][col] = 0; // backtrack
        num++;
    }
    return 0;
}

// Função para imprimir o tabuleiro
void print_board(int **board) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            // Converter número para string manualmente
            char buffer[4]; // buffer para armazenar o número como string
            int n = board[i][j], len = 0;

            if (n == 0) {
                buffer[len++] = '0'; // tratar o zero
            } else {
                int temp = n;
                while (temp > 0) {
                    len++;
                    temp /= 10;
                }
                temp = n;
                for (int k = len - 1; k >= 0; k--) {
                    buffer[k] = (temp % 10) + '0'; // converter para caractere
                    temp /= 10;
                }
            }
            buffer[len] = ' '; // espaço após o número
            write(1, buffer, len + 1); // imprimir número e espaço
            j++;
        }
        write(1, "\n", 1); // nova linha
        i++;
    }
}

// Função para converter argumento em inteiro
int convert_arg(char *arg) {
    int result = 0;
    while (*arg >= '0' && *arg <= '9') {
        result = result * 10 + (*arg - '0');
        arg++;
    }
    return result;
}

// Função principal
int main(int argc, char **argv) {
    if (argc != 2) {
        write(1, "Error\n", 6);
        return 1;
    }

    int views[SIZE * 4];
    char *arg = argv[1];
    int count = 0;

    while (count < SIZE * 4) {
        views[count] = convert_arg(arg);
        while (*arg && *arg != ' ') arg++; // Pular espaços
        while (*arg == ' ') arg++; // Pular próximo número
        count++;
    }

    // Alocação de memória para o tabuleiro
    int **board = malloc(SIZE * sizeof(int *));
    int i = 0;
    while (i < SIZE) {
        board[i] = malloc(SIZE * sizeof(int));
        i++;
    }

    if (solve(board, 0, 0, views)) {
        print_board(board);
    } else {
        write(1, "Error\n", 6);
    }

    // Liberação de memória
    i = 0;
    while (i < SIZE) {
        free(board[i]);
        i++;
    }
    free(board);

    return 0;
}
