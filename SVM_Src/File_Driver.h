#ifndef __FILE_DRIVER_H
#define __FILE_DRIVER_H

#include "stdio.h"

typedef struct
{
	char Buffer[2][4096];
	char *FileName;
	FILE *pFile;
	int switch_4K;
	
	int readsize[2];
	int position;
	int filesize;
}Buffer4K_TypeDef;

typedef struct
{
	char *FileName;
	long NumLines;
	long SpacePerLine;
	long LongestLine;
	long FileSize;
	Buffer4K_TypeDef *pBuffer4K;
	Buffer4K_TypeDef Buffer4K;
	
}FileProperty_TypeDef;

typedef struct word
{
	int wnum;
	float weight;
}WORD_TypeDef;


int File_DriverInit(void);
int File_DriverRelease(Buffer4K_TypeDef* Buffer4K);
int f_scanFileProerty(FileProperty_TypeDef *pFileProperty);

int f_getline(Buffer4K_TypeDef *Buffer4K, char *inBuffer);

#endif
