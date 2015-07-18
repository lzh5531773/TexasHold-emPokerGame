#include <stdio.h>
#include "Action.h"
#include "PokerRate.h"
#define ChangeActionNum 400
#define BigBlind 40
#define MaxDollar 40
#define MaxInning 600
#define EveryDollar 4000
#define ChangeRank  4     //�����ڴ��ڵ����������ʱ������
vector<int>MyColor;
vector<int>MyNumber;
vector<int>HandleColor;
vector<int>HandleNumber;
vector<int>HoldNumber;
vector<int>HoldColor;
int coul;

STATUS MyAction(){
	int state=pHandle.state;
	if(0 == state){
		MyColor.clear();MyNumber.clear();
		HandleColor.clear();HandleNumber.clear();
		HoldNumber.clear();HoldColor.clear();
		return TRUE;
	}
		
	if(0!=state && 1==pHandle.remainPlayerNum){
		pHandle.action = CHECK;
		return TRUE;
	}	
	switch(state){
		case 0:  MyColor.clear();MyNumber.clear();
			HandleColor.clear();HandleNumber.clear();
			HoldNumber.clear();HoldColor.clear();break;
		case 1:  pHandle.action = HoldCard_Action(&pHandle);
			break;
		case 2:  pHandle.action = FlopCard_Action(&pHandle);break;	
		case 3:  pHandle.action = TurnCard_Action(&pHandle);break;
		case 4:  pHandle.action = ReverCard_Action(&pHandle);break;
		default: pHandle.action = FOLD;break;
	}
	
	vector<int>::iterator it = MyNumber.begin();
	for(it = MyNumber.begin(); it != MyNumber.end(); it ++)
		fprintf(mylog,"%d ",*it);
	fprintf(mylog,"\n");
	
	return TRUE;
}


ActionEnum HoldCard_Action(HandleMsg *handlemsg){
	if(0 == handlemsg->count){
		MyColor.push_back(handlemsg->holdCard[0].color);
		MyColor.push_back(handlemsg->holdCard[1].color);
		MyNumber.push_back(handlemsg->holdCard[0].num);
		MyNumber.push_back(handlemsg->holdCard[1].num);
		sort(MyColor.begin(),MyColor.end());
		sort(MyNumber.begin(),MyNumber.end());
	}
	//����Ҵ��ںܶ��ʱ����ȫ��������ֱ��������500��
	int mytotal=handlemsg->otherPlayer [handlemsg->mySeat ].jetton +handlemsg->otherPlayer [handlemsg->mySeat ].money ;
	int throd= (MaxInning-handlemsg->inning)*BigBlind+(handlemsg->regPlayerNum)*EveryDollar/2;
	if( mytotal> throd )
		return FOLD;
	//400���Ժ��Ҵ������ƵĲ���
	if(pHandle.inning>=ChangeActionNum && pHandle.ranking>=ChangeRank){
			//�����ƶ�>J�Ÿ�,����ֻʣ���Լ�������ط�Ҳ����Ҫ�Ľ�����������AAʱ���Ƿ�Riase��
			if( MyNumber.back()>=13 || One_Pair(MyNumber)>=8 || (Straight(MyNumber)>=2 && MyNumber.back()>=10)  || (Flust(MyColor)>=2 && MyNumber.back()>=10))
					return CHECK;
			else if(handlemsg->remainPlayerNum==1)
					return CHECK;
			else
					return FOLD;
	}
	//�����Լ������������������Ĳ���
	else{
				//�����ƶ�>J�Ÿ�,����ֻʣ���Լ�������ط�Ҳ����Ҫ�Ľ�����������AAʱ���Ƿ�Riase��
			 if(pHandle.alivePlayerNum>4){
			   if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=8*BigBlind){
					return FOLD;
				}
			   if(MyNumber.front()<11){
						if(handlemsg->remainPlayerNum==1)
							return CHECK;
						else
							return FOLD;
					}
				else{
					return CHECK;
				}
			}
			else if(pHandle.alivePlayerNum>=3 && pHandle.alivePlayerNum<=4){
				if(MyNumber.front()>=13 || One_Pair(MyNumber)>=10 || (Straight(MyNumber)>=2 && MyNumber.back()>=10)  || (Flust(MyColor)>=2 && MyNumber.back()>=10) ){  //�����ƺܴ�ʱ���ͱ���ƴ����ʱ���ٿ���ƴһ��
						return CHECK;
				}
				if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind){
					return FOLD;
				}
				else if(MyNumber.front()>=8 && MyNumber.back()>=13)
					return CHECK;
				else if(handlemsg->remainPlayerNum==1)
					return CHECK;
				else
					return FOLD;
			}
			else if(pHandle.remainPlayerNum==2){
				if(MyNumber.front()>=13 || One_Pair(MyNumber)>=10){
					pHandle.raiseNum=pHandle.myPlayer.jetton/10;
					return RAISE;
				}
				if( MyNumber.back()>=12 || One_Pair(MyNumber) || Straight(MyNumber)>=2 || Flust(MyColor)>=2)
					return CHECK;
				else if(handlemsg->remainPlayerNum==1)
					return CHECK;
				else
					return FOLD;
			}
			else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
				return FOLD;
			}
}

