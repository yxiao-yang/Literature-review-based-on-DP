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
    string sentence;	//��¼����
    int num;			//��¼���ӵ��ʸ���
}Sentence;     
 
typedef struct sameSentence
{
    string libsent;     //��¼�Ե�����������е�lib�ľ���
	string textsent;    //��¼test�жԱȵľ���
    int textnum;		//��¼test�жԱȵľ��ӵĵ��ʵĸ���
	int samenum;	    //���÷�
}sameSentence;
 
sameSentence saveall[1000];   //�洢���жԱȺõ���Ϣ
 
Sentence libSentence[1000];     //����洢lib�ļ�������
int wordSimilarity[1000][1000]; 	//��̬�滮����
float Score[1000][1000];      //��¼����������еĵ÷�
string str1[1000],str2[1000];   //�洢ÿ�����ӷָ�����ĵ���
     
int libline = 0;     //lib�ļ��ľ��ӵĸ���
int testline = 0;    //test�ļ��ľ��ӵĸ���
 
void libtxtToSentence(string file);  //��lib������ݰ����Ӵ洢
void textToSentence(string file );   //��ȡtext�ı������ݲ�ʹ�ö�̬�滮���в���
float max(float a, float b);
float maxthree(float a, float b, float c);
 
 
int main()
{
	int j;
	double degree;
	double res1, res2;
	string libfile = "lib.txt";
	string textfile = "text.txt";
 
	libtxtToSentence(libfile);	//��lib������ݰ����Ӵ洢
 
	textToSentence(textfile);	//��ȡtext�ı������ݲ�ʹ�ö�̬�滮���в���
	
	cout << "�����ı�����" << testline-1 << "�仰" << endl;
	for(j = 1; j < testline; j++)
	{
		cout<<"��"<<j<<"��Ա�"<<endl;
		cout<<saveall[j-1].textsent<<endl;
		cout<<saveall[j-1].libsent<<endl;
		cout<<"�÷�:  "<<saveall[j-1].samenum<<"\t"<<"��������:  "<<saveall[j-1].textnum<<endl;
		degree = double(saveall[j-1].samenum) / double(saveall[j-1].textnum);
		cout<<"�þ�������ƶ�Ϊ"<<degree<<endl;
		cout<<"\n"<<endl;
		res2 += saveall[j-1].textnum;
		if(degree > 0.4)
			res1 = res1 + degree * saveall[j-1].textnum;
		degree = 0;
	}

    cout << "���ı�������ı����ƶ�Ϊ��" << res1 / res2 * 100 << "%" << endl;
    system("pause");
	return 0;
}
 
//��lib�ı�������ݰ����Ӵ洢
void libtxtToSentence( string file )
{
    ifstream infile; 
    infile.open(file.data());   //���ļ����������ļ��������� 
    assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
 
    char currentChar;	//��ǰ�ַ�
	string tempSentence="";    //��ʱ�ַ��������ڴ洢����
	Sentence temp;    //��ʱ�ṹ�壬���ڴ�ž��Ӻ��䵥�ʸ���
	int num1 = 0;	//��ʱ���������ڴ�ŵ��ʸ���
 
    infile >> noskipws;		//����infile��ȡ�հ׷�
 
    while (!infile.eof())
    {
        infile >> currentChar;
		//����ַ�����ĸ�����뵽��ʱ�ַ�����
		if(((currentChar >='a')&&(currentChar <='z'))||((currentChar >='A')&&(currentChar <='Z')))
			tempSentence += currentChar;
		//����ַ��Ǳ����ţ���ǰ�������������ʱ�ַ���
 		else if((currentChar =='.')||(currentChar =='?')||(currentChar =='!')||(currentChar == ','))
		{
			transform(tempSentence.begin(), tempSentence.end(), tempSentence.begin(), ::tolower);	//����ʱ�ַ�����дתСд
			temp.num = num1 + 1;	//��ʱ�ṹ��ľ��ӳ���+1
			temp.sentence = tempSentence;    //��ʱ�ṹ��ľ��Ӹ�Ϊ��ʱ�ַ���
			libSentence[libline] = temp;     //��ÿ���Ӵ洢lib�����ݵ�libSentence[]��
			num1 = 0;	//���ʸ�����0
			tempSentence = "";    //��ʱ�ַ�������
			libline++;      //lib�ļ��ľ��ӵĸ���
		}
		//ƥ�䵽�ո�
		else
		{
			//��ʱ�ַ���Ϊ�հ׷�������
			if(tempSentence=="")
				continue;
			//ƥ�䵽�ո��ҵ�ǰ��ĩ��Ϊ�ո���ӿո�
			else if(tempSentence[tempSentence.size()==1?1:tempSentence.size()-1] != ' ') 
			{
				tempSentence += " ";	//��ӿո�
				num1 += 1;    //���ʸ�����1
			}
		}
    }
    infile.close();             //�ر��ļ������� 
}
 
