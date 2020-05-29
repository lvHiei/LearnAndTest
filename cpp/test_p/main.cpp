#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

const int p = 88;
const int loopCount = 100000;

int f(){
    int rd = rand() % 100;
    if(rd < p){
        return 0;
    }

    return 1;
}

int g(){

    while(1){
        int f0 = f();
        int f1 = f();

        if(f0 == 0 && f1 == 1){
            return 0;
        }

        if(f0 == 1 && f1 == 0){
            return 1;
        }
    }

    return 0;
}

void testf(){
    int count0 = 0;
    int count1 = 0;
    for(int i = 0; i < loopCount; ++i){
        if(f()){
            count1++;
        }else{
            count0++;
        }
    }

    printf("c0:%d,c1:%d,p0:%.2f,p1:%.2f\n", count0, count1, 1.0*count0/loopCount, 1.0*count1/loopCount);
}

void testg(){
    int count0 = 0;
    int count1 = 0;
    for(int i = 0; i < loopCount; ++i){
        if(g()){
            count1++;
        }else{
            count0++;
        }
    }

    printf("c0:%d,c1:%d,p0:%.2f,p1:%.2f\n", count0, count1, 1.0*count0/loopCount, 1.0*count1/loopCount);
}

int main()
{
    srand (time(NULL));

    testf();
    testg();

    return 0;
}
