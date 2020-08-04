//*******************************************************************
// CPU : Megawin MPC89x515A @ 11.0592 MHz
//*******************************************************************
#include "82GXX.H"
#include "bin.c"
//********************** functions ***************************
void delay_1ms(int count);
void trig_out(void);
void LEDTypeError(void);
void LEDTypeLoad(void);
//********************** Parameter ***************************
sbit trigPin = P3^0;
sbit echoPin = P3^1;
char code Column_Scan[4]= {0x7F,0xBF,0xDF,0xEF};
char code Seg_Row_Data[19]= { 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x98,0x88,0x83,0xC6,0xA1,0x86,0x8E,0x92,0x8C,0xF7};//0~F
//********************** Variables ***************************
main()
{ 
int i=0,LED_TEMP[4]={0,0,0,0},TEMP,deTime=0,KeydeTime=6000,cnt,chcnt,cuta,cutb,OFFLED=0;
int A_KEY=0,F_KEY=0,E_KEY=0,Key_Scan,Command_SCAN,KET_TEMP[4]={0,0,0,0};
	trigPin = 0;
while(1)
{   P2=0xFF;
    if(i==1&&OFFLED==0){
    	P0=Seg_Row_Data[LED_TEMP[i]]^0x80;
	}
	else{
		P0=Seg_Row_Data[LED_TEMP[i]];
	}
    P2=Column_Scan[i];
    Key_Scan=P2&0x0F;
    if(KeydeTime!=0)KeydeTime--; 
    if((Key_Scan!=0x0F)&&(KeydeTime==0)){		
        KeydeTime=6000;
    	if (Key_Scan==0x07) {
        	TEMP=i*4;//0 4 8 12 ~ 0 4 8 C
		}
        else if(Key_Scan==0x0B){
        	TEMP=i*4+1;//1 5 9 13 ~ 1 5 9 D
		} 
        else if(Key_Scan==0x0D){
        	TEMP=i*4+2;//2 6 10 14 ~ 2 6 A E
		} 
        else if(Key_Scan==0x0E){
        	TEMP=i*4+3;//3 7 11 15 ~ 3 7 B F
		}
		if(TEMP<10){
			KET_TEMP[3]=KET_TEMP[2];
			KET_TEMP[2]=KET_TEMP[1];
			KET_TEMP[1]=KET_TEMP[0];
			KET_TEMP[0]=TEMP;
		}
		else{
			Command_SCAN=TEMP;
		}
		}    
	if(Command_SCAN!=0){	
       if(Command_SCAN==10){// A «ùÄò´ú¶Z 
          A_KEY=1;
	   }
       if(Command_SCAN==11){// B ¼È°±´ú¶Z 
          A_KEY=0;
	   }	   
       if(Command_SCAN==12){// C µ²§ô 
          A_KEY=0;
          E_KEY=0;
          F_KEY=0;
          LED_TEMP[3]=LED_TEMP[2]=LED_TEMP[1]=LED_TEMP[0]=0; 
	   }
       if(Command_SCAN==15){// F  
          A_KEY=0;
          F_KEY=1;
	   }	 
       if(Command_SCAN==14){// E
	      LEDTypeLoad();  
          chcnt=KET_TEMP[3]*1000+KET_TEMP[2]*100+KET_TEMP[1]*10+KET_TEMP[0];
          if(chcnt>=1500||F_KEY==0){
          	 LEDTypeError();
		  }
		  else{
		  	 F_KEY=0;
		  	 E_KEY=1;
		  }
	   }	    	   	   	
	   Command_SCAN=0;
	}
	if(E_KEY==1&&deTime==4000){//
	deTime=0;cnt=0;
	trig_out();
	while(echoPin == 0);	
	while(echoPin == 1 && cnt < 3000){
		cnt++;
	}	
	cuta=(chcnt*9)/10;
	cutb=(chcnt*11)/10;
	if(cnt>=cuta&&cnt<=cutb){
			LED_TEMP[3]=17;
			LED_TEMP[2]=10;
			LED_TEMP[1]=16;
			LED_TEMP[0]=16; 
			OFFLED=1;		
	}
	else{
      LED_TEMP[3]=cnt/1000;
      LED_TEMP[2]=cnt% 1000 / 100;
      LED_TEMP[1]=cnt% 100 / 10;
      LED_TEMP[0]=cnt%10; 
	  OFFLED=0;   
	       if(cnt>=3000){
     	LED_TEMP[3]=18;
			LED_TEMP[2]=18;
			LED_TEMP[1]=18;
			LED_TEMP[0]=18; 
	 }		
	}      
    }	
	if(F_KEY==1){//
			LED_TEMP[3]=KET_TEMP[3];
			LED_TEMP[2]=KET_TEMP[2];
			LED_TEMP[1]=KET_TEMP[1];
			LED_TEMP[0]=KET_TEMP[0];     
    }		    
    if(deTime<4000)deTime++;
    if(A_KEY==1&&deTime==4000){
	deTime=0;cnt=0;
	trig_out();
	while(echoPin == 0);	
	while(echoPin == 1 && cnt < 3000){
		cnt++;
	}	
    LED_TEMP[3]=cnt/1000;
    LED_TEMP[2]=cnt% 1000 / 100;
    LED_TEMP[1]=cnt% 100 / 10;
    LED_TEMP[0]=cnt%10;
     if(cnt>=3000){
     	LED_TEMP[3]=18;
			LED_TEMP[2]=18;
			LED_TEMP[1]=18;
			LED_TEMP[0]=18; 
	 }
    }
	i++;if(i==4)i=0;
}
}
//-----------------------------------------------------------------
void trig_out()
{	
  int i;
  trigPin = 1;
  for(i = 0; i < 125; i ++);	 
  trigPin = 0;
}
void LEDTypeLoad(void){
	int f;
	for(f=3;f>=0;f--){
		P2=Column_Scan[f];
			P0=B1111_1011;delay_1ms(150);
            P0=B1111_1101;delay_1ms(150);
            P0=B1111_1110;delay_1ms(150);
            P0=B1101_1111;delay_1ms(150);
            P0=B1110_1111;delay_1ms(150);
            P0=B1111_1111;delay_1ms(150);
            P0=B0111_1111;delay_1ms(150);	
        }
}
void LEDTypeError(void){
	char code Errortype[13]={0xFF,0xFF,0xFF,0xFF,0x86,0xAF,0xAF,0xA3,0xAF,0xFF,0xFF,0xFF,0xFF};
	int i=0,j=0,k=3,deTime=0;;
	while(1) {
	  P2=0xFF;
	  P0=Errortype[j+k];
      P2=Column_Scan[i];
      deTime++;
      if(deTime==6000){
          j++;
	      deTime=0;	
	  }
      if(j==10)break;
      i++;if(i==4)i=0;
      k--;if(k==-1)k=3;
	}
}
void delay_1ms(int count)
{ int i,j;
   for(i=0;i<count;i++) for(j=0;j<40;j++);
}
