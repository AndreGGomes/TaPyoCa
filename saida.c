#include <stdio.h>
#include <stdlib.h>

int processar_matriz(int* amostra, int limite, int fator) {
    int acumulador = 0;
    int idx = 0;
            acumulador = acumulador + (amostra[idx] * fator) / 2;
        idx = idx + 1;
}

void imprimir_alerta(char* mensagem, int codigo) {
    printf("%s\n", mensagem, codigo);
}

int main(void) {
    int* valores;
    int n = 5;
    int multiplicador = 3;
    int resultado = processar_matriz(valores, n, multiplicador);
    return 0;
}