//��ȡtext�ı������ݲ�ʹ�ö�̬�滮���в���
void textToSentence( string file )
{
    ifstream infile; 
    infile.open(file.data());   //���ļ����������ļ��������� 
    assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
 
    char currentChar;	//��ǰ�ַ�
	string tempSentence="";    //��ʱ�ַ��������ڴ洢����
	Sentence temp;    //��ʱ�ṹ�壬���ڴ�ž��Ӻ��䵥�ʸ���
	int num1 = 0;	//��ʱ���������ڴ�ŵ��ʸ���
 
    infile >> noskipws;    //����infile��ȡ�հ׷�
 
    while (!infile.eof())
    {
        infile >> currentChar;
		//����ַ�����ĸ�����뵽��ʱ�ַ�����
		if(((currentChar >='a')&&(currentChar <='z'))||((currentChar >='A')&&(currentChar <='Z')))
			tempSentence += currentChar;
		//���һ�����ӵĶ�ȡ�����ж�̬�滮ƥ�����
 		else if((currentChar =='.' )||(currentChar =='?')||(currentChar =='!'))
		{
			transform(tempSentence.begin(), tempSentence.end(), tempSentence.begin(), ::tolower);	//����ʱ�ַ�����дתСд
			temp.num = num1 + 1;            // �ѵ�ǰ��ȡ����test���ӵĵ��ʸ�������
			temp.sentence = tempSentence;  // �ѵ�ǰ��ȡ����test���Ӵ��� 
			num1 = 0;	//���ʸ�����0
			tempSentence = "";   //��ʱ�ַ�������
			//�ֱ���lib�е�ÿһ����жԱ�
			for(int i = 0; i < libline; i++)
			{
				string t;
				int n = 0;
				int k,j;
				//�洢��test�ľ��ӷָ�����ĵ���
				for(istringstream is(temp.sentence); is >> t;)
					str1[n++] = t;

				n = 0;

				//�洢��lib�ľ��ӷָ�����ĵ���
				for(istringstream is1(libSentence[i].sentence); is1>>t;)
					str2[n++] = t;
				
				//���������Ƿ���Ⱦ���
				for(k = 0; k < temp.num; k++)
				{
					//��lib�ļ��ľ��ӵĵ��ʷֱ���жԱ�
					for(j = 0; j < libSentence[i].num; j++)
					{
						//��̬�滮���������1��������0
						if(str1[k] == str2[j])
							wordSimilarity[k][j] = 1;
						else
							wordSimilarity[k][j] = 0;
					}
				}
				
				//�����÷־���
				//��ʼ����1��
				for(k = 0; k < temp.num; k++)
					Score[k][0] = max(Score[k-1][0]-0.5, wordSimilarity[k][0]-0.5*(k-1));
				//��ʼ����1��
				for(k = 0; k < libSentence[i].num; k++)
					Score[0][k] = max(Score[0][k-1]-0.5, wordSimilarity[0][k]-0.5*(k-1));
				//��̬�滮
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
			testline++;   //test�ļ��ľ��ӵĸ���
 
		}
		//ƥ�䵽�ո�
		else
		{
			if( tempSentence=="" )
				continue;
			//ƥ�䵽�ո񣬵�ǰ��ĩ��Ϊ�ո���ӿո�
			else if(tempSentence[tempSentence.size()==1?1:tempSentence.size()-1] != ' ')
			{
				tempSentence += " ";
				num1 += 1;
			}
		}
    }
    infile.close();             //�ر��ļ������� 
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
 