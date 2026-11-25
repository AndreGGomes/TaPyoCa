#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int a;
    L0:
    if( !(1 == 1) ) goto L1;
    {
        if( scanf("%d", &a) == 1 ) goto L2;
        fprintf(stderr, "Erro em tempo de execucao: esperado 'int'.\n");
        exit(1);
        L2:;
        if( !(a > 0) ) goto L3;
        {
            printf("%s\n", "o numero digitado é positivo");
        }
        goto L4;
        L3: ;
        {
            if( !(a < 0) ) goto L5;
            {
                printf("%s\n", "o numero digitado é negativo");
            }
            goto L6;
            L5: ;
            {
                printf("%s\n", "o numero digitado é zero, parando o programa");
                goto L1;
            }
            L6: ;
        }
        L4: ;
    }
    goto L0;
    L1: ;
    return 0;
}

