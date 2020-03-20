#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv){
    int32_t in0 = 1;
    int32_t in1 = 3;

    if(argc > 2){
        in0 = atoi(argv[1]);
        in1 = atoi(argv[2]);
    }


    uint32_t last = UINT32_MAX - in1;
    uint32_t now = in0;

    int32_t delay = (int32_t)(now - last);
    if(delay < 0){
        // 这里应该就是时间戳回退了，处理一下
       delay = (UINT32_MAX - last) + now;
    }    

    printf("delay=%d\n", delay);

#define LEN     128
    const char* src = NULL;
    //const char* src = "ddddslsl";
    char dst[LEN + 1];
    memset(dst, 0, sizeof(dst));
    strncpy(dst, src, LEN);
    printf("strncpy success....\n");

    return 0;
}
