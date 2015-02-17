#include "romparser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t getROMMask(uint8_t *buf, uint32_t buf_len);
uint8_t getRAMMask(uint8_t *buf, uint32_t buf_len, uint8_t map_mode);
uint8_t getMapType(uint8_t *buf, uint32_t buf_len);

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

	rom_buf->rom_options[0] = getMapType(rom_data, num); // map mode
	rom_buf->rom_options[1] = getROMMask(rom_data, num); // rom mask
	rom_buf->rom_options[2] = getRAMMask(rom_data, num, rom_buf->rom_options[0]); // ram mask
	rom_buf->rom_options[3] = (rom_buf->rom_options[2] == 0xFF) ? 0 : 1;
	rom_buf->rom_options[2] = (rom_buf->rom_options[2] == 0xFF) ? RAM_SIZE_32K : rom_buf->rom_options[2];
	rom_buf->rom_options[4] = (rom_buf->rom_options[0] == 0x00) ? rom_data[32726] : rom_data[65494]; // dsp on
	rom_buf->rom_options[4] = ((rom_buf->rom_options[4] == 3) || (rom_buf->rom_options[4] == 5)) ? 1 : 0;

	free(rom_buf->buffer);
	rom_buf->buffer = rom_data;
	rom_buf->buffer_size = num;

	if(rom_buf->rom_options[0] == 0)
		fprintf(stdout, "parseROMBuffer ---> LOROM\n");
	else
		fprintf(stdout, "parseROMBuffer ---> HIROM\n");
	
	if(rom_buf->rom_options[4] == 0)
		fprintf(stdout, "parseROMBuffer ---> DSP Off\n");
	else
		fprintf(stdout, "parseROMBuffer ---> DSP On\n");

	return 1;
}


uint8_t getROMMask(uint8_t *buf, uint32_t buf_len) {
	return 0;
}

uint8_t getRAMMask(uint8_t *buf, uint32_t buf_len, uint8_t map_mode) {
	return 0;
}

uint8_t getMapType(uint8_t *buf, uint32_t buf_len) {
	return 0;
}

