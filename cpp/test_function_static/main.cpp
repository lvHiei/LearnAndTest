#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>

#include <cstdio>

#define STATIC_TEST(str, b) {\
    if(b){ \ 
        static int s_uinon = 1; \
        s_uinon++; \
        printf("%s %d \n",str,  s_uinon); \
    } \
}

int func1(){
    static int s_func = 0;
    s_func++;

    {
        static int s_uinon = 1;
        s_uinon++;
        printf("fucking1 %d \n", s_uinon);
    }

    {
        static int s_uinon = 1;
        s_uinon++;
        printf("fucing2 %d \n", s_uinon);
    }

    STATIC_TEST("fffffff", 1)
    STATIC_TEST("eeeeeee", 0)


    printf("func1_sfunc %p\n", &s_func);
    return s_func;
}

int func2(){
    static int s_func = 2;
    func1();
    printf("func2_sfunc %p\n", &s_func);
    s_func++;
    return s_func;
}

int main(){
    for(int i = 0; i < 10; i++){
        printf("func1 %d\n", func1());
        printf("func2 %d\n\n", func2());
    }    

    return 0;
}
