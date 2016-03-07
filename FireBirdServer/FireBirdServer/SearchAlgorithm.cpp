#include "StdAfx.h"
#include "SearchAlgorithm.h"


CSearchAlgorithm::CSearchAlgorithm(void)
{
}


CSearchAlgorithm::~CSearchAlgorithm(void)
{
}
/*********************************************************** 
** 功能描述: 求三个数的最小者;
** 输入参数: 3个整型;
** 返回参数: 最小值;
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
** 功能描述: 相似度算法;
** 输入参数: strA    数据库文件名;
			 strB    搜索关键词;
** 返回参数: 相似度;
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
** 功能描述: LCS算法，最长公共子序列
** 输入参数: str1  数据库文件名;str2  搜索关键词;
** 返回参数: 最长公共子序列的长度;
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
** 功能描述: 负责搜索实现以及结果返回，先用相似度算法搜索，
若相似度算法算法没找到，则用LCS算法。			
** 输入参数: str1  数据库文件名;str2  搜索关键词;
** 返回参数: TRUE OR FALSE
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