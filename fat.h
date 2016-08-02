/*
 *  fat.h
 *  msdosdir
 *
 *  Created by Harry Anuszewski on 7/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef _FAT_H
#define _FAT_H

#include <stdio.h>
#include "types.h"


#define FIRST_FAT_SECTOR 1

typedef struct DirectoryEntryAttributes {
	unsigned int read_only : 1;
	unsigned int hidden : 1;
	unsigned int system : 1;
	unsigned int volume_label : 1;
	unsigned int subdirectory : 1;
	unsigned int archive : 1;
	unsigned int unused1 : 1;
	unsigned int unused2 : 1;
} DirectoryEntryAttributes;

typedef struct DirectoryEntry {
	unsigned char filename[8 + 1];
	unsigned char file_extension[3 + 1];
	DirectoryEntryAttributes attributes;
	BYTE reserved[10];
	UINT16 time;
	UINT16 date;
	UINT16 start_cluster;
	UINT32 file_size;
} DirectoryEntry;

typedef unsigned short sector_t; // Type for sector numbers
typedef unsigned short cluster_t;
UINT16 extract12(BYTE* arr, unsigned int index);
UINT16 extract12File(FILE* arr, unsigned int index);

int seekToSector(FILE* fp, sector_t sector, size_t sectorSize);

int readDirectoryEntry(DirectoryEntry* de, FILE* fp);

int readCluster(FILE* fp, cluster_t clusterNum, sector_t firstClusterSector, size_t sectorSize, size_t numSectorsPerCluster, char* buffer);

int seekToCluster(FILE* fp, cluster_t clusterNum, sector_t firstClusterSector, size_t sectorSize, size_t numSectorsPerCluster);
int isFile(const DirectoryEntry* de);
#endif