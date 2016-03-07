#pragma once
class CSearchAlgorithm
{
public:
	float calculateStringSemblance(CString strA, CString strB);  //相似度算法;	
	BOOL matching(CString str1,CString str2);//负责搜索实现以及结果返回
	int lcs_l2(CString str1,CString str2);// LCS算法，最长公共子序列	  
	int minValue(int a,int b,int c);//求三个数的最小者;
	~CSearchAlgorithm(void);
	CSearchAlgorithm(void);
};

