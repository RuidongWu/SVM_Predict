#ifndef __MODEL_FILE_H
#define __MODEL_FILE_H
#include "stdio.h"
#include "File_Driver.h"

typedef struct
{
	char version[16];
	int kernel_type;
	int poly_degree;
	float rbf_gamma;
	float coef_lin;
	float coef_const;
	int total_words;
	int total_doc;
	int svm_num;
	float b;
	float *pModelTable;
}ModelPara_Typedef;

extern FileProperty_TypeDef ModelFile_Structure;
extern FileProperty_TypeDef *pModelFile;
extern ModelPara_Typedef ModelPara_Structure;

int Parse_ModelHeader(void);
int Parse_ModelFile(float *pModelTable);
int Parse_ModelDocument(char *pLineBuffer, float *pWord);


#endif
