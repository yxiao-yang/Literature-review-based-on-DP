#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <math.h>
#include <iterator>
#include <cctype>
 
using namespace std;
 
typedef struct Sentence
{
    string sentence;	//记录句子
    int num;			//记录句子单词个数
}Sentence;     
 
typedef struct sameSentence
{
    string libsent;     //记录对到最长公共子序列的lib的句子
	string textsent;    //记录test中对比的句子
    int textnum;		//记录test中对比的句子的单词的个数
	int samenum;	    //最大得分
}sameSentence;
 
sameSentence saveall[1000];   //存储所有对比好的信息
 
Sentence libSentence[1000];     //按句存储lib文件的内容
int wordSimilarity[1000][1000]; 	//动态规划矩阵
float Score[1000][1000];      //记录最长公共子序列的得分
string str1[1000],str2[1000];   //存储每个句子分割出来的单词
     
int libline = 0;     //lib文件的句子的个数
int testline = 0;    //test文件的句子的个数
 
void libtxtToSentence(string file);  //把lib里的内容按句子存储
void textToSentence(string file );   //读取text文本的内容并使用动态规划进行查重
float max(float a, float b);
float maxthree(float a, float b, float c);
 
 
int main()
{
	int j;
	double degree;
	double res1, res2;
	string libfile = "lib.txt";
	string textfile = "text.txt";
 
	libtxtToSentence(libfile);	//把lib里的内容按句子存储
 
	textToSentence(textfile);	//读取text文本的内容并使用动态规划进行查重
	
	cout << "测试文本共有" << testline-1 << "句话" << endl;
	for(j = 1; j < testline; j++)
	{
		cout<<"第"<<j<<"句对比"<<endl;
		cout<<saveall[j-1].textsent<<endl;
		cout<<saveall[j-1].libsent<<endl;
		cout<<"得分:  "<<saveall[j-1].samenum<<"\t"<<"单词总数:  "<<saveall[j-1].textnum<<endl;
		degree = double(saveall[j-1].samenum) / double(saveall[j-1].textnum);
		cout<<"该句与库相似度为"<<degree<<endl;
		cout<<"\n"<<endl;
		res2 += saveall[j-1].textnum;
		if(degree > 0.4)
			res1 = res1 + degree * saveall[j-1].textnum;
		degree = 0;
	}

    cout << "该文本与库中文本相似度为：" << res1 / res2 * 100 << "%" << endl;
    system("pause");
	return 0;
}
 
//把lib文本里的内容按句子存储
void libtxtToSentence( string file )
{
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
 
    char currentChar;	//当前字符
	string tempSentence="";    //临时字符串，用于存储句子
	Sentence temp;    //临时结构体，用于存放句子和其单词个数
	int num1 = 0;	//临时变量，用于存放单词个数
 
    infile >> noskipws;		//设置infile读取空白符
 
    while (!infile.eof())
    {
        infile >> currentChar;
		//如果字符是字母，加入到临时字符串中
		if(((currentChar >='a')&&(currentChar <='z'))||((currentChar >='A')&&(currentChar <='Z')))
			tempSentence += currentChar;
		//如果字符是标点符号，当前句结束，重置临时字符串
 		else if((currentChar =='.')||(currentChar =='?')||(currentChar =='!')||(currentChar == ','))
		{
			transform(tempSentence.begin(), tempSentence.end(), tempSentence.begin(), ::tolower);	//将临时字符串大写转小写
			temp.num = num1 + 1;	//临时结构体的句子长度+1
			temp.sentence = tempSentence;    //临时结构体的句子赋为临时字符串
			libSentence[libline] = temp;     //按每句子存储lib的内容到libSentence[]中
			num1 = 0;	//单词个数归0
			tempSentence = "";    //临时字符串重置
			libline++;      //lib文件的句子的个数
		}
		//匹配到空格
		else
		{
			//临时字符串为空白符，跳过
			if(tempSentence=="")
				continue;
			//匹配到空格，且当前句末不为空格，添加空格
			else if(tempSentence[tempSentence.size()==1?1:tempSentence.size()-1] != ' ') 
			{
				tempSentence += " ";	//添加空格
				num1 += 1;    //单词个数加1
			}
		}
    }
    infile.close();             //关闭文件输入流 
}
 
