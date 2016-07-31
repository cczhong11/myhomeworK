

#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<ctime>
#include<string>
#include<cmath>
using namespace std;
/*ȫ�ֱ�����ĳó�����Ĺ�L�㣬�����ؿ͵���N����
ÿ�����ݵ�����Ա����ΪK��/K*100Kg��Kֵ���Ը��ݷ��������10~20��֮��ȷ������������ʱ��T*/
int L=10,N=3,K=8,T=10;
ofstream fout("shuchu.txt");
class Passenger
{public:
int AppearTime;       // ���ֵ�ʱ��
int AppearFloor;      //���ֵ�¥�� 
int Destination;      // Ҫȥ��¥��
friend bool operator!=(const Passenger&,const Passenger&);
friend ostream& operator << (ostream &out,Passenger &p);
int DirectionJudgement()               //  �жϰ�����(���� ����Ϊ1 ����Ϊ-1 ����Ϊ0)
{if(AppearFloor < Destination) return (1);
	   else if(AppearFloor>Destination) return (-1);
	     else return (0);
	}
Passenger(int a=0,int b=-1,int c=-1)
{
	
	srand(unsigned(time(0)));            //��ʼ�� ���ʱ���¥��
	AppearTime=0;//AppearTime=rand()%(T-10);
	AppearFloor=10;//AppearFloor=rand()%L+1;
	Destination=9;//Destination=rand()%L+1;
	
}
	friend class Building;
	friend class Elevator;
};
ostream& operator << (ostream &out,Passenger &p)
{
	out<<"���˳����ڵ�"<<p.AppearFloor<<"�㣬Ҫȥ��"<<p.Destination<<"��"<<endl;
	return out;
}
bool  operator!=(const Passenger& a,const Passenger& b)
{

	if((a.AppearFloor!=b.AppearFloor)||(a.AppearTime!=b.AppearTime )||(a.Destination!=b.Destination))
		return true;
	else return false;
}

class Elevator
{
	int Number;          //�������   (0~N-1)
	int CurrentFloor;     // ��ǰ¥��   (1~L)
	int CurrentPopulation; //��ǰ����  (0~K)
	int Direction;         //�˶�����  (���� ����Ϊ1 ����Ϊ-1 !!!����Ϊ0)  
	int* Floor;        //ָ��ָ���¼��������崢���ÿ����������������      
	int StartTime;       //����״̬�ĳ�ʼʱ��
	int Velocity;       //���������ٶ�     
	int PassTime;//ƽ��ÿ�˽�����������ʱ��
	int InTime;   //!!!ͣ��ʱ��  
	Passenger* Request;  //!!!�ⲿ������
public:
	bool IsEmpty(){return (CurrentPopulation==0);}     //�жϿ���     
	bool IsFull(){return (CurrentPopulation==K);}       //�ж�����           

	void WentOn(Building & b, Passenger *pa );      //���˺���      
	void WentOff(Building & b);     //���˺���
	void UpDown();      //��������¥����      
	void Stop(int);        //ͣ������
	void Run(int time,Building &b,Passenger *Pa)  ;     //���к���
	bool IsWentoff;//�ж��Ƿ�������
	bool IsWenton;//�ж��Ƿ�������
	friend ostream& operator << (ostream &out,Elevator &e);
Elevator()                      //��ʼ��
{

	CurrentPopulation=0;
	Direction=0;
	Floor=new int[L];
	for(int i=0;i<L;i++)Floor[i]=0;
	StartTime=0;Velocity=3;PassTime=2;
	InTime=0;
	Request=new Passenger[L];
	bool IsWentoff=0;
	bool IsWenton=0;
	 for(int i=0;i<L;i++) 
	 {
		 Request[i].AppearFloor=-1;
	 }
}

~Elevator() 
{
	delete [] Floor;
	delete[] Request;
}

	friend class Building;
};

