#include "supered.h"

#include <unistd.h>
#include <stdio.h>

int checkEverdrive_present(serport_ds *serport) {
	char buf = 0;

	if(write(serport->fd, "*t", 2) != 2) {
		fprintf(stderr, "checkEverdrive -> Unable to write!\n");
		return -1;
	}

	if(read(serport->fd, &buf, 1) != 1) {
		fprintf(stderr, "checkEverdrive -> Unable to read!\n");
		return -1;
	}

	if (buf == 'k') return 1;
	else return -1;
}

