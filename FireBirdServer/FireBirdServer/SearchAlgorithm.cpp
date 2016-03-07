#include "StdAfx.h"
#include "SearchAlgorithm.h"


CSearchAlgorithm::CSearchAlgorithm(void)
{
}


CSearchAlgorithm::~CSearchAlgorithm(void)
{
}
/*********************************************************** 
** ��������: ������������С��;
** �������: 3������;
** ���ز���: ��Сֵ;
************************************************************/
int CSearchAlgorithm::minValue(int a,int b,int c)
{
	int min;
	min=(a>b)?b:a;
	if (min>c)
		min=c;
	return min;
}
/*********************************************************** 
** ��������: ���ƶ��㷨;
** �������: strA    ���ݿ��ļ���;
			 strB    �����ؼ���;
** ���ز���: ���ƶ�;
************************************************************/
float CSearchAlgorithm::calculateStringSemblance(CString strA, CString strB)    
{  
	int lenA = (int)strA.GetLength()+1;  
	int lenB = (int)strB.GetLength()+1;  
	int **c = new int*[lenA];  
	for(int i = 0; i < lenA; i++)  
		c[i] = new int[lenB];  
	for(int i = 0; i < lenA; i++) c[i][0] = i;  
	for(int j = 0; j < lenB; j++) c[0][j] = j;  
	c[0][0] = 0;  
	for(int i = 1; i < lenA; i++)  
	{  
		for(int j = 1; j < lenB; j++)  
		{  
			if(strB[j-1] == strA[i-1])  
				c[i][j] = c[i-1][j-1];  
			else  
				c[i][j] = minValue(c[i][j-1], c[i-1][j], c[i-1][j-1]) + 1;  
		}  
	}  
	int ret =  c[lenA-1][lenB-1];  
	for(int i = 0; i < lenA; i++)  
		delete [] c[i];  
	delete []c;  
	int maxLength=(lenA>lenB)?lenA:lenB;
	float semblance;
	semblance=1.0-(float)ret/(float)maxLength;
	return semblance;
}  
/*********************************************************** 
** ��������: LCS�㷨�������������
** �������: str1  ���ݿ��ļ���;str2  �����ؼ���;
** ���ز���: ����������еĳ���;
************************************************************/
int CSearchAlgorithm::lcs_l2(CString str1,CString str2)    
{
	int c[50][50]; 
	int m,n;
	m=str1.GetLength();
	n=str2.GetLength();
	for(int k=0;k<=m;k++) c[k][0]=0;
	for(int k=1;k<=n;k++) c[0][k]=0;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=n;j++)
		{
			if(str1[i-1]==str2[j-1])                 
			{
				c[i][j]=c[i-1][j-1]+1;
			}
			else if(c[i-1][j]>c[i][j-1])
			{
				c[i][j]=c[i-1][j];
			}
			else if(c[i-1][j]<c[i][j-1])
			{
				c[i][j]=c[i][j-1];
			}
			else if(c[i-1][j]==c[i][j-1])
			{
				c[i][j]=c[i][j-1];
			}
		}
	return c[m][n];
}
/*********************************************************** 
** ��������: ��������ʵ���Լ�������أ��������ƶ��㷨������
�����ƶ��㷨�㷨û�ҵ�������LCS�㷨��			
** �������: str1  ���ݿ��ļ���;str2  �����ؼ���;
** ���ز���: TRUE OR FALSE
************************************************************/
BOOL CSearchAlgorithm::matching(CString str1,CString str2)
{
	if (calculateStringSemblance(str1,str2)>0.449)
	{
		return TRUE;
	}
	if (lcs_l2(str1,str2)>=4)
	{
		return TRUE;
	}
	return FALSE;
}