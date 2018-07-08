#include "common_deser.h"


uint8_t deser_uint8(unsigned char** target_memory){
        union {
                uint8_t in_uchar;
                unsigned char in_bytes;
        } u8union;

        u8union.in_bytes= **target_memory;
        (*target_memory)++;

        return u8union.in_uchar;
}


uint16_t deser_uint16(unsigned char** target_memory){
        int read =0;
        union {
                uint16_t u16;
                unsigned char bytes[sizeof(uint16_t)];
        } u16union;

        for(int i=0; i<sizeof(uint16_t); i++) {
                u16union.bytes[i] = *(*target_memory+i);
        }
        read += sizeof(uint16_t);

        *target_memory += read;
        return u16union.u16;
}

std::list<uint16_t> deser_uint16_list(unsigned char** target_memory){
        std::list<uint16_t> ret;
        int s = deser_uint16(target_memory);


        for(int i=0; i<s; i++)
                ret.push_back(deser_uint16(target_memory));


        return ret;
}



uint32_t deser_uint32(unsigned char** target_memory){
        int read=0;

        union {
                uint32_t u32;
                unsigned char bytes[sizeof(uint32_t)];
        } u32union;

        for(int i=0; i<sizeof(uint32_t); i++) {
                u32union.bytes[i] = *(*target_memory+i);
        }
        read += sizeof(uint32_t);
        for(int i=0; i<sizeof(uint32_t); i++)

        *target_memory += read;
        return u32union.u32;
}

std::list<uint32_t> deser_small_uint32_list(unsigned char** target_memory){
        std::list<uint32_t> ret;
        int s = deser_uint8(target_memory);
        for(int i=0; i<s; i++)
                ret.push_back(deser_uint32(target_memory));

        return ret;
}
