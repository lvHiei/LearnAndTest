//
// Created by mj on 19-5-6.
//

#ifndef TOOLKIT_VVMIDIPARSER_H
#define TOOLKIT_VVMIDIPARSER_H

#include <stdint.h>

namespace vvmidi{

#pragma pack(push)  //保存对齐状态
#pragma pack(1)     //设定为1字节对齐

struct midi_segment
{
    // 开始时间，单位毫秒
    int32_t begin;

    // 持续时长，单位毫秒
    int32_t length;

    // midi note
    uint32_t note;
};

struct midi_file_header
{
    uint32_t magic;

    uint32_t segment_length;

    uint32_t crc;

    midi_segment* data;
};

#pragma pack(pop)//恢复对齐状态

class VVMIDIParser {
public:
    enum MIDIError{
        OK = 0,             // succuss
        INVALID_FILE_NAME,  // filename is invalid
        FILE_NOT_FOUND,     // file not found or no permission
        INVALID_MIDI_DATA,  // midi file is not correct
        CRC_ERROR,          // crc check error
        OOM,                // out of memory
    };
public:
    VVMIDIParser();

    ~VVMIDIParser();

public:
    /**
     * parse midi file
     * @param midiFileName midi file name absolute path
     * @return 0 on success, otherwise error
     */
    int parse(const char *midiFileName);

    /**
     * get midi header
     * @return midi header if parse ok, otherwise NULL
     */
    midi_file_header* getMIDIHeader()const { return mParseState == OK ? mMIDIHeader : NULL;}
    uint32_t getMIDISegmentLength()const { return mValidMIDISegLength;}

    void release();

private:
    bool isValidNote(int idx);
    uint32_t read_uint32(uint8_t* data);
    int32_t read_int32(uint8_t* data);
    void read_uint32(uint8_t* data, uint32_t& value);
    void read_int32(uint8_t* data, int32_t& value);
    void read_uint32_big_endian(uint8_t* data, uint32_t& value);
    void read_uint32_little_endian(uint8_t* data, uint32_t& value);
    void read_int32_big_endian(uint8_t* data, int32_t& value);
    void read_int32_little_endian(uint8_t* data, int32_t& value);

private:
    midi_file_header* mMIDIHeader;
    uint32_t mMIDISegLength;
    uint32_t mValidMIDISegLength;
    bool mBigEndian;
    MIDIError mParseState;
};

}


#endif //TOOLKIT_VVMIDIPARSER_H
