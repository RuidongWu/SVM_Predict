#ifndef __TEST_FILE_H
#define __TEST_FILE_H

#include "stdio.h"
#include "File_Driver.h"

extern FileProperty_TypeDef TestFile_Structure;
extern FileProperty_TypeDef *pTestFile;

int Parse_TestDocument(char *pLineBuffer, float *pResult, WORD_TypeDef *pWord);

#endif
