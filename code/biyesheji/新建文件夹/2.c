#include<reg52.h>
sbit on=P1^7;
void main()
{
	while(1)
	{
		P0=0x00;
		P1=0x00;
		P3=0x00;
		P2=0x00;
		on=0;
	}
}