#include "common/defines.h"
#include "serport.h"

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>

static speed_t brateConverter(int br);

int serport_open(serport_ds *ds) {
	if (!ds) {
		fprintf(stderr, "[serport_open] Passed a NULL parameter!\n");	
		return -1;
	}

	// Open the serial port in RW 
	if ((ds->fd = open(ds->dev, O_RDWR /*| O_NONBLOCK*/)) < 0) {
		fprintf(stderr, "[serport_open] Unable to open serial port %s!\n", ds->dev);	
		return -1;
	}

#if 0
	if (flock(ds->fd, LOCK_EX|LOCK_NB) < 0) {
		logger_print(LOGR_BASE, "[serport_open] Unable to lock serial port %s!\n", ds->dev);	
		return -1;
	}
#endif

	tcgetattr(ds->fd, &(ds->old_attr)); // Backup the serial port attributes

	memset(&(ds->attr), 0, sizeof(ds->attr)); // Clean the struct where we're going to configure our attributes

	// Canonical mode
	ds->attr.c_iflag = IGNPAR; // INPUT FLAGS: IGNPAR makes the OS ignore bytes with parity errors
	ds->attr.c_oflag &= ~OPOST; // Enables "raw" output
	ds->attr.c_lflag &= ~(ICANON | ECHO | ISIG); // Enables "raw" mode for input 
	ds->attr.c_cc[VMIN] = 4;
	ds->attr.c_cc[VTIME] = 0xFF;

	ds->attr.c_cflag = CS8|CREAD|CLOCAL; // 8n1

	//cfmakeraw(&(ds->attr));
	cfsetospeed(&(ds->attr), brateConverter(ds->brate)); // Set the I/O baudrate
	cfsetispeed(&(ds->attr), brateConverter(ds->brate));

	tcsetattr(ds->fd, TCSANOW, &(ds->attr)); // Apply the attributes to the serial port

	return 1;
}

int serport_close(serport_ds *ds) {
	if (!ds) {
		fprintf(stderr, "[serport_close] Passed a NULL parameter!\n");	
		return -1;
	}

	tcsetattr(ds->fd, TCSANOW, &(ds->old_attr)); // Restore serial port attributes

#if 0
	// Release the lock
	if(flock(ds->fd, LOCK_UN|LOCK_NB) < 0) {
		logger_print(LOGR_BASE, "[serport_close] Unable to unlock the port!?\n");	
	}
#endif
	return close(ds->fd);
}


// We need a converter for baudrate:
// While termios definitions generally matches the correspondent numeric
// value, this might not always be the case, so we are playing it safe and 
// do an explicit match...
static speed_t brateConverter(int br) {
	switch(br) {
		case 50:
			return B50;
		case 75:
			return B75;
		case 110:
			return B110;
		case 134:
			return B134;
		case 150:
			return B150;
		case 200:
			return B200;
		case 300:
			return B300;
		case 600:
			return B600;
		case 1200:
			return B1200;
		case 1800:
			return B1800;
		case 2400:
			return B2400;
		case 4800:
			return B4800;
		case 9600:
			return B9600;
		case 19200:
			return B19200;
		case 38400:
			return B38400;
		case 57600:
			return B57600;
		case 115200:
			return B115200;
		case 230400:
		case 0:
		default:
			return B0; 
	}
}
