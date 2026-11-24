#include <stdio.h>
#include <stdlib.h>

struct Pessoa {
    char* nome;
    int idade;
};

int main(void) {
    struct Pessoa a;
    printf("%s\n", "digite seu nome:");
    a.nome = malloc(256);
    if (scanf("%255s", a.nome) != 1) { fprintf(stderr, "Erro em tempo de execucao: esperado 'string'.\n"); exit(1); }
    printf("%s\n", "digite sua idade:");
    if (scanf("%d", &a.idade) != 1) { fprintf(stderr, "Erro em tempo de execucao: esperado 'int'.\n"); exit(1); }
    printf("%s %s %s %d %s\n", "Oi", a.nome, "você tem", a.idade, "anos!");
    return 0;
}

