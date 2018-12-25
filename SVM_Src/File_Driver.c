#include "File_Driver.h"


int File_Read4K(Buffer4K_TypeDef* Buffer4K)
{
	Buffer4K->readsize[Buffer4K->switch_4K] = \
		fread(Buffer4K->Buffer[Buffer4K->switch_4K], sizeof(char), 4096, Buffer4K->pFile);
	return 0;
}

int File_DriverInit(Buffer4K_TypeDef* Buffer4K)
{
	Buffer4K->pFile = fopen(Buffer4K->FileName,"rb");
	if (Buffer4K->pFile != NULL)
	{
		Buffer4K->position = 0;
		Buffer4K->switch_4K = 0;
		Buffer4K->readsize[0] = 0;
		Buffer4K->readsize[1] = 0;

		//需要修正的地方
		fseek(Buffer4K->pFile, 0, SEEK_END);
		Buffer4K->filesize = ftell(Buffer4K->pFile);
		fseek(Buffer4K->pFile, 0, SEEK_SET);

		File_Read4K(Buffer4K);
		return 0;
	}
	else
		return 1;
}

int File_DriverRelease(Buffer4K_TypeDef* Buffer4K)
{
	if (Buffer4K->pFile != NULL)
		fclose(Buffer4K->pFile);
	return 0;
}


char f_getc(Buffer4K_TypeDef* Buffer4K)
{
	char temp;
	temp = Buffer4K->Buffer[Buffer4K->switch_4K][Buffer4K->position];
	
	Buffer4K->position++;
	if (Buffer4K->position == Buffer4K->readsize[Buffer4K->switch_4K])
	{
		Buffer4K->switch_4K = 1 - Buffer4K->switch_4K;
		Buffer4K->position = 0;
		File_Read4K(Buffer4K);
	}
	return temp;
}

int f_getline(Buffer4K_TypeDef *Buffer4K, char *inBuffer)
{
	int lineLength = 0;
	char temp;
	while (1)
	{
		temp = f_getc(Buffer4K);
		*inBuffer = temp;
		inBuffer++;
		lineLength++;
		if ((temp == 0x0A) || (lineLength == 1024000))
			break;
	}
	*inBuffer = 0;
	return lineLength;
}

int f_scanFileProerty(FileProperty_TypeDef *pFileProperty)
{
	int i = 0;
	char tempc;
	long current_length = 0, current_space = 0;
	long lines = 1;

	pFileProperty->Buffer4K.FileName = pFileProperty->FileName;
	pFileProperty->pBuffer4K = &(pFileProperty->Buffer4K);
	File_DriverInit(pFileProperty->pBuffer4K);
	pFileProperty->FileSize = pFileProperty->Buffer4K.filesize;

	while (i < pFileProperty->FileSize)
	{
		tempc = f_getc(&(pFileProperty->Buffer4K));
		current_length++;
		if ((tempc == 0) || (tempc == ' '))
			current_space++;
		else if (tempc == 0x0a)
		{
			lines++;
			if (current_length>pFileProperty->LongestLine)
			{
				pFileProperty->LongestLine = current_length;
			}
			if (current_space > pFileProperty->SpacePerLine)
			{
				pFileProperty->SpacePerLine = current_space;
			}
			current_space = 0;
			current_length = 0;
		}
		i++;
	}
	pFileProperty->NumLines = lines;

	File_DriverRelease(&(pFileProperty->Buffer4K));

	File_DriverInit(pFileProperty->pBuffer4K);

	return 0;
}



