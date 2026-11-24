#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int idade = 20;
    int aprovado = 1;
    if (!(idade >= 18)) goto L0;
        if (!(aprovado == 1)) goto L1;
            printf("%s\n", "Aprovado");
        L1:
    L0:
    return 0;
}

