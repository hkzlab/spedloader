#ifndef _SERPORT_HEADER_
#define _SERPORT_HEADER_

#include <termios.h>

#define SER_DEVNAME_LEN 128

// Data structure used to access the serial port device
typedef struct {
	char dev[SER_DEVNAME_LEN];
	int brate;

	struct termios old_attr;
	struct termios attr;

	int fd;
} serport_ds;

// Open and setup a serial port
// You must pass a pointer to a serport_ds structure
// whose dev and brate fields are already correctly setup.
int serport_open(serport_ds *ds);

// Close and reset serial port status
int serport_close(serport_ds *ds);

#endif /* _SERPORT_HEADER_ */
