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
    if( scanf("%255s", a.nome) == 1 ) goto L0;
    fprintf(stderr, "Erro em tempo de execucao: esperado 'string'.\n");
    exit(1);
    L0:;
    printf("%s\n", "digite sua idade:");
    if( scanf("%d", &a.idade) == 1 ) goto L1;
    fprintf(stderr, "Erro em tempo de execucao: esperado 'int'.\n");
    exit(1);
    L1:;
    if( !(a.idade >= 18) ) goto L2;
    {
        printf("%s %s\n", a.nome, "voce é maior de idade");
    }
    goto L3;
    L2: ;
    {
        printf("%s %s\n", a.nome, "você é menor de idade");
    }
    L3: ;
    return 0;
}

