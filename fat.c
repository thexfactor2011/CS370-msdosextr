/*
 *  fat.c
 *  msdosdir
 *
 *  Created by Harry Anuszewski on 7/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "fat.h"
/**
 * Extracts the (index)th 12-bit value from byte array
 **/
UINT16 extract12(BYTE* arr, unsigned int index)
{
	UINT16 value = 0;
	BYTE* pVal = (BYTE*)&value;
	unsigned int i = (index * 3) / 2; // Index to first byte in arr array that contains the 12-bit entry
	if (index % 2 == 0) // Even
	{
		// arr[i] = [YZ],  arr[i+1] = [?X]
		// Must make value be [0X][YZ]
		pVal[1] = arr[i+1] & 0x0F; // Make [0X]
		pVal[0] = arr[i]; // Make [YZ]
	}
	else // Odd
	{
		// arr[i] = [Z?],  arr[i+1] = [XY]
		// Must make value be [0X][YZ]
		pVal[1] = arr[i+1] >> 4; // Make [0X]
		pVal[0] = (arr[i+1] << 4) | (arr[i] >> 4); // Make [YZ]
	}
	return value;
}

/**
 * Extracts the (index)th 12-bit value from a filestream starting at the current file
 * stream position. 
 * @return the stream back to its original position when its done
 **/
UINT16 extract12File(FILE* fp, unsigned int index)
{
	UINT16 value = 0;
	BYTE arr[2];
	long orig_pos = ftell(fp);
	BYTE* pVal = (BYTE*)&value;
	unsigned int i = (index * 3) / 2; // Index to first byte where the 12-bit entry is stored
	fseek(fp, i, SEEK_CUR); // Move up to where the 12-bit entry is.
	fread(arr, sizeof(BYTE), 2, fp);
	if (index % 2 == 0) // Even
	{
		// arr[i] = [YZ],  arr[i+1] = [?X]
		// Must make value be [0X][YZ]
		pVal[1] = arr[1] & 0x01; // Make [0X]
		pVal[0] = arr[0]; // Make [YZ]
	}
	else // Odd
	{
		// arr[i] = [Z?],  arr[i+1] = [XY]
		// Must make value be [0X][YZ]
		pVal[1] = arr[1] >> 4; // Make [0X]
		pVal[0] = (arr[1] << 4) | (arr[0] >> 4); // Make [YZ]
	}
	
	fseek(fp, orig_pos, SEEK_SET); // Move back to the begininning of the sector
	return value;
}

/**
 * Seeks file to the beginning of a given sector
 **/
int seekToSector(FILE* fp, sector_t sector, size_t sectorSize)
{
	return fseek(fp, sector * sectorSize, SEEK_SET);
}

/**
* @param fp will be seeked to the index of first bit of the cluster
* @param clusterNum first Cluster is 2, anything less than 2 will fail
* @param firstClusterSector first sector of a cluster
* @return -1 == fail or 0 == success
*/
int seekToCluster(FILE* fp, cluster_t clusterNum, sector_t firstClusterSector, size_t sectorSize, size_t numSectorsPerCluster)
{
	unsigned int offsetToFirstClusterSector = firstClusterSector * numSectorsPerCluster;
	unsigned int offsetToNthClusterSector = offsetToFirstClusterSector + (clusterNum - 2) * numSectorsPerCluster;
	return seekToSector(fp, offsetToNthClusterSector, sectorSize);
}

int readCluster(FILE* fp, cluster_t clusterNum, sector_t firstClusterSector, size_t sectorSize, size_t numSectorsPerCluster, char* buffer)
{
	seekToCluster(fp, clusterNum, firstClusterSector, sectorSize, numSectorsPerCluster);
	return fread(buffer, sizeof(BYTE), (sectorSize * numSectorsPerCluster), fp);
}

int readDirectoryEntry(DirectoryEntry* de, FILE* fp)
{
	fread(&de->filename, sizeof(BYTE), 8, fp); // 0-7
	de->filename[8] = 0; // Null terminator
	fread(&de->file_extension, sizeof(BYTE), 3, fp); // 8-10
	de->file_extension[3] = 0; // Null terminator
	fread(&de->attributes, sizeof(BYTE), 1, fp); // 11
	fread(&de->reserved, sizeof(BYTE), 10, fp); // 12-21
	fread(&de->time, sizeof(UINT16), 1, fp); // 22-23
	fread(&de->date, sizeof(UINT16), 1, fp); // 24-25
	fread(&de->start_cluster, sizeof(UINT16), 1, fp); // 26-27
	fread(&de->file_size, sizeof(UINT32), 1, fp); // 28-31
	return 0;
}

int isFile(const DirectoryEntry* de)
{
	if (de->filename[0] == 0 || de->filename[0] == 0xE5)
		return 0;
	if (de->attributes.volume_label == 1)
		return 0;
	if (de->attributes.subdirectory == 1)
		return 0;

	return 1;
}