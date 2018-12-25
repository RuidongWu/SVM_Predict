#include "Test_File.h"

FileProperty_TypeDef TestFile_Structure;
FileProperty_TypeDef *pTestFile;



int Parse_TestDocument(char *pLineBuffer, float *pResult, WORD_TypeDef *pWord)
{
	char featurepair[64];//用来保存特征对字符串的内容
	int max_pos = 0, pos = 0;
	int wpos = 0;//用来计数解析了多少个wnum:weight

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
	//首先第一步进行result内容的提取
	sscanf(pLineBuffer, "%s ", featurepair);//这个地方有空格

	pos = 0;
	while ((featurepair[pos] != ':') && (featurepair[pos]))
	{
		pos++;
	}
	if (featurepair[pos] == ':')
	{
		return -2;
	}

	sscanf(featurepair, "%f", pResult);

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

	//说明只有Result，没有具体的需要进行测试的数据
	if (pLineBuffer[pos] == 0)
		return -3;

	//然后开始周期性读取数据，即wnum:weight
	wpos = 0;//用于计数有多少个数据

	while (1)
	{
		sscanf(pLineBuffer + pos, "%s ", featurepair);
		if (sscanf(featurepair, "%d:%f", &index, &value) != 2)
			return -4;

		pWord[wpos].wnum = index;
		pWord[wpos].weight = value;
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


