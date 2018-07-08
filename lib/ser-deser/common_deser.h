#ifndef common_deser_h
#define common_deser_h

#include <list>
#include <Arduino.h>




uint8_t deser_uint8(unsigned char** target_memory);

uint16_t deser_uint16(unsigned char** target_memory);
std::list<uint16_t> deser_uint16_list(unsigned char** target_memory);
uint32_t deser_uint32(unsigned char** target_memory);
std::list<uint32_t> deser_small_uint32_list(unsigned char** target_memory);
#endif