ActionEnum FlopCard_Action(HandleMsg *handlemsg){
	
	if(0 == handlemsg->count){
		for(int i=0;i<3;i++){
		HandleColor.push_back(handlemsg->flopCard[i].color);
		HandleNumber.push_back(handlemsg->flopCard[i].num);
		MyColor.push_back(handlemsg->flopCard[i].color);
		MyNumber.push_back(handlemsg->flopCard[i].num);
		}
		sort(MyColor.begin(),MyColor.end());
		sort(MyNumber.begin(),MyNumber.end());
		sort(HandleNumber.begin(),HandleNumber.end());
		sort(HandleColor.begin(),HandleColor.end());
	}
	//����Ҵ��ںܶ��ʱ����ȫ��������ֱ��������500��
	int mytotal=handlemsg->otherPlayer [handlemsg->mySeat ].jetton +handlemsg->otherPlayer [handlemsg->mySeat ].money ;
	int throd= (MaxInning-handlemsg->inning)*BigBlind+(handlemsg->regPlayerNum)*EveryDollar/2;
	if( mytotal> throd )
		return FOLD;
		//400���Ժ��Ҵ������ƵĲ���
	if(pHandle.inning>=ChangeActionNum && pHandle.ranking>=ChangeRank){

			if(King_Straight_Flust(MyNumber,MyColor,handlemsg))//�ʼ�ͨ����
				return ALLIN;
			 else if(Straight_Flust(MyNumber,MyColor,handlemsg)){ //ͬ��˳
				if(Straight_Flust(HandleNumber,HandleColor,handlemsg)){
					if(MyNumber.back()==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/10;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}		
			else if(Four_OF_A_KIND(MyNumber)){    //����
				if(Four_OF_A_KIND(HandleNumber)){
					if(Four_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Full_House(MyNumber)){   //��«
				if(Full_House(HandleNumber)){
					if(Full_House(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/3;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)>=5){   //ͬ��
				if(Flust(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)>=5){   //˳��
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if(Straight(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
			else if(Three_OF_A_KIND(MyNumber)){  //3��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;
				if(Three_OF_A_KIND(HandleNumber)){
					if(Three_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=4*BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=6*BigBlind;;
						return RAISE; // 1/2*MaxDollar???
					}
	
			}
	
			else if(Two_Pair(MyNumber)){  //����
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;		
				if(Two_Pair(HandleNumber)){
					if(Two_Pair(MyNumber)>=12 && Two_Pair(HandleNumber)<Two_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=3*BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(One_Pair(MyNumber)){  //1��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind)
						return FOLD;
				
				if((pHandle.minCallNum>=6*BigBlind || pHandle.myPlayer.ebet>=26*BigBlind) && pHandle.alivePlayerNum>=4)
						return FOLD;		
				if(One_Pair(HandleNumber)){
					if(One_Pair(MyNumber)>=13 && One_Pair(HandleNumber)<One_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else if(One_Pair(HandleNumber)<One_Pair(MyNumber) || MyNumber.back()>=13)
						return CHECK;
					else
						return FOLD;
				}
				else{	pHandle.raiseNum=BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)==4){  //4ɫ
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=4){
					return FOLD;
				}
				if(Flust(HandleNumber)==4){
						return CHECK;
				}
				else{		pHandle.raiseNum=BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)==4){  //4˳
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=4){
					return FOLD;
				}
				if(Straight(HandleNumber)==4){
						return CHECK;
				}
				else{	
						pHandle.raiseNum=BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
			else if(MyNumber.back()>=11){
				if(pHandle.remainPlayerNum>4){
					return FOLD;
				}
				else if(pHandle.remainPlayerNum>=3 && pHandle.remainPlayerNum<=4){
					if(MyNumber.front()<11){
						if(handlemsg->remainPlayerNum==1)
							return CHECK;
						else
							return FOLD;
					}
					else{
						if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind){
							return FOLD;
						}
						else
							return CHECK;
					}
				}
				else if(pHandle.remainPlayerNum==2){
					if(MyNumber.back()>=13){
						if(pHandle.minCallNum>=5*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind)
							return FOLD;
						else
							return CHECK;
					}
					else if(handlemsg->remainPlayerNum==1)
						return CHECK;
					else
						return FOLD;
				}
				else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
					return FOLD ;	
			}
			else if(handlemsg->remainPlayerNum==1)
				return CHECK;
			else
				return FOLD;
	}
	//����������Լ����Ƚ����������
	else{
		   if(King_Straight_Flust(MyNumber,MyColor,handlemsg))//�ʼ�ͨ����
				return ALLIN;
			 else if(Straight_Flust(MyNumber,MyColor,handlemsg)){ //ͬ��˳
				if(Straight_Flust(HandleNumber,HandleColor,handlemsg)){
					if(MyNumber.back()==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/10;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}		
			else if(Four_OF_A_KIND(MyNumber)){    //����
				if(Four_OF_A_KIND(HandleNumber)){
					if(Four_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Full_House(MyNumber)){   //��«
				if(Full_House(HandleNumber)){
					if(Full_House(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/3;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)>=5){   //ͬ��
				if(Flust(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)>=5){   //˳��
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if(Straight(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
	else if(Three_OF_A_KIND(MyNumber)){  //3��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;
				if(Three_OF_A_KIND(HandleNumber)){
					if(Three_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=4*BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=6*BigBlind;;
						return RAISE; // 1/2*MaxDollar???
					}
	
			}
	
			else if(Two_Pair(MyNumber)){  //����
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;		
				if(Two_Pair(HandleNumber)){
					if(Two_Pair(MyNumber)>=12 && Two_Pair(HandleNumber)<Two_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=3*BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(One_Pair(MyNumber)){  //1��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind)
						return FOLD;
				if((pHandle.minCallNum>=6*BigBlind || pHandle.myPlayer.ebet>=26*BigBlind) && pHandle.alivePlayerNum>=4)
						return FOLD;
				if((One_Pair(HandleNumber)<One_Pair(MyNumber)) || MyNumber.back()>=13)
						return CHECK;
					else
						return FOLD;
			}
			else if(Flust(MyColor)==4){  //4ɫ
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=4){
					return FOLD;
				}
				if(Flust(HandleNumber)==4){
						return CHECK;
				}
				else{		pHandle.raiseNum=BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)==4){  //4˳
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=4){
					return FOLD;
				}
				if(Straight(HandleNumber)==4){
						return CHECK;
				}
				else{	
						pHandle.raiseNum=BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
			else if(MyNumber.back()>=11){
				if(pHandle.remainPlayerNum>4){
					return FOLD;
				}
				else if(pHandle.remainPlayerNum>=3 && pHandle.remainPlayerNum<=4){
					if(MyNumber.front()<11){
						if(handlemsg->remainPlayerNum==1)
							return CHECK;
						else
							return FOLD;
					}
					else{
						if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind){
							return FOLD;
						}
						else
							return CHECK;
					}
				}
				else if(pHandle.remainPlayerNum==2){
					if(MyNumber.back()>=13){
						if(pHandle.minCallNum>=5*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind)
							return FOLD;
						else
							return CHECK;
					}
					else if(handlemsg->remainPlayerNum==1)
						return CHECK;
					else
						return FOLD;
				}
				else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
					return FOLD ;	
			}
			else if(handlemsg->remainPlayerNum==1)
				return CHECK;
			else
				return FOLD;
	}
}

ActionEnum TurnCard_Action(HandleMsg *handlemsg){
	if(0 == handlemsg->count){
		HandleColor.push_back(handlemsg->turnCard.color);
		HandleNumber.push_back(handlemsg->turnCard.num);
		MyColor.push_back(handlemsg->turnCard.color);
		MyNumber.push_back(handlemsg->turnCard.num);
		sort(MyColor.begin(),MyColor.end());
		sort(MyNumber.begin(),MyNumber.end());
		sort(HandleNumber.begin(),HandleNumber.end());
		sort(HandleColor.begin(),HandleColor.end());
	}
		//����Ҵ��ںܶ��ʱ����ȫ��������ֱ��������500��
	int mytotal=handlemsg->otherPlayer [handlemsg->mySeat ].jetton +handlemsg->otherPlayer [handlemsg->mySeat ].money ;
	int throd= (MaxInning-handlemsg->inning)*BigBlind+(handlemsg->regPlayerNum)*EveryDollar/2;
	if( mytotal> throd )
		return FOLD;
		//400���Ժ��Ҵ������ƵĲ���
	if(pHandle.inning>=ChangeActionNum && pHandle.ranking>=ChangeRank){

			if(King_Straight_Flust(MyNumber,MyColor,handlemsg))//�ʼ�ͨ����
				return ALLIN;
			 else if(Straight_Flust(MyNumber,MyColor,handlemsg)){ //ͬ��˳
				if(Straight_Flust(HandleNumber,HandleColor,handlemsg)){
					if(MyNumber.back()==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/10;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}		
			else if(Four_OF_A_KIND(MyNumber)){    //����
				if(Four_OF_A_KIND(HandleNumber)){
					if(Four_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Full_House(MyNumber)){   //��«
				if(Full_House(HandleNumber)){
					if(Full_House(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/3;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)>=5){   //ͬ��
				if(Flust(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)>=5){   //˳��
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if(Straight(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
	else if(Three_OF_A_KIND(MyNumber)){  //3��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;
				if(Three_OF_A_KIND(HandleNumber)){
					if(Three_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=4*BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=6*BigBlind;;
						return RAISE; // 1/2*MaxDollar???
					}
	
			}
	
			else if(Two_Pair(MyNumber)){  //����
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;		
				if(Two_Pair(HandleNumber)){
					if(Two_Pair(MyNumber)>=12 && Two_Pair(HandleNumber)<Two_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=3*BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(One_Pair(MyNumber)){  //1��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind)
						return FOLD;
				if((pHandle.minCallNum>=6*BigBlind || pHandle.myPlayer.ebet>=26*BigBlind) && pHandle.alivePlayerNum>=4)
						return FOLD;
				if((One_Pair(HandleNumber)<One_Pair(MyNumber)) || MyNumber.back()>=13)
						return CHECK;
					else
						return FOLD;
			}
			else if(Flust(MyColor)==4){  //4ɫ
				if(Flust(HandleColor)==4 && MyNumber.back()<=12)
					return FOLD;
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=3){
					return FOLD;
				}
				else
					return CHECK;
			}
			else if(Straight(MyNumber)==4){  //4˳
				if(Flust(HandleNumber)>=4)
					return FOLD;
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=3){
					return FOLD;
				}
				return CHECK;				
			}
			else if(MyNumber.back()>=11){
				if(pHandle.remainPlayerNum>2){
					return FOLD;
				}
				else if(pHandle.remainPlayerNum==2){
					if(MyNumber.back()>=13){
						if(pHandle.minCallNum>=5*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind)
							return FOLD;
						else
							return CHECK;
					}
					else if(handlemsg->remainPlayerNum==1)
						return CHECK;
					else
						return FOLD;
				}
				else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
					return FOLD ;	
			}
			else if(handlemsg->remainPlayerNum==1)
				return CHECK;
			else
				return FOLD;
	}
	//����������Լ����Ƚ����������
	else{
				if(King_Straight_Flust(MyNumber,MyColor,handlemsg))//�ʼ�ͨ����
				return ALLIN;
			 else if(Straight_Flust(MyNumber,MyColor,handlemsg)){ //ͬ��˳
				if(Straight_Flust(HandleNumber,HandleColor,handlemsg)){
					if(MyNumber.back()==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/10;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}		
			else if(Four_OF_A_KIND(MyNumber)){    //����
				if(Four_OF_A_KIND(HandleNumber)){
					if(Four_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Full_House(MyNumber)){   //��«
				if(Full_House(HandleNumber)){
					if(Full_House(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/3;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)>=5){   //ͬ��
				if(Flust(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)>=5){   //˳��
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if(Straight(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
	else if(Three_OF_A_KIND(MyNumber)){  //3��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;
				if(Three_OF_A_KIND(HandleNumber)){
					if(Three_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=4*BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=6*BigBlind;;
						return RAISE; // 1/2*MaxDollar???
					}
	
			}
	
			else if(Two_Pair(MyNumber)){  //����
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;		
				if(Two_Pair(HandleNumber)){
					if(Two_Pair(MyNumber)>=12 && Two_Pair(HandleNumber)<Two_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=3*BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(One_Pair(MyNumber)){  //1��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind)
						return FOLD;
				if((pHandle.minCallNum>=6*BigBlind || pHandle.myPlayer.ebet>=26*BigBlind) && pHandle.alivePlayerNum>=4)
						return FOLD;
				if((One_Pair(HandleNumber)<One_Pair(MyNumber)) || MyNumber.back()>=13)
						return CHECK;
					else
						return FOLD;
			}
			else if(Flust(MyColor)==4){  //4ɫ
				if(Flust(HandleColor)==4 && MyNumber.back()<=12)
					return FOLD;
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=3){
					return FOLD;
				}
				else
					return CHECK;
			}
			else if(Straight(MyNumber)==4){  //4˳
				if(Flust(HandleNumber)>=4)
					return FOLD;
				if((pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind) && pHandle.alivePlayerNum>=3){
					return FOLD;
				}
				return CHECK;				
			}
			else if(MyNumber.back()>=11){
				if(pHandle.remainPlayerNum>2){
					return FOLD;
				}
				else if(pHandle.remainPlayerNum==2){
					if(MyNumber.back()>=13){
						if(pHandle.minCallNum>=5*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind)
							return FOLD;
						else
							return CHECK;
					}
					else if(handlemsg->remainPlayerNum==1)
						return CHECK;
					else
						return FOLD;
				}
				else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
					return FOLD ;	
			}
			else if(handlemsg->remainPlayerNum==1)
				return CHECK;
			else
				return FOLD;
	}
}

ActionEnum ReverCard_Action(HandleMsg *handlemsg){
	if(0 == handlemsg->count){
		HandleColor.push_back(handlemsg->riverCard.color);
		HandleNumber.push_back(handlemsg->riverCard.num);
		MyColor.push_back(handlemsg->riverCard.color);
		MyNumber.push_back(handlemsg->riverCard.num);
		sort(MyColor.begin(),MyColor.end());
		sort(MyNumber.begin(),MyNumber.end());
		sort(HandleNumber.begin(),HandleNumber.end());
		sort(HandleColor.begin(),HandleColor.end());
	}
		//����Ҵ��ںܶ��ʱ����ȫ��������ֱ��������500��
	int mytotal=handlemsg->otherPlayer [handlemsg->mySeat ].jetton +handlemsg->otherPlayer [handlemsg->mySeat ].money ;
	int throd= (MaxInning-handlemsg->inning)*BigBlind+(handlemsg->regPlayerNum)*EveryDollar/2;
	if( mytotal> throd )
		return FOLD;
		//400���Ժ��Ҵ������ƵĲ���
	if(pHandle.inning>=ChangeActionNum && pHandle.ranking>=ChangeRank){

			if(King_Straight_Flust(MyNumber,MyColor,handlemsg))//�ʼ�ͨ����
				return ALLIN;
			 else if(Straight_Flust(MyNumber,MyColor,handlemsg)){ //ͬ��˳
				if(Straight_Flust(HandleNumber,HandleColor,handlemsg)){
					if(MyNumber.back()==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/10;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}		
			else if(Four_OF_A_KIND(MyNumber)){    //����
				if(Four_OF_A_KIND(HandleNumber)){
					if(Four_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Full_House(MyNumber)){   //��«
				if(Full_House(HandleNumber)){
					if(Full_House(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/3;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)>=5){   //ͬ��
				if(Flust(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)>=5){   //˳��
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if(Straight(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
	else if(Three_OF_A_KIND(MyNumber)){  //3��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;
				if(Three_OF_A_KIND(HandleNumber)){
					if(Three_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=4*BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=6*BigBlind;;
						return RAISE; // 1/2*MaxDollar???
					}
	
			}
	
			else if(Two_Pair(MyNumber)){  //����
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;		
				if(Two_Pair(HandleNumber)){
					if(Two_Pair(MyNumber)>=12 && Two_Pair(HandleNumber)<Two_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=3*BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(One_Pair(MyNumber)){  //1��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind)
						return FOLD;
				if((pHandle.minCallNum>=6*BigBlind || pHandle.myPlayer.ebet>=26*BigBlind) && pHandle.alivePlayerNum>=4)
						return FOLD;
				if((One_Pair(HandleNumber)<One_Pair(MyNumber)) || MyNumber.back()>=13)
						return CHECK;
					else
						return FOLD;
			}
			else if(MyNumber.back()>=11){
				if(pHandle.remainPlayerNum>2){
					return FOLD;
				}
				else if(pHandle.remainPlayerNum==2){
					if(MyNumber.back()>=13){
						if(pHandle.minCallNum>=5*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind)
							return FOLD;
						else
							return CHECK;
					}
					else if(handlemsg->remainPlayerNum==1)
						return CHECK;
					else
						return FOLD;
				}
				else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
					return FOLD ;	
			}
			else if(handlemsg->remainPlayerNum==1)
				return CHECK;
			else
				return FOLD;
	}
	//����������Լ����Ƚ����������
	else{
			if(King_Straight_Flust(MyNumber,MyColor,handlemsg))//�ʼ�ͨ����
				return ALLIN;
			 else if(Straight_Flust(MyNumber,MyColor,handlemsg)){ //ͬ��˳
				if(Straight_Flust(HandleNumber,HandleColor,handlemsg)){
					if(MyNumber.back()==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/10;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}		
			else if(Four_OF_A_KIND(MyNumber)){    //����
				if(Four_OF_A_KIND(HandleNumber)){
					if(Four_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/2;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Full_House(MyNumber)){   //��«
				if(Full_House(HandleNumber)){
					if(Full_House(MyNumber)==14){
						pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/3;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Flust(MyColor)>=5){   //ͬ��
				if(Flust(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/4;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(Straight(MyNumber)>=5){   //˳��
				if(Flust(HandleNumber)>=5)
					return FOLD;
				if(Straight(HandleNumber)>=5){
						return CHECK;
				}
				else{	pHandle.raiseNum=pHandle.myPlayer.jetton/5;
						return RAISE; // 1/2*MaxDollar???
					}
				
			}
	else if(Three_OF_A_KIND(MyNumber)){  //3��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;
				if(Three_OF_A_KIND(HandleNumber)){
					if(Three_OF_A_KIND(MyNumber)==14){
						pHandle.raiseNum=4*BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=6*BigBlind;;
						return RAISE; // 1/2*MaxDollar???
					}
	
			}
	
			else if(Two_Pair(MyNumber)){  //����
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=8*BigBlind || pHandle.myPlayer.ebet>=16*BigBlind)
						return FOLD;		
				if(Two_Pair(HandleNumber)){
					if(Two_Pair(MyNumber)>=12 && Two_Pair(HandleNumber)<Two_Pair(MyNumber)){
						pHandle.raiseNum=BigBlind;
						return RAISE;
					}
					else
						return CHECK;
				}
				else{	pHandle.raiseNum=3*BigBlind;
						return RAISE; // 1/2*MaxDollar???
					}
			}
			else if(One_Pair(MyNumber)){  //1��
				if(Straight(HandleNumber)>=5 || Flust(HandleNumber)>=5 )
					return FOLD;
				if(Straight(HandleNumber)>=4 || Flust(HandleNumber)>=4 )
					if(pHandle.minCallNum>=4*BigBlind || pHandle.myPlayer.ebet>=10*BigBlind)
						return FOLD;
				if((pHandle.minCallNum>=6*BigBlind || pHandle.myPlayer.ebet>=26*BigBlind) && pHandle.alivePlayerNum>=4)
						return FOLD;
				if((One_Pair(HandleNumber)<One_Pair(MyNumber)) || MyNumber.back()>=13)
						return CHECK;
					else
						return FOLD;
			}
			else if(MyNumber.back()>=11){
				if(pHandle.remainPlayerNum>2){
					return FOLD;
				}
				else if(pHandle.remainPlayerNum==2){
					if(MyNumber.back()>=13){
						if(pHandle.minCallNum>=5*BigBlind || pHandle.myPlayer.ebet>=12*BigBlind)
							return FOLD;
						else
							return CHECK;
					}
					else if(handlemsg->remainPlayerNum==1)
						return CHECK;
					else
						return FOLD;
				}
				else //�п���ֻ���Լ�����������߳�bug�������Ҫ����ǰ�淵��ֵ��©����
					return FOLD ;	
			}
			else if(handlemsg->remainPlayerNum==1)
				return CHECK;
			else
				return FOLD;
	}
}




int High_Card(vector<int> &vec){
//��������ֵΪvector�����ֵ
	return vec.back();
}
int One_Pair(vector<int> & vec){
//��������vector���ж��ӵ�һ����,���������򷵻�0
	int temp=0;
	vector<int>::iterator it;
	for(it=vec.begin();it!=vec.end();it++){
		if(temp==*it)
			return temp;
		temp=*it;
	}
	return 0;
}
int Two_Pair(vector<int> & vec){
//�������������нϴ��һ�Ե�ֵ���������������򷵻�0
	int temp=0;
	int twocnt=0;
	vector<int>::iterator it;
	for(it=vec.begin();it!=vec.end();it++){
		if(temp==*it){
			if(twocnt)
				return temp;
			else{
				twocnt++;
				temp=0;
				continue;
			}
		}
		temp=*it;
	}
	return 0;
}

int Three_OF_A_KIND(vector<int> & vec){
//��������������ֵ��û�з���0
	int temp=0;
	int cnt=0;
	vector<int>::iterator it;
	for(it=vec.begin();it!=vec.end();it++){
		if(temp==*it){
			if(cnt)
				return temp;
			else
				cnt++;
		}
		else
			cnt=0;
		temp=*it;
	}
	return 0;

}

int Four_OF_A_KIND(vector<int> & vec){
//��������������ֵ��û�з���0
	int temp=0;
	int cnt=0;
	vector<int>::iterator it;
	for(it=vec.begin();it!=vec.end();it++){
		if(temp==*it){
			if(cnt==2)
				return temp;
			else
				cnt++;
		}
		else
			cnt=0;
		temp=*it;		
	}
	return 0;
}
int Straight(vector<int> & vec){
//����˳����˳��ĸ�������û���򷵻�0��(û�п���4���п������˳�ӵ����)
	vector<int>::iterator it;
	vector<int> temp_vec;
	int cnt=1;
	int temp=0;
	//ȥ���ظ���
	for(it=vec.begin();it!=vec.end();it++){
		if(temp==*it)
			continue;
	    temp_vec.push_back(*it);
		temp=*it;
	}
	if(temp_vec.size()>=4){
		if(temp_vec.size()>=5)
			if(temp_vec.back()==14 && vec[0]==2 && vec[1]==3 && vec[2]==4 && vec[3]==5) //��A2345Ϊ˳�ӵ����,������������ʱԽ������
				return 5;
	}
	else
		return 1;

	temp=temp_vec.back();
	for(it=temp_vec.end()-2;it>=temp_vec.begin();it--){
		if(temp==(*it)+1){
			cnt++;
		}
		else
			cnt=1;
		temp=*it;		
		if(it==temp_vec.begin())
			break;
	}
	return cnt;
}

int Flust(vector<int> & vec){
//������������ͬ���Ļ�ɫ������û�з���ͬ��������һ���Ƶ������д��Ľ�������û���򷵻�0��(û�п���4���п������ͬ�������)
	int temp=-1;
	int cnt=1;
	vector<int>::iterator it;
	for(it=vec.begin();it!=vec.end();it++){
		if(temp==(*it)){
				cnt++;
		}
		else
			cnt=1;
		temp=*it;		
	}
	return cnt;
}    

int Full_House(vector<int> & vec){
//�������غ�«��������ֵ����û���򷵻�0
	int three=Three_OF_A_KIND(vec);
	int twopair=Two_Pair(vec);
	if(three && twopair)
		return three;
	else
		return 0;
}

int Straight_Flust(vector<int> & number,vector<int> & coler,HandleMsg *handlemsg)
{  
//��������ͬ��˳�������ƣ���û���򷵻�0 
	vector<int>MyBufferNumber;
	int flust=Flust(coler);
	int straight,i;
	if(flust>=5)//ͬ��
	{
		if(coler.size() ==5)
			 straight=Straight(number);
		else  if(coler.size() ==6) 
		{
			for( i=0;i<=1;i++)
				if(handlemsg->holdCard[i].color ==coul)
					MyBufferNumber.push_back(handlemsg->holdCard[i].num );
			for(i=0;i<3;i++)
				if(handlemsg->flopCard[i].color ==coul)
					MyBufferNumber.push_back(handlemsg->flopCard[i].num);

			if(handlemsg->turnCard.color==coul)
				MyBufferNumber.push_back(handlemsg->turnCard.num);

			sort(MyBufferNumber.begin(),MyBufferNumber.end());
			straight=Straight(MyBufferNumber);
	
		}
		else 
		{
			for( i=0;i<=1;i++)
				if(handlemsg->holdCard[i].color ==coul)
					MyBufferNumber.push_back(handlemsg->holdCard[i].num);
			for(i=0;i<3;i++)
				if(handlemsg->flopCard[i].color ==coul)
					MyBufferNumber.push_back(handlemsg->flopCard[i].num);

			if(handlemsg->turnCard.color==coul)
				MyBufferNumber.push_back(handlemsg->turnCard.num);
			if(handlemsg->riverCard.color ==coul)
				MyBufferNumber.push_back(handlemsg->riverCard.num);

			sort(MyBufferNumber.begin(),MyBufferNumber.end());
			straight=Straight(MyBufferNumber);

		}
		return straight;
	}
	else//����ͬ��˳
		return 0;
}
int King_Straight_Flust(vector<int> & number,vector<int> & coler,HandleMsg *handlemsg)
{
//��������ͬ��˳��������ΪAʱ����Ϊ�ʼ�ͬ��˳����û���򷵻�0 
	 if(Straight_Flust( number,coler,handlemsg)==14)
		return 14;
	 else
		 return 0;
}