void Elevator::Stop(int t)
{

	if(IsWenton)
	{
		if((t-StartTime)%PassTime==0)
		{
			CurrentPopulation++;
		}
		 if(t == StartTime+InTime)
		{
			IsWenton=0;
		}
	}
	if(IsWentoff)
	{
		if((t-StartTime)%PassTime==0)
		{
			CurrentPopulation--;
		}
		if(t == StartTime+InTime)
		{
			IsWentoff=0;
		}
	}
	if(t == StartTime+InTime)
	{
		StartTime=t;
		InTime=0;
	}
	cout<<"����������"<<endl;
}

void Elevator::Run(int time,Building &b,Passenger *Pa)
{
	

	if(CurrentPopulation!=0)
	{
		if (InTime==0) WentOff(b);
		if (InTime==0) WentOn(b,Pa);
		if (InTime) Stop(time);
		if ((Direction!=0)&&(InTime==0)) UpDown();
	}
	else if(CurrentPopulation==0)
	{
		
		if(Direction)
		{
			if(InTime==0)
				UpDown();
			if (InTime) Stop(time);
		}
		if (InTime==0) 
			WentOn(b,Pa);
		
		else if (Direction==0) 
	{      for(int i=0;i<L;i++) 
				if(Request[i].AppearFloor!=-1) 
			 {  if (i>CurrentFloor)
					Direction=1;
				else Direction=-1;
		     }
		}  
	}
	
}
ostream& operator << (ostream &out,Elevator &e)
{
	string s;
	if(e.Direction==1) s="��¥";
	if(e.Direction==0) s="����";
	if(e.Direction==-1) s="��¥";
	out<<"������ţ�"<<e.Number<<'\t'<<"����Ŀǰ״̬��"<<s<<'\t'<<"����Ŀǰ��¥��:"<<e.CurrentFloor<<'\t'<<"����Ŀǰ������"<<e.CurrentPopulation<<endl;
	return out;

}

class Building
{
	int a;  //�˿���Ŀ
	Passenger* Pa;  //ָ��ָ��������ֵĳ˿� 
	int* EachFloor;//ָ��ָ��ÿ����ⲿ�������������  �������� ����1 ����-1 û���˰�Ϊ0 ����ͬʱ��Ϊ2��
	Elevator* Ele;//ָ��ָ��������� 
	public:
	int t;           //��ǰʱ��
	int Start();   //�ܵĿ�ʼ���� ��ʱt
	int Select(int);  //���Ⱥ��� ����ֵΪ������ţ�ʧ��Ϊ-1��
	int geta(){return a;}
	friend class Elevator;
Building(int La,int Na,int Ka,int Ta)  //���캯�� ��ʼ��
{
	cout<<"������һ����¥��Ķ���"<<endl;
	L=La; N=Na; K=Ka; T=Ta;
	t=0;
	int i;
	EachFloor=new int[L];
	Ele=new Elevator[N];
	for(i=0;i<L;i++) 
		EachFloor[i]=0;
	a=1;//a=rand()%20+10;
	Pa=new Passenger[a];
	for(i=0;i<N;i++) Ele[i].Number=i;
	for(i=0;i<(N/3);i++) Ele[i].CurrentFloor=1;
	for(i=(N/3);i<(N*2/3);i++) Ele[i].CurrentFloor=L;
	for(i=(N*2/3);i<N;i++) Ele[i].CurrentFloor=L/2;
}

~Building()
{
	cout<<"��¥������"<<endl;
	//delete [] EachFloor;
	delete[] Ele;
	delete[] Pa;}

};


