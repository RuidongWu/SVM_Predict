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
	char featurepair[128];//���������������ַ���������
	int max_pos = 0, pos = 0;//����������ǰ���������ַ�����λ��
	int wpos = 0;//�������������˶��ٸ�wnum:weight
	float label=0;

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
	//���ȵ�һ������label���ݵ���ȡ
	sscanf(pLineBuffer, "%s ", featurepair);//����ط��пո�

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

	//˵��ֻ��Label��û�о������������
	if (pLineBuffer[pos] == 0)
	{
		return -3;
	}

	//Ȼ��ʼ�����Զ�ȡ���ݣ���wnum:weight
	wpos = 0;//���ڼ����ж��ٸ�����
	
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
