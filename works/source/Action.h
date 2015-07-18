#ifndef _ACTION_H_
#define _ACTION_H_
#include<iostream>
#include<vector>
#include<algorithm>
#include"Type.h"
using namespace std;

ActionEnum HoldCard_Action(HandleMsg *handlemsg);
ActionEnum FlopCard_Action(HandleMsg *handlemsg);
ActionEnum TurnCard_Action(HandleMsg *handlemsg);
ActionEnum ReverCard_Action(HandleMsg *handlemsg);

STATUS MyAction();

//����С˳�������ж�
//int King_Straight_Flust(vector<int> & number,vector<int> & coler);//�ʼ�ͬ��˳
//int Straight_Flust(vector<int> & number,vector<int> & coler); //ͬ��˳
int King_Straight_Flust(vector<int> & number,vector<int> & coler,HandleMsg *handlemsg);//�ʼ�ͬ��˳
int Straight_Flust(vector<int> & number,vector<int> & coler,HandleMsg *handlemsg); //ͬ��˳
int Four_OF_A_KIND(vector<int> & vec);//����
int Full_House(vector<int> & vec);//��«
int Flust(vector<int> & vec);    //ͬ��
int Straight(vector<int> & vec);  //˳��
int Three_OF_A_KIND(vector<int> & vec);//����
int Two_Pair(vector<int> & vec);//����
int One_Pair(vector<int> & vec);//һ��
int High_Card(vector<int> & vec);//����

ActionEnum  pre_Check(HandleMsg *handlemsg,double coef);
ActionEnum  pre_Raise(HandleMsg *handlemsg,double coef1,double coef2);
int ProStraight(vector<int> & vec);//Ǳ��˳��

#endif
