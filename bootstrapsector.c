/*
 *  bootstrapsector.c
 *  msdosdir
 *
 *  Created by Harry Anuszewski on 7/27/10.
 *  Copyright 2010. All rights reserved.
 *
 */


#include "bootstrapsector.h"

UINT16 BIG_ENDIAN(UINT16* val)
{
	BYTE* bytes = (BYTE*)val;
	BYTE temp = bytes[0];
	bytes[0] = bytes[1];
	bytes[1] = temp;
	return (*val);
}

int readBootStrapSector(BootStrapSector* bss, FILE* fp)
{
	fread(&bss->firstInstruction, sizeof(BYTE), 3, fp);
	fread(&bss->OEM, sizeof(BYTE), 8, fp);
	fread(&bss->numBytesPerSector, sizeof(UINT16), 1, fp);
	fread(&bss->numSectorsPerCluster, sizeof(BYTE), 1, fp);
	fread(&bss->numReservedSectors, sizeof(UINT16), 1, fp);
	fread(&bss->numCopiesFAT, sizeof(BYTE), 1, fp);
	fread(&bss->numEntriesRootDir, sizeof(UINT16), 1, fp);
	fread(&bss->numSectors, sizeof(UINT16), 1, fp);
	fread(&bss->mediaDescriptor, sizeof(BYTE), 1, fp);
	fread(&bss->numSectorsInFAT, sizeof(UINT16), 1, fp);
	fread(&bss->numSectorsPerTrack, sizeof(UINT16), 1, fp);
	fread(&bss->numSides, sizeof(UINT16), 1, fp);
	fread(&bss->numHiddenSectors, sizeof(UINT16), 1, fp);
	fread(&bss->unknown, sizeof(BYTE), 9, fp);
	fread(&bss->volume_serial2, sizeof(UINT16), 1, fp);
	fread(&bss->volume_serial1, sizeof(UINT16), 1, fp);
	fread(&bss->formatType, sizeof(BYTE), 467, fp);
	fread(&bss->magic55AA, sizeof(BYTE), 2, fp);
	return 0;
}