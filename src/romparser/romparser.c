#include "romparser.h"

#include <stdlib.h>
#include <string.h>

int parseROMBuffer(rom_buffer_structure *rom_buf) {
	uint32_t num = rom_buf->buffer_size;
	uint32_t pos_idx = 0;
	uint8_t *rom_data;

	if ((num % 1024) == 512) {
		pos_idx = 512;
		num -= 512;
	}
	if ((num % 65536) != 0) {
		num = num / 65536 * 65536 + 65536;
	}

	rom_data = malloc(num);
	memcpy(rom_data, &(rom_buf->buffer[pos_idx]), num);

	return -1;
}


uint8_t getROMMask(uint8_t *buf, uint32_t buf_len) {
	return 0;
}

uint8_t getRAMMask(uint8_t *buf, uint32_t buf_len) {
	return 0;
}

uint8_t getMapType(uint8_t *buf, uint32_t buf_len) {
	return 0;
}

