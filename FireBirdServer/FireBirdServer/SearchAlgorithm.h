#pragma once
class CSearchAlgorithm
{
public:
	float calculateStringSemblance(CString strA, CString strB);  //���ƶ��㷨;	
	BOOL matching(CString str1,CString str2);//��������ʵ���Լ��������
	int lcs_l2(CString str1,CString str2);// LCS�㷨�������������	  
	int minValue(int a,int b,int c);//������������С��;
	~CSearchAlgorithm(void);
	CSearchAlgorithm(void);
};

