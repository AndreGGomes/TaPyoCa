#include <stdio.h>
#include <stdlib.h>

void ler_e_dobrar(int *valor) {
    if (scanf("%d", &(*valor)) != 1) { fprintf(stderr, "Erro em tempo de execucao: esperado 'int'.\n"); exit(1); }
    (*valor) = (*valor) * 2;
}

void trocar(int *a, int *b) {
    int aux = (*a);
    (*a) = (*b);
    (*b) = aux;
}

int main(void) {
    int x = 0;
    int y = 5;
    ler_e_dobrar(&(x));
    trocar(&(x), &(y));
    printf("%d\n", x);
    printf("%d\n", y);
    return 0;
}

