#include "Test_File.h"

FileProperty_TypeDef TestFile_Structure;
FileProperty_TypeDef *pTestFile;



int Parse_TestDocument(char *pLineBuffer, float *pResult, WORD_TypeDef *pWord)
{
	char featurepair[64];//���������������ַ���������
	int max_pos = 0, pos = 0;
	int wpos = 0;//�������������˶��ٸ�wnum:weight

	int index = 0;
	float value = 0;

	if (*pLineBuffer == '#')
		return -1;//������һ����ע��

	//�Ƴ�ÿһ������ע������
	while (pLineBuffer[pos])
	{
		if ((pLineBuffer[pos] == '#') || (pLineBuffer[pos] == 0x0a))
		{
			pLineBuffer[pos] = 0;
			max_pos = pos;
		}

		pos++;
	}

	//Ԥ��������Ժ�ʼ�����ݽ��н���
	//���ȵ�һ������result���ݵ���ȡ
	sscanf(pLineBuffer, "%s ", featurepair);//����ط��пո�

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

	//����ǰ��Ŀո�
	pos = 0;
	while ((pLineBuffer[pos] == ' ') && (pLineBuffer[pos] != 0))
		pos++;

	//������һ���ַ���
	while (pLineBuffer[pos] != ' ' && (pLineBuffer[pos] != 0))
		pos++;
	//��������Ŀո�
	while (pLineBuffer[pos] == ' ' && (pLineBuffer[pos] != 0))
		pos++;

	//˵��ֻ��Result��û�о������Ҫ���в��Ե�����
	if (pLineBuffer[pos] == 0)
		return -3;

	//Ȼ��ʼ�����Զ�ȡ���ݣ���wnum:weight
	wpos = 0;//���ڼ����ж��ٸ�����

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


