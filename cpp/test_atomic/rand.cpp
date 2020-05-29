#include "inc.h"
#include <stdlib.h>

void init_random()
{
    srand (time(NULL));
}

int random(int min, int max)
{
    if( min == max){
        return min;
    }

    if(min > max){
        return random(max, min);
    }

    int r = rand() % (max - min);
    return r + min;
}
