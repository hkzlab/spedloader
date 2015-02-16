#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <getopt.h>

#include "common/defines.h"
#include "common/cmn_datatypes.h"

#include "serport/serport.h"
#include "romparser/romparser.h"
#include "supered/supered.h"

#define FNAME_BUF_SIZE 256

static serport_ds *serial_device = NULL;
static char filename[FNAME_BUF_SIZE];
static rom_buffer_structure rom_buf;

static void exit_cleanup(); // Called at exit to provide final cleanup
static void print_usage(char *progname); // Small utility function to print the command help
static int readROMIntoBuffer(char *romname, rom_buffer_structure *rom_buf);

int main(int argc, char *argv[]) {
	int opt;
	uint8_t h_flag = 0, d_flag = 0, f_flag = 0; // Option flags

	rom_buf.buffer = NULL;

	// Allocate a buffer for serial structure
	serial_device = malloc(sizeof(serport_ds));
	if (!serial_device) {
		fprintf(stderr, "Failed to allocate memory for serial structure!\n");
		return EXIT_FAILURE;
	}
	
	// Parse the command line
	while ((opt = getopt(argc, argv, "d:f:h")) != -1) {
		switch(opt) {
			case 'd': // Parallel port to use
				d_flag = 1;
				strncpy(serial_device->dev, optarg, SER_DEVNAME_LEN - 1);
				break;
			case 'h': // Help requested
				h_flag = 1;
				break;
			case 'f': // Input file
				f_flag = 1;
				strncpy(filename, optarg, FNAME_BUF_SIZE - 1);
				break;
			case '?':
				if (optopt == 'd' || optopt == 'D' || optopt == 'f') {
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				} else if (isprint(optopt)) {
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				}
			default:
				return EXIT_FAILURE;
		}
	}

	// Manage the option flags
	if (h_flag) {
		print_usage(argv[0]);
		return EXIT_SUCCESS;
	}

	if (!d_flag || !f_flag) {
		fprintf(stderr, "Missing mandatory parameters!\n");
		print_usage(argv[0]);

		return EXIT_FAILURE;
	}

	// Register the exit handler
	atexit(exit_cleanup);

	if (readROMIntoBuffer(filename, &rom_buf) < 0) { // Read ROM into memory
		fprintf(stderr, "Failure to load ROM from file %s .\n", filename);
		return EXIT_FAILURE;
	}

	serial_device->brate = 38400; // Set baudrate
	if (serport_open(serial_device) < 0) { // Initialize the serial port module
		fprintf(stderr, "Failure to initialize the serial port device %s .\n", serial_device->dev);
		serial_device = NULL;
		return EXIT_FAILURE;
	}

	if (checkEverdrive_present(serial_device) < 0) {
		fprintf(stderr, "Super Everdrive did not answer\n");
		return EXIT_FAILURE;
	} else {
		fprintf(stdout, "Super Everdrive found!!!\n");
	}

	int ret_val = -1;

	if (ret_val < 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int readROMIntoBuffer(char *romname, rom_buffer_structure *rom_buf) {
	fprintf(stdout, "Trying to load %s\n", romname);

	int retval = 1;
	FILE *rom_f = fopen(romname, "rb");
	
	if (!rom_f) return -1; // Unable to open file

	fseek(rom_f, 0, SEEK_END);
	rom_buf->buffer_size = ftell(rom_f);
	fseek(rom_f, 0, SEEK_SET);

	fprintf(stdout, "File size %u\n", rom_buf->buffer_size);

	rom_buf->buffer = malloc(rom_buf->buffer_size);
	
	if (!(rom_buf->buffer)) {
		fclose(rom_f);
		return -1;
	}

	if(fread((rom_buf->buffer), 1, (rom_buf->buffer_size), rom_f) != (rom_buf->buffer_size)) {
		fprintf(stdout, "ROM Read failure.\n");
		retval = -1;
		free(rom_buf->buffer);
		rom_buf->buffer = NULL;
	}

	fclose(rom_f);

	return retval;
}

static void print_usage(char *progname) {
	fprintf(stdout, "Usage: %s -d SERIAL_DEVICE -f rom_filename [-h]\n"
			"\t-d SERIAL_DEVICE\tDefine the serial port to use.\n"
			"\t-f rom_filename\t\tROM filename.\n"
			"\t-h\t\t\tPrint this help\n", progname);
}

static void exit_cleanup() {
	// Close & Clean serial port
	if (serial_device) {
		serport_close(serial_device);
		free(serial_device);
	}

	if(rom_buf.buffer) free(rom_buf.buffer);

	return;
}



