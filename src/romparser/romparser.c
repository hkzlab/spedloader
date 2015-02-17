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
	uint32_t num = buf_len;
	uint8_t b = (num > 4194304) ? ROM_SIZE_8M : ((num > 2097152) ? ROM_SIZE_4M : ((num > 1048576) ? ROM_SIZE_2M : ((num > 524288) ? ROM_SIZE_1M : ((num > 262144) ? ROM_SIZE_512K : ROM_SIZE_256K))));
	uint8_t b2 = b;

	if (b2 <= 7) {
		switch (b2) {
			case 0:
				fprintf(stdout, "getROMMask ---> ROM size 256k\n");
				return b;
			case 1:
				fprintf(stdout, "getROMMask ---> ROM size 512k\n");
				return b;
			case 2:
				break;
			case 3:
				fprintf(stdout, "getROMMask ---> ROM size 1M\n");
				return b;
			default:
				if (b2 == 7) {
					fprintf(stdout, "getROMMask ---> ROM size 2M\n");
					return b;
				}
				break;
			}
	} else {
			if (b2 == 15) {
				fprintf(stdout, "getROMMask ---> ROM size 4M\n");
				return b;
			}
			if (b2 == 31) {
				fprintf(stdout, "getROMMask ---> ROM size 8M\n");
				return b;
			}
	}
		
	fprintf(stdout, "getROMMask ---> SRAM off\n");
	return b;
}

uint8_t getRAMMask(uint8_t *buf, uint32_t buf_len, uint8_t map_mode) {
	uint8_t b = (map_mode == 0) ? buf[32728] : buf[65496];
	uint8_t b2 = (b == 1) ? RAM_SIZE_2K : ((b == 2) ? RAM_SIZE_4K : ((b == 3) ? RAM_SIZE_8K : ((b == 4) ? RAM_SIZE_16K : RAM_SIZE_32K)));
			
	if (b == 0) {
		b2 = 0xFF;
	}
		
	uint8_t b3 = b2;
	switch (b3) {
		case 0:
			fprintf(stdout, "getRAMMask ---> RAM size 2K\n");
			return b2;
		case 1:
			fprintf(stdout, "getRAMMask ---> RAM size 4K\n");
			return b2;
		case 2:
			break;
		case 3:
			fprintf(stdout, "getRAMMask ---> RAM size 8K\n");
			return b2;
		default:
			if (b3 == 7) {
				fprintf(stdout, "getRAMMask ---> RAM size 16K\n");
				return b2;
			}
			if (b3 == 15) {
				fprintf(stdout, "getRAMMask ---> RAM size 32K\n");
				return b2;
			}
			break;
		}
		
	fprintf(stdout, "getRAMMask ---> RAM OFF\n");
	return b2;
}

uint8_t getMapType(uint8_t *buf, uint32_t buf_len) {
	uint32_t num = (uint32_t)buf[32732] | ((uint32_t)buf[32733] << 8);
	uint32_t num2 = (uint32_t)buf[32734] | ((uint32_t)buf[32735] << 8);
	
	if (num == (~num2 & 0xFFFF) && num != 0 && num != 0xFFFF) {
		fprintf(stdout, "getMapType ---> CRC OK\n");
		return 0;
	}

	num = ((int)buf[65500] | (int)buf[65501] << 8);
	num2 = ((int)buf[65502] | (int)buf[65503] << 8);

	if (num == (~num2 & 0xFFFF) && num != 0 && num != 0xFFFF) {
		fprintf(stdout, "getMapType ---> CRC OK\n");
		return 1;
	}

	if (buf_len <= 2097152) {
		return 0;
	}
	
	return 1;
}

