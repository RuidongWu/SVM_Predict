#include "Model_File.h"


FileProperty_TypeDef ModelFile_Structure;
FileProperty_TypeDef *pModelFile;

ModelPara_Typedef ModelPara_Structure;

static char lineBuffer[1024000];
int Parse_ModelHeader(void)
{

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "SVM-light Version %s\n", ModelPara_Structure.version);
	printf("SVM version:%s\n", ModelPara_Structure.version);

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%d%*[^\n]\n", &(ModelPara_Structure.kernel_type));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%d%*[^\n]\n", &(ModelPara_Structure.poly_degree));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%f%*[^\n]\n", &(ModelPara_Structure.rbf_gamma));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%f%*[^\n]\n", &(ModelPara_Structure.coef_lin));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%f%*[^\n]\n", &(ModelPara_Structure.coef_const));

	f_getline(pModelFile->pBuffer4K, lineBuffer);

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%d%*[^\n]\n", &(ModelPara_Structure.total_words));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%d%*[^\n]\n", &(ModelPara_Structure.total_doc));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%d%*[^\n]\n", &(ModelPara_Structure.svm_num));

	f_getline(pModelFile->pBuffer4K, lineBuffer);
	sscanf(lineBuffer, "%f%*[^\n]\n", &(ModelPara_Structure.b));

	return 0;
}

int Parse_ModelFile(float *pModelTable)
{
	int i = 0;
	int parse_status = 0;
	for (i = 0; i < ModelPara_Structure.svm_num-1; i++)
	{
		f_getline(pModelFile->pBuffer4K, lineBuffer);
		parse_status = Parse_ModelDocument(lineBuffer, pModelTable);
		if (parse_status < 0)
			printf("svm %d parse error!\n", i);
		else if (parse_status>0)
		{
			//printf("svm %d parse %d parameters!\n", i, parse_status);
			printf(".");
		}
		
	}
	
	return 0;

}

int Parse_ModelDocument(char *pLineBuffer, float *pWord)
{
	char featurepair[128];//用来保存特征对字符串的内容
	int max_pos = 0, pos = 0;//用来表征当前解析的行字符串的位置
	int wpos = 0;//用来计数解析了多少个wnum:weight
	float label=0;

	int index = 0;
	float value = 0;

	if (*pLineBuffer == '#')
		return -1;//表明这一行是注释

	//移除每一行最后的注释内容
	while (pLineBuffer[pos])
	{
		if ((pLineBuffer[pos] == '#') || (pLineBuffer[pos] == 0x0a))
		{
			pLineBuffer[pos] = 0;
			max_pos = pos;
		}
			
		pos++;
	}

	//预处理完毕以后开始对数据进行解析
	//首先第一步进行label内容的提取
	sscanf(pLineBuffer, "%s ", featurepair);//这个地方有空格

	pos = 0;
	while ((featurepair[pos] != ':') && (featurepair[pos]))
	{
		pos++;
	}
	if (featurepair[pos] == ':')
	{
		printf("Lines must start with label or zero!\n");
		return -2;
	}

	sscanf(featurepair, "%f", &label);

	//消除前面的空格
	pos = 0;
	while ((pLineBuffer[pos] == ' ') && (pLineBuffer[pos] != 0))
		pos++;

	//消除第一个字符串
	while (pLineBuffer[pos] != ' ' && (pLineBuffer[pos] != 0))
		pos++;
	//消除后面的空格
	while (pLineBuffer[pos] == ' ' && (pLineBuffer[pos] != 0))
		pos++;

	//说明只有Label，没有具体的数据内容
	if (pLineBuffer[pos] == 0)
	{
		return -3;
	}

	//然后开始周期性读取数据，即wnum:weight
	wpos = 0;//用于计数有多少个数据
	
	while (1)
	{
		sscanf(pLineBuffer + pos, "%s ", featurepair);
		if (sscanf(featurepair, "%d:%f", &index, &value) != 2)
			return -4;

		
		pWord[index] += value*label;
		wpos++;

		while (pLineBuffer[pos] != ' ' && (pLineBuffer[pos] != 0))
			pos++;
		while (pLineBuffer[pos] == ' ' && (pLineBuffer[pos] != 0))
			pos++;

		if (pLineBuffer[pos] == 0)
			break;
	}
	
	return wpos;
}
