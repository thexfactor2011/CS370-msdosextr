/*
 *  main.c
 *  msdosdir
 *
 *  Created by Harry Anuszewski on 7/27/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bootstrapsector.h"
#include "fat.h"

#define BUFFER_SIZE 255

int main (int argc, const char * argv[]) {
	FILE* fp;// = fopen("samplefat.bin", "rb");

	if (argc == 2) {
		fp = fopen(argv[1], "rb");
		if(fp == NULL)
		{
			printf("Invalid file path.\n");
			return 0;
		}
	}
	else {
		printf("usage: %s %s\n", argv[0], "sample.bin");
		return 1;
	}
	
	BootStrapSector bss;
	readBootStrapSector(&bss, fp);

	if(bss.magic55AA[0] != 0x55 || bss.magic55AA[1] !=0xAA)
	{
		printf("Invalid FAT-12 file.\n");
		return 1;
	}

	sector_t rootDirectorySector = FIRST_FAT_SECTOR + bss.numSectorsInFAT * bss.numCopiesFAT;
	size_t numSectorsInDirectory = bss.numEntriesRootDir * 32 / bss.numBytesPerSector;
	sector_t firstClusterSector = rootDirectorySector + numSectorsInDirectory;
	seekToSector(fp, rootDirectorySector, bss.numBytesPerSector);
	
	// Extract
	int i;
	int fileCounter = 0;

	seekToSector(fp, rootDirectorySector, bss.numBytesPerSector);
	printf("Extracting...\n");	
	
	for (i = 0; i < bss.numEntriesRootDir; i++)
	{
		DirectoryEntry de;
		readDirectoryEntry(&de, fp);
		if (isFile(&de))
		{
			fileCounter++;
			long offsetToNextDirectoryEntry = ftell(fp);
			printf("%s.%s%s", de.filename, de.file_extension, (i < bss.numEntriesRootDir ? ", " : "\n"));
			FILE* fat_file_p = fopen(de.filename, "wb");
			UINT16 curCluster = de.start_cluster;
			//printf("%x\n", curCluster);
			BYTE buffer[bss.numBytesPerSector * bss.numSectorsPerCluster];
			int fileSize = de.file_size;
			while (curCluster < 0xFF8 && fileSize > 0)
			{
				readCluster(fp, curCluster, firstClusterSector, bss.numBytesPerSector, bss.numSectorsPerCluster, buffer);
				//}
				if(fileSize < bss.numBytesPerSector * bss.numSectorsPerCluster)
				{
					fwrite(buffer, sizeof(BYTE), fileSize, fat_file_p);
				}
				else
				{
					fwrite(buffer, sizeof(BYTE), sizeof(buffer), fat_file_p);
				}
				
				seekToSector(fp, FIRST_FAT_SECTOR, bss.numBytesPerSector);
				curCluster = extract12File(fp, curCluster);
				fileSize -= bss.numBytesPerSector * bss.numSectorsPerCluster;
			}
			fclose(fat_file_p);
			fseek(fp, offsetToNextDirectoryEntry, SEEK_SET);
		}
	}
	printf("\nExtracted: %i files\n", fileCounter);

	fclose(fp);

    return 0;
}