int Building::Select(int n)
{
	int i,j,k;
	int *order=new int[N];//����ѡ����ݵ�����
	for(i=0;i<N;i++)  order[i]=i;
	for(i=0;i<N;i++)
		for(j=i+1;j<N;j++)
			if(abs(Pa[n].AppearFloor-Ele[order[i]].CurrentFloor)>abs(Pa[n].AppearFloor-Ele[order[j]].CurrentFloor)) 
			{
				k=order[j];
				order[j]=order[i];
				order[i]=k;
			}
	i=0;		
	while(i<N)
	{
		if ((Ele[order[i]].Direction==Pa[n].DirectionJudgement())||(Ele[order[i]].Direction==0)) break;
		i++;
	}
	if(i<N)
	  {k=order[i];
	   delete[] order;

	   cout<<"��"<<n<<"��ѡ���"<<k<<"�ŵ���"<<endl;
	   return (k);}
	if(i==N)
	  {k=0;
	   if(Pa[n].DirectionJudgement()<0)
		   for(i=1;i<N;i++) if(Ele[i].CurrentFloor>Ele[k].CurrentFloor)k=i;//ȡ������¥����ߵĵ���
		   else    for(i=1;i<N;i++) if(Ele[i].CurrentFloor<Ele[k].CurrentFloor)k=i;
	   delete[] order;
	   cout<<"��"<<n<<"��ѡ���"<<k<<"�ŵ���"<<endl;
	   return (k);}
	}


int Building::Start()
{
	cout<<"����ʼ"<<endl;
	int m,k,x,i;
	t=0;
	while(t<T)
	{
		cout <<t;
		for( i=0;i<N;i++) 
		{
			Ele[i].Run(t,*this,Pa);
			cout<<Ele[i];
		}
		for(i=0;i<a;i++)
			if(t==Pa[i].AppearTime)
			{ 
				cout<<Pa[i];
				m=Select(i); 
				k=Pa[i].AppearFloor;
				Ele[m].Request[k]=Pa[i];
				x=Pa[i].DirectionJudgement(); 
				if(EachFloor[k]==0) 
					EachFloor[k]=x;
				else if(EachFloor[k]!=x) 
					EachFloor[k]=2;
			}
			t++;
	}
	return 0;
}

//building���geta()

	void Elevator:: WentOn( Building & b, Passenger *pa )     //���˺���	  
	{	
	
		int count=0;
		for (int i=0;i<b.geta();i++)
	
		{
			if( (CurrentFloor==pa[i].AppearFloor)&&!IsFull() &&( Direction==pa[i].DirectionJudgement()||Direction==0)&&pa[i].AppearTime<b.t)	//����δ����������������˶�����ͬ
			{count++;Floor[pa[i].Destination]++;Request[pa[i].AppearFloor].AppearFloor=-1;
			if(CurrentPopulation==0)
			{Direction=pa[i].DirectionJudgement();pa[i].AppearFloor=-1;}//delete this person
			}
		}
		if(count)
		{
			IsWenton=1;cout<<"��������"<<endl;
		
		}
		InTime=PassTime*count;//��¼�������õ�ʱ��
		
		if(b.EachFloor[CurrentFloor]!=2)		
		{b.EachFloor[CurrentFloor]=0;}
		else if (Direction==1)
		{
			b.EachFloor[CurrentFloor]=-1;
		}
		else if (Direction==-1)
		{
			b.EachFloor[CurrentFloor]=1;
		}
	}		

	void Elevator::WentOff(Building & b)     //���˺���	
	{	
		if( Floor[CurrentFloor]!=0 )
		{	
			IsWentoff=1;
			InTime=PassTime*Floor[CurrentFloor];
			Floor[CurrentFloor]=0;
			cout<<"��������"<<endl;
	
		}	
	}
	void Elevator ::UpDown()
	{
		InTime=Velocity;
		if ((CurrentFloor==L )&&(Direction==1)) Direction=-1;
		if ((CurrentFloor==1 )&&(Direction==-1)) Direction=1;
		if (Direction==1)
		{
			CurrentFloor++;
		}
		if (Direction==-1)
		{
			CurrentFloor--;
		}
	}
	int main()
	{
		cout<<"������������"<<endl;
		Building one(L,N,K,T);
		one.Start();
	}