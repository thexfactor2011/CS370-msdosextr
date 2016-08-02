/*
 *  bootstrapsector.h
 *  msdosdir
 *
 *  Created by Harry Anuszewski on 7/27/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef _BOOTSTRAPSECTOR_H
#define _BOOTSTRAPSECTOR_H

#include <stdio.h>

#include "types.h"


typedef struct BootStrapSector
{
	BYTE firstInstruction[3]; //0
	BYTE OEM[8]; //3
	UINT16 numBytesPerSector; //11
	BYTE numSectorsPerCluster; //13
	UINT16 numReservedSectors;//14
	BYTE numCopiesFAT;//15
	UINT16 numEntriesRootDir;//16
	UINT16 numSectors;//19
	BYTE mediaDescriptor;//21
	UINT16 numSectorsInFAT;//22
	UINT16 numSectorsPerTrack;//24
	UINT16 numSides;//26
	UINT16 numHiddenSectors;//28
	BYTE unknown[9]; // 30
	UINT16 volume_serial2; //39
	UINT16 volume_serial1; //41
	BYTE formatType[467]; //43 // FAT12 or FAT16 in this program
	BYTE magic55AA[2]; // 510
} BootStrapSector;

//UINT16 BIG_ENDIAN(UINT16* val);

int readBootStrapSector(BootStrapSector* bss, FILE* fp);

#endif