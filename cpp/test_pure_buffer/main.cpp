#include <stdio.h>
#include <malloc.h>
#include "PureBuffer.h"

using namespace vvav;

int main(){

    PureBuffer buffer;

    uint8_t* pbuf = (uint8_t*)malloc(1024);
    uint8_t* obuf = (uint8_t*)malloc(64);
    
    for (int i = 0 ;i < 1024; ++i){
        pbuf[i] = i % 255;
    }

    buffer.push(pbuf, 1024);

    while(buffer.size() >= 64){
        buffer.pop(obuf, 64);
        for(int i = 0 ; i < 6; i++){
            printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n", obuf[i * 10 + 0], obuf[i * 10 + 2], obuf[i * 10 + 4], obuf[i * 10 + 6]
                    , obuf[i * 10 + 8], obuf[i * 10 + 0xa], obuf[i * 10 + 0xc], obuf[i * 10 + 0xe] );
        }
    }

    free(pbuf);
    free(obuf);
    return 0;
}
