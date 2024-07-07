/*
 * crc8.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Mirko Serra
 */

#include "crc8.h"

#include <assert.h>

uint8_t crc8(uint8_t crc, const void* mem, size_t len)
{
    assert(mem);
    const uint8_t* data = (const uint8_t*) mem;
    while (len--) {
        crc ^= *data++;
        for (unsigned k = 0; k < 8; k++) {
            crc = (crc & 0x80) ? ((crc << 1) ^ 0x31) : (crc << 1);
        }
    }
    return crc;
}
