/*
 * crc8.h
 *
 *  Created on: Jul 7, 2024
 *      Author: Mirko Serra
 */

#ifndef API_INC_CRC8_H_
#define API_INC_CRC8_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint8_t crc8(uint8_t crc, const void* mem, size_t len);

#endif /* API_INC_CRC8_H_ */