//读取text文本的内容并使用动态规划进行查重
void textToSentence( string file )
{
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
 
    char currentChar;	//当前字符
	string tempSentence="";    //临时字符串，用于存储句子
	Sentence temp;    //临时结构体，用于存放句子和其单词个数
	int num1 = 0;	//临时变量，用于存放单词个数
 
    infile >> noskipws;    //设置infile读取空白符
 
    while (!infile.eof())
    {
        infile >> currentChar;
		//如果字符是字母，加入到临时字符串中
		if(((currentChar >='a')&&(currentChar <='z'))||((currentChar >='A')&&(currentChar <='Z')))
			tempSentence += currentChar;
		//完成一个句子的读取，进行动态规划匹配查重
 		else if((currentChar =='.' )||(currentChar =='?')||(currentChar =='!'))
		{
			transform(tempSentence.begin(), tempSentence.end(), tempSentence.begin(), ::tolower);	//将临时字符串大写转小写
			temp.num = num1 + 1;            // 把当前读取到的test句子的单词个数存下
			temp.sentence = tempSentence;  // 把当前读取到的test句子存下 
			num1 = 0;	//单词个数归0
			tempSentence = "";   //临时字符串重置
			//分别与lib中的每一句进行对比
			for(int i = 0; i < libline; i++)
			{
				string t;
				int n = 0;
				int k,j;
				//存储把test的句子分割出来的单词
				for(istringstream is(temp.sentence); is >> t;)
					str1[n++] = t;

				n = 0;

				//存储把lib的句子分割出来的单词
				for(istringstream is1(libSentence[i].sentence); is1>>t;)
					str2[n++] = t;
				
				//构建单词是否相等矩阵
				for(k = 0; k < temp.num; k++)
				{
					//与lib文件的句子的单词分别进行对比
					for(j = 0; j < libSentence[i].num; j++)
					{
						//动态规划矩阵相等置1，不等置0
						if(str1[k] == str2[j])
							wordSimilarity[k][j] = 1;
						else
							wordSimilarity[k][j] = 0;
					}
				}
				
				//构建得分矩阵
				//初始化第1列
				for(k = 0; k < temp.num; k++)
					Score[k][0] = max(Score[k-1][0]-0.5, wordSimilarity[k][0]-0.5*(k-1));
				//初始化第1行
				for(k = 0; k < libSentence[i].num; k++)
					Score[0][k] = max(Score[0][k-1]-0.5, wordSimilarity[0][k]-0.5*(k-1));
				//动态规划
				for(k = 1; k < temp.num; k++)
					for(j = 1; j < libSentence[i].num; j++)
						Score[k][j] = maxthree(Score[k-1][j]-0.5, Score[k][j-1]-0.5, Score[k-1][j-1]+wordSimilarity[k][j]);

				int sum = Score[temp.num-1][libSentence[i].num-1];

				if(saveall[testline].samenum < sum)
				{
					saveall[testline].libsent = libSentence[i].sentence;
					saveall[testline].samenum = sum;
					saveall[testline].textnum = temp.num;
					saveall[testline].textsent = temp.sentence;
				}
			}
			testline++;   //test文件的句子的个数
 
		}
		//匹配到空格
		else
		{
			if( tempSentence=="" )
				continue;
			//匹配到空格，当前句末不为空格，添加空格
			else if(tempSentence[tempSentence.size()==1?1:tempSentence.size()-1] != ' ')
			{
				tempSentence += " ";
				num1 += 1;
			}
		}
    }
    infile.close();             //关闭文件输入流 
}
 
 
float max( float a, float b )
{
	if( a > b )
		return a;
	else
		return b;
} 
 
 
float maxthree( float a, float b, float c )
{
	float temp = a;
	if( temp < b )
		temp = b;
	if( temp < c )
		temp = c;
	return temp;
} 
 