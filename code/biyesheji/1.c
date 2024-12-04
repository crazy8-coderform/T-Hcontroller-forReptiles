#include<reg52.h>							//ͨ��18-1 37 ����ϱ�ҵ��ƣ�������֣�
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
sbit J1=P3^7;									//����
sbit J2=P3^6;									//���ȵ�
sbit J3=P3^5;									//��ʪ��
sbit A1=P3^4;                 //������
uint aa=0,cc=0,kk,ll;
uint flag;
uint U8R=0,U8RL=0,U8=0,U8L=0,COM=0;
uchar bb,n,nn=1,K1,K2,K3,K4,K11,K12;
void delay(uint z)   //������ʱ����(����) ���ݻ������ڼ���ó�
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
void delay_10us()	//�Ա�׼��89c52��˵�κ�һ��ָ����ڻ����1US,nop����C������û�У���Ҫ����ͷ�ļ�������debug����һ��nop��ԼΪ1.1us������д9��
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
void zhi (uint a)  //1602ָ���������
{
	delay(1);
	e=0;
	rs=0;
	rw=0;
	data=a;
	e=1;
	e=0;
}
void shu (uchar b)  //1602�����������uchar ת uint ����1602�ַ�����ʾ����
{
	delay(1);
	e=0;
	rs=1;
	rw=0;
	data=b;
	e=1;
	e=0;
}
void inta (void)  //1602��ʼ������
{
	zhi(0x38);
	zhi(0x0c);
	zhi(0x06);
}
void show(void)   //1602��ʾ����
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
		if(K2!=0&&K11==0)							//���¼�1ʱ�ڼ����޸ĵ���Ŀ�ϱ��ͺţ���ʱ�ٰ��¼�2����,�ٰ��¼�1*��ʧ��Ԥ��ֵȷ��
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
	shang[13]=kk%10+0x30;						//��ʾԤ��ο�ֵ
	xia12[12]=ll/10+0x30;
	xia12[13]=ll%10+0x30;						
	if(K2==0&&K11==0)								//������¼������߼�
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
	delay(20);						//������Ҫ����18ms
	dt=1;
	delay_10us();
	delay_10us();
	delay_10us();					//�����������40us�����20us����ζ��Ҫѡ��40<x<100us��ѡ���ӳ�
	delay_10us();
	delay_10us();
	delay_10us();
	delay_10us();
	if(dt==0)							//�ж���Ч��Χ���Ƿ��յ�DHT11��Ӧ�ź�
	{
		while(!dt)
		{
			n++;
			if(n==3000)
			{
				nn=1;
				n=0;							//��ֹ����whileѭ��
				break;
			}
		}
		while(dt)					//�����ߵ͵�ƽֱ�ӽ������ݽ��ս׶�
		{
			n++;
			if(n==3000)
			{
				nn=1;
				n=0;
				break;
			}
		}
		nn=0;							//��⵽��Ӧ�źź�����������
	}
}
void dht11_com()
{
	n=0;
	while(!dt)						//����50us�͵�ƽ
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
	delay_10us();				//�ж�30us���Ƿ�Ϊ�ߵ�ƽ������Ϊ1��������Ϊ0
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
		U8R <<=1;								//��Ҫ���ܰ˴����ݣ�������Ҫ����7�Σ������ƶ���ֵ
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
	if(bb==COM&&nn==0)					//У����ȶԣ���ȷ��ֵ��ʾ
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
			K11=1;							//���������̧�𰴼���ɾ��*
			K12=0;
		}
		delay(980);
		if(k1==0)
		{
			K1=1;										//Ԥ������谴�³���1S��*
		}
	}
		if(k2==0)
	{
		delay(20);
		if(k2==0)
		{
			K2=~K2;									//�л���������*����
		}
	}
		if(k3==0)
	{
		delay(10);
		if(k3==0)
		{
			K3=1;									//�¼�
		}
	}
		if(k4==0)
	{
		delay(8);
		if(k4==0)
		{
			K4=1;									//�ϼ�
		}
	}
}
void relay()
{
	if(aa<kk&&K12==0)
	{
		J1=1;
		J2=0;											//*����ʧ�����¶�С��Ԥ���¶�ʱ�������ȵ棨���£�
		J3=1;
	}
	if(aa>kk&&K12==0)
	{
		J1=1;											//*����ʧ�����¶ȴ���Ԥ���¶�ʱȫ���������������½���	�����£�
		J2=1;
		J3=1;
	}
	if(cc<ll&&K12==0)
	{
		J1=1;
		J2=1;
		J3=0;										//*����ʧ����ʪ��С��Ԥ��ֵʱ������ʪ��		����ʪ��
	}
	if(cc>ll&&K12==0)
	{
		J2=1;									//*����ʧ����ʪ��С��Ԥ��ֵʱ��������		����ʪ��
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
	delay(1500);					//�������ϵ��Ҫ�ȴ� 1s ��Խ�����ȶ�״̬�ڴ��ڼ����跢���κ�ָ�
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
		show();						//��ֹ�ϵ���Ļ����
		show();	
		relay();
		alarm();
	}
}