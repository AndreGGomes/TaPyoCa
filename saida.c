#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int contador = 0;
    L0:
    if (!(contador < 10)) goto L1;
    {
        printf("%d\n", contador);
        if (!(contador == 3)) goto L2;
            goto L1;
        L2:
        contador = contador + 1;
    }
    goto L0;
    L1: ;
    return 0;
}

