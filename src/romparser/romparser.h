#ifndef _ROMPARSER_HEADER_
#define _ROMPARSER_HEADER_

#include "common/cmn_datatypes.h"

typedef enum {
	RAM_SIZE_2K = 0,
	RAM_SIZE_4K = 1,
	RAM_SIZE_8K = 3,
	RAM_SIZE_16K = 7,
	RAM_SIZE_32K = 15
} RAM_Size;

typedef enum {
	ROM_SIZE_256K = 0,
	ROM_SIZE_512K = 1,
	ROM_SIZE_1M = 3,
	ROM_SIZE_2M = 7,
	ROM_SIZE_4M = 15,
	ROM_SIZE_8M = 31
} ROM_Size;

#define ROM_NAME_SIZE 256

typedef struct {
	char rom_name[ROM_NAME_SIZE];
	uint8_t rom_options[5];

	uint8_t *buffer;
	uint32_t buffer_size;
} rom_buffer_structure;

int parseROMBuffer(rom_buffer_structure *rom_buf);

#endif /* _ROMPARSER_HEADER_ */
