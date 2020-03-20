//
// Created by mj on 19-5-6.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "VVMIDIParser.h"
#include "UtilLog.h"

namespace vvmidi{
VVMIDIParser::VVMIDIParser() {
    mMIDIHeader = NULL;
    mMIDISegLength = 0;
    mValidMIDISegLength = 0;
    mBigEndian = false;
    mParseState = INVALID_FILE_NAME;
}

VVMIDIParser::~VVMIDIParser() {
    release();
}

int VVMIDIParser::parse(const char *midiFileName) {
    if(mMIDIHeader){
        release();
    }

    FILE* pFILE = NULL;
    int ret = OK;
    long filesize = 0;
    uint8_t *pData = NULL;
    uint8_t *pBuf = NULL;
    uint32_t header_size = 0;
    int32_t offset = 0;
    uint32_t crc = 0;
    uint32_t checkCount = 0;
    uint32_t midiSegmentSize = 0;

    if(!midiFileName){
        ret = INVALID_FILE_NAME;
        LOGE("invalid midi filename NULL");
        goto parseerr;
    }

    pFILE = fopen(midiFileName, "r");
    if(!pFILE){
        ret = FILE_NOT_FOUND;
        LOGE("midi file name not found or no permission,file:%s", midiFileName);
        goto parseerr;
    }

    struct stat statbuff;
    if(stat(midiFileName, &statbuff) < 0){

    }else{
        filesize = statbuff.st_size;
    }

    if(filesize < sizeof(midi_file_header)){
        ret = INVALID_MIDI_DATA;
        LOGE("midi file invalid,file:%s,filesize:%ld,sizeof(midi_file_header):%d",
             midiFileName, filesize, sizeof(midi_file_header));
        goto parseerr;
    }

    pData = (uint8_t *) malloc(filesize * sizeof(uint8_t));
    if(!pData){
        ret = OOM;
        LOGE("malloc file data oom");
        goto parseerr;
    }

    fread(pData, 1, filesize, pFILE);

    mMIDIHeader = (midi_file_header *) malloc(sizeof(midi_file_header));
    if(!mMIDIHeader){
        ret = OOM;
        LOGE("malloc mMIDIHeader oom");
        goto parseerr;
    }

    memset(mMIDIHeader, 0, sizeof(midi_file_header));

    pBuf = pData;
    read_uint32(pBuf, mMIDIHeader->magic);
    pBuf += sizeof(uint32_t);
    read_uint32(pBuf, mMIDIHeader->segment_length);
    pBuf += sizeof(uint32_t);
    read_uint32(pBuf, mMIDIHeader->crc);
    pBuf += sizeof(uint32_t);

    header_size = sizeof(midi_file_header) - sizeof(midi_segment*);

    midiSegmentSize = filesize - header_size;
    if(mMIDIHeader->segment_length > 0){
        mMIDISegLength = midiSegmentSize / mMIDIHeader->segment_length;
    }
    checkCount = midiSegmentSize / sizeof(uint32_t);

    mValidMIDISegLength  = 0;

    if(mMIDISegLength > 0){
        mMIDIHeader->data = (midi_segment *) malloc(mMIDISegLength * sizeof(midi_segment));
        if(!mMIDIHeader->data){
            ret = OOM;
            LOGE("malloc mMIDIHeader->data oom");
            goto parseerr;
        }

        // check file
        offset = 0;
        crc = 0;
        for(int i = 0; i < checkCount; ++i){
            crc ^= read_uint32(pBuf + offset);
            offset += sizeof(uint32_t);
        }

        if(crc != mMIDIHeader->crc){
            LOGE("analyse crc error, expect:%u,calculate:%u", mMIDIHeader->crc, crc);
            ret = CRC_ERROR;
            goto parseerr;
        }

        int idx = 0;
        offset = 0;
        for(int i = 0; i < mMIDISegLength; ++i){
            read_int32(pBuf + offset, mMIDIHeader->data[idx].begin);
            offset += sizeof(int32_t);
            read_int32(pBuf + offset, mMIDIHeader->data[idx].length);
            offset += sizeof(int32_t);
            read_uint32(pBuf + offset, mMIDIHeader->data[idx].note);
            offset += sizeof(uint32_t);

            if(isValidNote(idx)){
                ++idx;
            }

            // 这里兼容新加字段，每次buf前进segment_length
            pBuf += mMIDIHeader->segment_length;
            offset = 0;
        }
        mValidMIDISegLength = idx;
    }

parseerr:
    if(pFILE){
        fclose(pFILE);
    }

    if(pData){
        free(pData);
    }

    mParseState = (MIDIError) ret;

    return ret;
}

void VVMIDIParser::release() {
    if(mMIDIHeader){
        if(mMIDIHeader->data){
            free(mMIDIHeader->data);
            mMIDIHeader->data = NULL;
        }
        free(mMIDIHeader);
        mMIDIHeader = NULL;
        mMIDISegLength = 0;
    }
}

bool VVMIDIParser::isValidNote(int idx) {
    //return true;
    if(idx >= mMIDISegLength){
        return false;
    }

    if(idx > 0){
        if(mMIDIHeader->data[idx].begin <= mMIDIHeader->data[idx - 1].begin) {
            return false;
        }
    }

    if(mMIDIHeader->data[idx].length < 10){
        return false;
    }

    if(mMIDIHeader->data[idx].note <=0 || mMIDIHeader->data[idx].note > 127){
        return false;
    }

    return true;
}

uint32_t VVMIDIParser::read_uint32(uint8_t *data) {
    uint32_t value;
    read_uint32(data, value);
    return value;
}

int32_t VVMIDIParser::read_int32(uint8_t *data) {
    int32_t value;
    read_int32(data, value);
    return value;
}

void VVMIDIParser::read_uint32(uint8_t *data, uint32_t &value) {
    if(mBigEndian){
        read_uint32_big_endian(data, value);
    } else{
        read_uint32_little_endian(data, value);
    }
}

void VVMIDIParser::read_int32(uint8_t *data, int32_t &value) {
    if(mBigEndian){
        read_int32_big_endian(data, value);
    } else{
        read_int32_little_endian(data, value);
    }
}

void VVMIDIParser::read_uint32_big_endian(uint8_t *data, uint32_t &value) {
    uint32_t v0 = data[0];
    uint32_t v1 = data[1];
    uint32_t v2 = data[2];
    uint32_t v3 = data[3];

    value = (v0 << 24) | (v1 << 16) | (v2 << 8) | v3;
}

void VVMIDIParser::read_uint32_little_endian(uint8_t *data, uint32_t &value) {
    uint32_t v0 = data[3];
    uint32_t v1 = data[2];
    uint32_t v2 = data[1];
    uint32_t v3 = data[0];

    value = (v0 << 24) | (v1 << 16) | (v2 << 8) | v3;
}

void VVMIDIParser::read_int32_big_endian(uint8_t *data, int32_t &value) {
    int32_t v0 = data[0];
    int32_t v1 = data[1];
    int32_t v2 = data[2];
    int32_t v3 = data[3];

    value = (v0 << 24) | (v1 << 16) | (v2 << 8) | v3;
}

void VVMIDIParser::read_int32_little_endian(uint8_t *data, int32_t &value) {
    int32_t v0 = data[3];
    int32_t v1 = data[2];
    int32_t v2 = data[1];
    int32_t v3 = data[0];

    value = (v0 << 24) | (v1 << 16) | (v2 << 8) | v3;
}


}
