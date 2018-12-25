#include "stdio.h"
#include "File_Driver.h"
#include "Model_File.h"
#include "Test_File.h"
#include "stdlib.h"
#include "string.h"


char *TestlineBuffer;
int main(void)
{
	int i = 0, j = 0;
	
	WORD_TypeDef *pTestWords;
	int test_status = 0;
	float result = 0;
	float test_sum = 0;
	int true_cnt = 0, false_cnt = 0;

	pModelFile = &ModelFile_Structure;
	pModelFile->FileName = "news20.model";
	f_scanFileProerty(pModelFile);

	Parse_ModelHeader();
	ModelPara_Structure.pModelTable = malloc((ModelPara_Structure.total_words+1)*sizeof(float));
	memset(ModelPara_Structure.pModelTable, 0, (ModelPara_Structure.total_words + 1)*sizeof(float));
	Parse_ModelFile(ModelPara_Structure.pModelTable);
	File_DriverRelease(pModelFile->pBuffer4K);

	
	pTestFile = &TestFile_Structure;
	pTestFile->FileName = "news20.t";
	f_scanFileProerty(pTestFile);
	pTestWords = malloc(pTestFile->SpacePerLine*sizeof(WORD_TypeDef));
	memset(pTestWords, 0, pTestFile->SpacePerLine*sizeof(WORD_TypeDef));
	TestlineBuffer = malloc(pTestFile->LongestLine*sizeof(char));
	memset(TestlineBuffer, 0, pTestFile->LongestLine*sizeof(char));
	for (i = 0; i < pTestFile->NumLines-1; i++)
	{
		f_getline(pTestFile->pBuffer4K, TestlineBuffer);
		test_status = Parse_TestDocument(TestlineBuffer, &result, pTestWords);
		if (test_status < 0)
		{
			switch (test_status)
			{
			case -1:
				printf("Line %d is comment!\n",i);
				break;
			case -2:
				printf("Line %d must start with label or zero!\n",i);
				break;
			case -3:
				printf("Line %d has no vectors!\n",i);
				break;
			case -4:
				printf("Line %d of Index:Value parses error!\n",i);
				break;
			default:
				printf("Line %d error code %d!\n", i, test_status);
				break;
			}
		}
		else if (test_status>0)
		{
			test_sum = 0;
			for (j = 0; j < test_status; j++)
			{
				test_sum += pTestWords[j].weight * ModelPara_Structure.pModelTable[pTestWords[j].wnum];
			}
			test_sum -= ModelPara_Structure.b;
			if (test_sum>0)
			{
				test_sum = 1;
			}
			else
			{
				test_sum = -1;
			}
			if (test_sum == result)
			{
				true_cnt++;
			}
			else
			{
				false_cnt++;
			}
		}
	}
	File_DriverRelease(pTestFile->pBuffer4K);
	printf("true:%d    false:%d\n", true_cnt, false_cnt);
	printf("Accuracy on test set: %.2f%%\n",(float)true_cnt*100/(float)(true_cnt+false_cnt));

	getch();
	return 0;
}
