#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "VVMIDIParser.h"
#include "UtilLog.h"
#define MIDI_NAME   "../90323557.mid"
//#define MIDI_NAME   "../90501185x.mid"
//#define MIDI_NAME   "../90501185.mid"
//#define MIDI_NAME   "/home/disk/source/githup/myGitHup/LearnAndTest/cpp/test_struct/main.cpp"


// // #pragma pack(push)  //保存对齐状态
// // #pragma pack(4)     //设定为1字节对齐

// struct midi_segment
// {
//     // 开始时间，单位毫秒
//     int32_t begin;

//     // 持续时长，单位毫秒
//     int32_t length;

//     // midi note
//     uint32_t note;
// };

// struct midi_file_header
// {
//     // char cc;
//     uint32_t magic;
//     // char ccc;
//     uint32_t segment_length;

//     uint32_t crc;
//     // char cccc;
//     // int16_t xx;


//  //    char ccc;
//  //    char ccccc;
//  // char cccccc;
//  // char ccccccc;

//     midi_segment* data;
// };

// // #pragma pack(pop)//恢复对齐状态

int main(int argc, char** argv){
    using namespace vvmidi;
    char* c_midi_name = MIDI_NAME;
    if(argc > 1){
        c_midi_name = argv[1];
    }
    VVMIDIParser * parser = new VVMIDIParser();
    parser->parse(c_midi_name);

    midi_file_header* midiHeader = parser->getMIDIHeader();
    LOGI("magic %u", midiHeader->magic);
    LOGI("segment_length %u", midiHeader->segment_length);
    LOGI("crc %u", midiHeader->crc);

    for(int i = 0; i < parser->getMIDISegmentLength(); ++i){
       LOGI("id:%d, begin:%d,length:%d,note:%u", i, midiHeader->data[i].begin, midiHeader->data[i].length, midiHeader->data[i].note);
    }

    delete parser;

    LOGI("sizeof(midi_file_header) = %d", sizeof(midi_file_header));

    return 0;
}
