#include<reg52.h>							//通本18-1 37 宋昕煜毕业设计（软件部分）
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define data P0
#define busy
sbit rs=P1^4;
sbit rw=P1^5;
sbit e=P1^6;
sbit dt=P2^7;
sbit k1=P2^0;
sbit k2=P2^1;
sbit k3=P2^2;
sbit k4=P2^3;
sbit J1=P3^7;									//风扇
sbit J2=P3^6;									//加热垫
sbit J3=P3^5;									//加湿器
sbit A1=P3^4;                 //蜂鸣器
uint aa=0,cc=0,kk,ll;
uint flag;
uint U8R=0,U8RL=0,U8=0,U8L=0,COM=0;
uchar bb,n,nn=1,K1,K2,K3,K4,K11,K12;
void delay(uint z)   //任意延时程序(毫秒) 根据机器周期计算得出
{
	uint i;
	uchar j;
	for(i=0;i<z;i++)
	{	
		for(j=0;j<110;j++)
		{
		}
	}
}
void delay_10us()	//对标准的89c52来说任何一个指令都等于或大于1US,nop函数C语言中没有，需要引入头文件，经过debug分析一个nop大约为1.1us，所以写9个
{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
}
void zhi (uint a)  //1602指令输入程序
{
	delay(1);
	e=0;
	rs=0;
	rw=0;
	data=a;
	e=1;
	e=0;
}
void shu (uchar b)  //1602数据输入程序，uchar 转 uint 根据1602字符表显示内容
{
	delay(1);
	e=0;
	rs=1;
	rw=0;
	data=b;
	e=1;
	e=0;
}
void inta (void)  //1602初始化程序
{
	zhi(0x38);
	zhi(0x0c);
	zhi(0x06);
}
void show(void)   //1602显示程序
{
	uchar k,g;
	uchar shang[16]={" NT:   C ET:   C"};
	uchar xia12[16]={" NM:   R EM:   R"};
	shang[4]=aa/10+0x30;
	shang[5]=aa%10+0x30;
	xia12[4]=cc/10+0x30;
	xia12[5]=cc%10+0x30;
	if(K1==1)
	{
		if(K2==0&&K11==0)
		{
			shang[8]='*';
			xia12[8]=' ';
			K12=1;
			delay(100);
		}
		if(K2!=0&&K11==0)							//按下键1时在即将修改的项目上标型号，此时再按下键2换行,再按下键1*消失，预定值确认
		{
			shang[8]=' ';
			xia12[8]='*';
			K12=1;
			delay(100);
		}
		if(K11==1)
		{
			shang[8]=' ';
			xia12[8]=' ';
			K1=0;
			K2=0;
			K11=0;
		}
	}	
	shang[12]=kk/10+0x30;
	shang[13]=kk%10+0x30;						//显示预设参考值
	xia12[12]=ll/10+0x30;
	xia12[13]=ll%10+0x30;						
	if(K2==0&&K11==0)								//设计上下键操作逻辑
	{
		if(K3==1)
		{
			K3=0;
			kk--;
		}
		if(K4==1)
		{
			K4=0;
			kk++;
		}
	}
		if(K2!=0&&K11==0)
	{
		if(K3==1)
		{
			K3=0;
			ll--;
		}
		if(K4==1)
		{
			K4=0;
			ll++;
		}
	}
	
	inta();
	zhi(0x80);
	for(k=0;k<16;k++)
	{
		shu(shang[k]);
	}
	zhi(0xc0);
	for(g=0;g<16;g++)
	{
		shu(xia12[g]);
	}
}
void dht11_start()
{
	n=0;
	dt=0;
	dt=1;
	dt=0;											
	delay(20);						//至少需要拉低18ms
	dt=1;
	delay_10us();
	delay_10us();
	delay_10us();					//主机最多拉高40us，最低20us，意味着要选择40<x<100us内选择延迟
	delay_10us();
	delay_10us();
	delay_10us();
	delay_10us();
	if(dt==0)							//判断有效范围内是否收到DHT11响应信号
	{
		while(!dt)
		{
			n++;
			if(n==3000)
			{
				nn=1;
				n=0;							//防止卡在while循环
				break;
			}
		}
		while(dt)					//跳过高低电平直接进入数据接收阶段
		{
			n++;
			if(n==3000)
			{
				nn=1;
				n=0;
				break;
			}
		}
		nn=0;							//检测到响应信号后，允许检测数据
	}
}
void dht11_com()
{
	n=0;
	while(!dt)						//跳过50us低电平
	{
		n++;
		if(n==3000)
		{
			nn=1;
			n=0;
			break;
		}
	}
	delay_10us();
	delay_10us();				//判断30us后是否为高电平，是则为1，不是则为0
	if(dt==1)
	{
		flag=1;
		while(dt)
		{
			n++;
			if(n==3000)
			{
				nn=1;
				n=0;
				break;
			}
		}
	}
	else flag=0;
}
void dht11_rec()
{
	uint i;
	dht11_start();
	for(i=0;i<8;i++)
	{
		if(nn==0)
		{
		dht11_com();
		U8R <<=1;								//需要接受八次数据，但仅需要左移7次，故先移动后赋值
		U8R |=flag;
		}
	}
	for(i=0;i<8;i++)
	{
		if(nn==0)
		{
		dht11_com();
		U8RL <<=1;
		U8RL |=flag;
		}
	}
	for(i=0;i<8;i++)
	{
		if(nn==0)
		{
		dht11_com();
		U8 <<=1;
		U8 |=flag;
		}
	}
	for(i=0;i<8;i++)
	{
		if(nn==0)
		{
		dht11_com();
		U8L <<=1;
		U8L |=flag;
		}
	}
		for(i=0;i<8;i++)
	{
		if(nn==0)
		{
		dht11_com();
		COM <<=1;
		COM |=flag;
		}
	}
	bb=U8R+U8RL+U8+U8L;
	if(bb==COM&&nn==0)					//校验码比对，正确数值显示
	{
		aa=U8;
		cc=U8R;
	}
}
void key_detection()
{
	if(k1==0)
	{
		delay(20);
		if(k1==0&&K12==1)
		{
			K11=1;							//如果半毫秒后抬起按键则删除*
			K12=0;
		}
		delay(980);
		if(k1==0)
		{
			K1=1;										//预设键，需按下超过1S表*
		}
	}
		if(k2==0)
	{
		delay(20);
		if(k2==0)
		{
			K2=~K2;									//切换键，按下*换行
		}
	}
		if(k3==0)
	{
		delay(10);
		if(k3==0)
		{
			K3=1;									//下键
		}
	}
		if(k4==0)
	{
		delay(8);
		if(k4==0)
		{
			K4=1;									//上键
		}
	}
}
void relay()
{
	if(aa<kk&&K12==0)
	{
		J1=1;
		J2=0;											//*号消失后并且温度小于预定温度时启动加热垫（升温）
		J3=1;
	}
	if(aa>kk&&K12==0)
	{
		J1=1;											//*号消失后并且温度大于预定温度时全不启动，依靠室温降温	（降温）
		J2=1;
		J3=1;
	}
	if(cc<ll&&K12==0)
	{
		J1=1;
		J2=1;
		J3=0;										//*号消失后并且湿度小于预定值时启动加湿器		（加湿）
	}
	if(cc>ll&&K12==0)
	{
		J2=1;									//*号消失后并且湿度小于预定值时启动风扇		（除湿）
		J1=0;
		J3=1;
	}
}
void alarm()
{
	if(aa>30||aa<20)
	{
		A1=0;
	}
	if(cc>80||cc<30)
	{
		A1=0;
	}
	if(aa<=30&&aa>=20&&cc<=80&&cc>=30)
	{
		A1=1;
	}
}
void main()
{
	delay(1500);					//传感器上电后，要等待 1s 以越过不稳定状态在此期间无需发送任何指令。
	K1=0;
	K11=0;
	K12=0;
	K2=0;
	K3=0;
	K4=0;
	kk=25;
	ll=40;
	while(1)
	{
		U8R=0,U8RL=0,U8=0,U8L=0,COM=0;
		delay(50);
		dht11_rec();
		nn=1;
		key_detection();
		show();							
		show();						//防止上电屏幕花屏
		show();	
		relay();
		alarm();
	}
}