

#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<ctime>
#include<string>
#include<cmath>
using namespace std;
/*全局变量：某贸易中心共L层，设有载客电梯N部。
每部电梯的最大乘员量均为K人/K*100Kg（K值可以根据仿真情况在10~20人之间确定）。总运行时间T*/
int L=10,N=3,K=8,T=10;
ofstream fout("shuchu.txt");
class Passenger
{public:
int AppearTime;       // 出现的时间
int AppearFloor;      //出现的楼层 
int Destination;      // 要去的楼层
friend bool operator!=(const Passenger&,const Passenger&);
friend ostream& operator << (ostream &out,Passenger &p);
int DirectionJudgement()               //  判断按上下(方向 向上为1 向下为-1 不动为0)
{if(AppearFloor < Destination) return (1);
	   else if(AppearFloor>Destination) return (-1);
	     else return (0);
	}
Passenger(int a=0,int b=-1,int c=-1)
{
	
	srand(unsigned(time(0)));            //初始化 随机时间和楼层
	AppearTime=0;//AppearTime=rand()%(T-10);
	AppearFloor=10;//AppearFloor=rand()%L+1;
	Destination=9;//Destination=rand()%L+1;
	
}
	friend class Building;
	friend class Elevator;
};
ostream& operator << (ostream &out,Passenger &p)
{
	out<<"此人出现在第"<<p.AppearFloor<<"层，要去第"<<p.Destination<<"层"<<endl;
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
	int Number;          //电梯序号   (0~N-1)
	int CurrentFloor;     // 当前楼层   (1~L)
	int CurrentPopulation; //当前人数  (0~K)
	int Direction;         //运动方向  (方向 向上为1 向下为-1 !!!空梯为0)  
	int* Floor;        //指针指向记录电梯内面板储存的每层所到人数的数组      
	int StartTime;       //运行状态的初始时间
	int Velocity;       //电梯运行速度     
	int PassTime;//平均每人进出电梯所用时间
	int InTime;   //!!!停靠时间  
	Passenger* Request;  //!!!外部的请求
public:
	bool IsEmpty(){return (CurrentPopulation==0);}     //判断空梯     
	bool IsFull(){return (CurrentPopulation==K);}       //判断满梯           

	void WentOn(Building & b, Passenger *pa );      //上人函数      
	void WentOff(Building & b);     //下人函数
	void UpDown();      //电梯上下楼函数      
	void Stop(int);        //停靠函数
	void Run(int time,Building &b,Passenger *Pa)  ;     //运行函数
	bool IsWentoff;//判断是否在下人
	bool IsWenton;//判断是否在上人
	friend ostream& operator << (ostream &out,Elevator &e);
Elevator()                      //初始化
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
	cout<<"电梯运行中"<<endl;
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
	if(e.Direction==1) s="上楼";
	if(e.Direction==0) s="空梯";
	if(e.Direction==-1) s="下楼";
	out<<"电梯序号："<<e.Number<<'\t'<<"电梯目前状态："<<s<<'\t'<<"电梯目前的楼层:"<<e.CurrentFloor<<'\t'<<"电梯目前的人数"<<e.CurrentPopulation<<endl;
	return out;

}

class Building
{
	int a;  //乘客数目
	Passenger* Pa;  //指针指向随机出现的乘客 
	int* EachFloor;//指针指向每层的外部按键情况的数组  （按键分 向上1 向下-1 没有人按为0 上下同时按为2）
	Elevator* Ele;//指针指向电梯数组 
	public:
	int t;           //当前时间
	int Start();   //总的开始函数 计时t
	int Select(int);  //调度函数 返回值为电梯序号（失败为-1）
	int geta(){return a;}
	friend class Elevator;
Building(int La,int Na,int Ka,int Ta)  //构造函数 初始化
{
	cout<<"构造了一个大楼类的对象"<<endl;
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
	cout<<"大楼类析构"<<endl;
	//delete [] EachFloor;
	delete[] Ele;
	delete[] Pa;}

};


int Building::Select(int n)
{
	int i,j,k;
	int *order=new int[N];//优先选择电梯的序列
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

	   cout<<"第"<<n<<"人选择第"<<k<<"号电梯"<<endl;
	   return (k);}
	if(i==N)
	  {k=0;
	   if(Pa[n].DirectionJudgement()<0)
		   for(i=1;i<N;i++) if(Ele[i].CurrentFloor>Ele[k].CurrentFloor)k=i;//取反方向楼层最高的电梯
		   else    for(i=1;i<N;i++) if(Ele[i].CurrentFloor<Ele[k].CurrentFloor)k=i;
	   delete[] order;
	   cout<<"第"<<n<<"人选择第"<<k<<"号电梯"<<endl;
	   return (k);}
	}


int Building::Start()
{
	cout<<"程序开始"<<endl;
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

//building添加geta()

	void Elevator:: WentOn( Building & b, Passenger *pa )     //上人函数	  
	{	
	
		int count=0;
		for (int i=0;i<b.geta();i++)
	
		{
			if( (CurrentFloor==pa[i].AppearFloor)&&!IsFull() &&( Direction==pa[i].DirectionJudgement()||Direction==0)&&pa[i].AppearTime<b.t)	//电梯未满、请求方向与电梯运动方向同
			{count++;Floor[pa[i].Destination]++;Request[pa[i].AppearFloor].AppearFloor=-1;
			if(CurrentPopulation==0)
			{Direction=pa[i].DirectionJudgement();pa[i].AppearFloor=-1;}//delete this person
			}
		}
		if(count)
		{
			IsWenton=1;cout<<"电梯上人"<<endl;
		
		}
		InTime=PassTime*count;//记录上人所用的时间
		
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

	void Elevator::WentOff(Building & b)     //下人函数	
	{	
		if( Floor[CurrentFloor]!=0 )
		{	
			IsWentoff=1;
			InTime=PassTime*Floor[CurrentFloor];
			Floor[CurrentFloor]=0;
			cout<<"电梯下人"<<endl;
	
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
		cout<<"进入主函数了"<<endl;
		Building one(L,N,K,T);
		one.Start();
	}