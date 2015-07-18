
#ifndef TYPE_H_
#define TYPE_H_

enum STATUS{FALSE,TRUE};
enum ActionEnum {NOACTION,CHECK,CALL,RAISE,FOLD,BLIND,ALLIN}; 
enum ColorEnum{SPADES,HEARTS,CLUBS,DIAMONDS};
enum PlayerSatus{PLAYING,FOLDED,ELIMINATE};
typedef struct PlayerInformation{
	int pid;//ID
	int seat;//��λ��Ϣ������ʱ�������ɽ���Զ����1��2��3��4��5��6��7���ϼ����ڵ�һ��
	int jetton;//���г���
	int money;//ʣ����
	int bet;//�������ۼ�Ͷע��
	int ebet;
	enum ActionEnum action;//�ж�
	enum PlayerSatus status;
}PlayerInfo;

typedef struct CardInformation {
	int num;
	enum ColorEnum color;
}CardInfo;


typedef struct HandleMessage{
	int state;//��һ���е�ǰ���е��ڼ����׶�  0��1��2��3...... 
	int count;//ÿһ�ֵĺ�ע��ע�Ĵ����ۼӡ�
	PlayerInfo otherPlayer[8];
	PlayerInfo myPlayer;
	CardInfo holdCard[2];
	CardInfo flopCard[3];
	CardInfo turnCard;
	CardInfo riverCard;
	enum ActionEnum action;
	int raiseNum;
	int remainPlayerNum;
	int alivePlayerNum;
	int mySeat;//�ҵ�λ��
	int totalPot;
	int regPlayerNum;//ע�������
	int minCallNum; //���θ�ע��С���
	int ranking;
	int inning;//һ�������еڼ���
	enum STATUS gameOverFlag;
}HandleMsg;



extern HandleMsg pHandle;
extern int myID;
extern FILE *mylog;

#endif
