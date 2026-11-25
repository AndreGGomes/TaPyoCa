#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    double x;
    int intervalo1 = 0;
    int intervalo2 = 0;
    int intervalo3 = 0;
    int intervalo4 = 0;
    L0:
    if( !(1) ) goto L1;
    {
        if( scanf("%lf", &x) == 1 ) goto L2;
        fprintf(stderr, "Erro em tempo de execucao: esperado 'real'.\n");
        exit(1);
        L2:;
        if( !(x < 0) ) goto L3;
        {
            goto L1;
        }
        goto L4;
        L3: ;
        {
            if( !(x >= 0 && x <= 25) ) goto L5;
            {
                intervalo1 = intervalo1 + 1;
            }
            L5: ;
            if( !(x >= 26 && x <= 50) ) goto L7;
            {
                intervalo2 = intervalo2 + 1;
            }
            L7: ;
            if( !(x >= 51 && x <= 75) ) goto L9;
            {
                intervalo3 = intervalo3 + 1;
            }
            L9: ;
            if( !(x >= 76 && x <= 100) ) goto L11;
            {
                intervalo4 = intervalo4 + 1;
            }
            L11: ;
        }
        L4: ;
    }
    goto L0;
    L1: ;
    printf("%s %d\n", "numeros no intervalo [0, 25]:", intervalo1);
    printf("%s %d\n", "numeros no intervalo [26, 50]:", intervalo2);
    printf("%s %d\n", "numeros no intervalo [51, 75]:", intervalo3);
    printf("%s %d\n", "numeros no intervalo [76, 100]:", intervalo4);
    return 0;
}

