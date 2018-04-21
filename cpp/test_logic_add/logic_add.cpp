#include <stdio.h>

int add(int a, int b)
{
    int s = a ^ b;
    int c = a & b;
    while(c > 0){
        a = s;
        b = c << 1;
        s = a ^ b;
        c = a & b;
    }
    return s;
}


int main()
{
    printf("2+5=%d\n", add(2, 5));
    int value = 14;

    for(int i = 0; i < 50; ++i){
        printf("%d+%d=%d\n", i, value, add(i, value));
    }


    for(int i = 0; i < 50; ++i){
        printf("%d+%d=%d\n", -i, value, add(-i, value));
    }

    return 0;
}
