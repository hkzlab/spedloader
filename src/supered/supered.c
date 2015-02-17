#include "supered.h"

#include <unistd.h>
#include <stdio.h>

int internal_sendROMData(rom_buffer_structure *rom, serport_ds *serport);
int internal_sendROMInfo(rom_buffer_structure *rom, serport_ds *serport);

int checkEverdrive_present(serport_ds *serport) {
	char buf = 0, retval;

	retval = write(serport->fd, "*t", 2);
	if(read(serport->fd, &buf, 1) != 1) {
		fprintf(stderr, "checkEverdrive -> Unable to read!\n");
		return -1;
	}

	if (buf == 'k') return 1;
	else return -1;
}

int everdriveUploadROM(rom_buffer_structure *rom, serport_ds *serport) {
	if (internal_sendROMData(rom, serport) < 0) return -1;
	if (internal_sendROMInfo(rom, serport) < 0) return -1;
	write(serport->fd, "*s", 2);

	return 1;
}

int internal_sendROMData(rom_buffer_structure *rom, serport_ds *serport) {
	uint8_t array[5];
	char buf;

	for (uint32_t idx = 0; idx < rom->buffer_size; idx+=4) {
		array[0] = rom->buffer[idx++];
		array[1] = rom->buffer[idx++];
		array[2] = rom->buffer[idx++];
		array[3] = rom->buffer[idx++];
		rom->buffer[--idx] = array[0];
		rom->buffer[--idx] = array[1];
		rom->buffer[--idx] = array[2];
		rom->buffer[--idx] = array[3];
	}

	array[0] = array[1] = array[2] = 0;
	array[3] = (uint8_t)(rom->buffer_size / 65536 & 0xFF);

	write(serport->fd, "*e", 2);
	write(serport->fd, array, 4);
	write(serport->fd, "*t", 2);
	if(read(serport->fd, &buf, 1) != 1) {
		fprintf(stderr, "internal_sendROMData -> Unable to read!\n");
		return -1;
	}	

	uint32_t num = 32768;
	uint32_t num2 = rom->buffer_size;
	write(serport->fd, "*f", 2);

	array[0] = (uint8_t)(num2/num);
	write(serport->fd, array, 1);
	for (uint32_t j = 0; j < num2; j+=num) {
		array[0] = (uint8_t)(j/1048576);
		array[1] = (uint8_t)(j/4096);
		array[2] = (uint8_t)(num/4096);
		write(serport->fd, array, 3);
		write(serport->fd, &(rom->buffer[j]), num);
	
		if(read(serport->fd, &buf, 1) != 1) {
			fprintf(stderr, "checkEverdrive -> Unable to read!\n");
			return -1;
		}
	}

	return 1;
}

int internal_sendROMInfo(rom_buffer_structure *rom, serport_ds *serport) {
	char buf = 0, retval;

	retval = write(serport->fd, "*i", 2);
	retval = write(serport->fd, rom->rom_name, ROM_NAME_SIZE);
	retval = write(serport->fd, rom->rom_options, 5);
	retval = write(serport->fd, "*t", 2);
	retval = write(serport->fd, "*t", 2);

	if(read(serport->fd, &buf, 1) != 1) {
		fprintf(stderr, "sendROMInfo-> Unable to read!\n");
		return -1;
	}

	if (buf == 'k') return 1;
	else return -1;

}

