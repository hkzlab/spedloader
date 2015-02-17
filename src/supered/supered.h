#ifndef _SUPERED_HEADER_
#define _SUPERED_HEADER_

#include "common/cmn_datatypes.h"
#include "serport/serport.h"
#include "romparser/romparser.h"

int checkEverdrive_present(serport_ds *serport);
int everdriveUploadROM(rom_buffer_structure *rom, serport_ds *serport);

#endif /* _SUPERED_HEADER_ */
