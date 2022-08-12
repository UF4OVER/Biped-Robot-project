#include "gps.h"
#include "sys.h"
extern uint8_t RxBuffer4[400]; 
extern uint8_t Rec_Len;
uint8_t Hour,Min,Sec,Day,Mouth,Year,GPS_VA,weidu_dir,jingdu_dir,star_num,star_num_t,star_run[12],GPS_3D;
float jingdu,weidu;
u16 star_info[12][4],star_info_t[12][4],GPS_ATLI;
void GPRMC_DAT(void);
void GPGSV_DAT(void);
void GPGSA_DAT(void);
void GPGGA_DAT(void);
/****************************************************************************
* ��    �ƣ�void GPRMC_DAT(void)
* ��    �ܣ�GPS NMEA GPRMCЭ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void GPRMC_DAT(void){	
   unsigned char i,i1=0,uf=0;
   float l_g;
		
   for(i=0;i<Rec_Len;i++){
   	  if(RxBuffer4[i]==0x2c){				                         //�ж��Ƿ��Ƕ���
	  	i1++;
		  uf=0;
	  }	 
	  if(i1==1&&uf==0){	                                             //GPRMC ʱ��
	  	Hour=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30)+8;		 //ʱ
   	  Min=(RxBuffer4[i+3]-0x30)*10+(RxBuffer4[i+4]-0x30);		     //��
      Sec=(RxBuffer4[i+5]-0x30)*10+(RxBuffer4[i+6]-0x30);		     //�� 
		  i=i+6;		
		  uf=1;	
   	  }	
	  else if(i1==2&&uf==0){	                                     //GPRMC ״̬��Ч��
	  	if(RxBuffer4[i+1]=='A')	GPS_VA=1; 		                     //�ɹ���λ
		  else GPS_VA=0;												 //��λδ�ɹ�
		i++;	
		uf=1;
   	  }	
	  else if(i1==3&&uf==0){	                                     //GPRMC γ��
	  	if(RxBuffer4[i+1]==0x2c) weidu=0;
	  	else {
//			weidu=((RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30)+	 //�����γ��ֵ����������
//			       (((((RxBuffer4[i+3]-0x30)*10)+(RxBuffer4[i+4]-0x30))/0.6)*0.01))*3600;
        weidu=(RxBuffer4[i+1]-0x30)*1000+(RxBuffer4[i+2]-0x30)*100+(RxBuffer4[i+3]-0x30)*10+(RxBuffer4[i+4]-0x30);
//			l_g=(((RxBuffer4[i+6]-0x30)*1000)+((RxBuffer4[i+7]-0x30)*100)+     //�����γ��ֵ��С������
//			     ((RxBuffer4[i+8]-0x30)*10)+(RxBuffer4[i+9]-0x30))*0.006;
l_g=(((RxBuffer4[i+6]-0x30)*10000)+((RxBuffer4[i+7]-0x30)*1000)+     //�����γ��ֵ��С������
			     ((RxBuffer4[i+8]-0x30)*100)+(RxBuffer4[i+9]-0x30)*10+(RxBuffer4[i+10]-0x30))/100000.0;
	    	weidu=weidu+l_g;                						 //���յ�γ��ֵ ����34.xxxx
			i=i+9;
		}
		uf=1;	
	  }
	  else if(i1==4&&uf==0){	                                     //GPRMC γ��	�ϱ������ʾ 
	  	if(RxBuffer4[i+1]==0x2c) jingdu_dir=0;
		else if(RxBuffer4[i+1]=='N') weidu_dir=0;                    //��γ
		else if(RxBuffer4[i+1]=='S') weidu_dir=1;                    //��γ
		i++; 
		uf=1;	
	  } 			
	  else if(i1==5&&uf==0){	                                     //GPRMC ����
	  	if(RxBuffer4[i+1]==0x2c) jingdu=0; 
		else{
//			jingdu=((RxBuffer4[i+1]-0x30)*100+(RxBuffer4[i+2]-0x30)*10+	   //���������ֵ����������
//			(RxBuffer4[i+3]-0x30)+(((((RxBuffer4[i+4]-0x30)*10)+(RxBuffer4[i+5]-0x30))/0.6)*0.01))*3600;
//			l_g=(((RxBuffer4[i+7]-0x30)*1000)+((RxBuffer4[i+8]-0x30)*100)+  //���������ֵ��С������ 
//			    ((RxBuffer4[i+9]-0x30)*10)+(RxBuffer4[i+10]-0x30))*0.006;
    jingdu=((RxBuffer4[i+1]-0x30)*10000+(RxBuffer4[i+2]-0x30)*1000+	   //���������ֵ����������
			(RxBuffer4[i+3]-0x30)*100+(RxBuffer4[i+4]-0x30)*10)+(RxBuffer4[i+5]-0x30);
			l_g=(((RxBuffer4[i+7]-0x30)*10000)+((RxBuffer4[i+8]-0x30)*1000)+  //���������ֵ��С������ 
			    ((RxBuffer4[i+9]-0x30)*100)+(RxBuffer4[i+10]-0x30)*10+(RxBuffer4[i+10]-0x30))/100000.0;
	    	jingdu=jingdu+l_g;              								//���յľ���ֵ ����107.xxxx
			i=i+10;
		}
		uf=1;	
	  }
	  else if(i1==6&&uf==0){	                                     //GPRMC ����	�������� 
	  	if(RxBuffer4[i+1]==0x2c) jingdu_dir=0;
		else if(RxBuffer4[i+1]=='E') jingdu_dir=0;                   //����
		else if(RxBuffer4[i+1]=='W') jingdu_dir=1;                   //����
		i++; 
		uf=1;	
	  } 
	  else if(i1==7&&uf==0){	                                     //GPRMC �����ٶ�  
	  	uf=1;	
	  }
	  else if(i1==8&&uf==0){	                                     //GPRMC �ٶȷ��� 
	  	uf=1;	
	  }
	  else if(i1==9&&uf==0){	                                     //GPRMC ����
	  	Day=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30);			 //��
   		Mouth=(RxBuffer4[i+3]-0x30)*10+(RxBuffer4[i+4]-0x30);		 //��
   		Year=(RxBuffer4[i+5]-0x30)*10+(RxBuffer4[i+6]-0x30);		 //��		
		i=i+6;
		uf=1;	
   	  }	
   }																 
}							
/****************************************************************************
* ��    �ƣ�void GPGSV_DAT(void)
* ��    �ܣ�GPS NMEA GPGSVЭ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void GPGSV_DAT(void){	
   unsigned char i,i1=0,no=0,uf=0,gsv_no=0;  
   for(i=0;i<Rec_Len;i++){
   	  if(RxBuffer4[i]==0x2c){
	  	i1++;
		uf=0;
	  }	 
	  if(i1==1&&uf==0){	                                         //GPGSV�����   	      
		uf=1;	
   	  }	
	  else if(i1==2&&uf==0){	                                 //GPGSV ���� 0-2
	    gsv_no=RxBuffer4[i+1]-0x31;
		i++;		
		uf=1;	
   	  }	
	  else if(i1==3&&uf==0){	                                 //������
	  	star_num=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30);
		i=i+2;	
		uf=1;	
   	  }	
	  else if((i1==4||i1==8||i1==12||i1==16)&&uf==0){	          //���� ��� 	   00-32
	  	no=i1/4;
		if(RxBuffer4[i+1]==0x2c||RxBuffer4[i+1]=='*') star_info[(no-1)+gsv_no*4][0]=0xff;  //�����
		else star_info[(no-1)+gsv_no*4][0]=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30); //���ֵ  
		//i++;
		uf=1;	
	  }
	  else if((i1==5||i1==9||i1==13||i1==17)&&uf==0){	          //���� ���  ����	00-90
	  	if(RxBuffer4[i+1]==0x2c||RxBuffer4[i+1]=='*') star_info[(no-1)+gsv_no*4][1]=0xff;  //������
		else star_info[(no-1)+gsv_no*4][1]=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30); //��Ч������ֵ 
		uf=1;	
	  }															   
	  else if((i1==6||i1==10||i1==14||i1==18)&&uf==0){	          //���� ���  ��λ��	00-359
	  	if(RxBuffer4[i+1]==0x2c||RxBuffer4[i+1]=='*') star_info[(no-1)+gsv_no*4][2]=0xff;    //�޷�λ��
		else star_info[(no-1)+gsv_no*4][2]=(RxBuffer4[i+1]-0x30)*100+			 //��Ч�ķ�λ��ֵ
			 (RxBuffer4[i+2]-0x30)*10+(RxBuffer4[i+3]-0x30); 
		uf=1;	
	  }	
	  else if((i1==7||i1==11||i1==15||i1==19)&&uf==0){	          //���� ��� �ź�ǿ��	00-99
	  	if(RxBuffer4[i+1]==0x2c||RxBuffer4[i+1]=='*') star_info[(no-1)+gsv_no*4][3]=0xff;  //���ź�ǿ��
		else star_info[(no-1)+gsv_no*4][3]=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30); //��Ч���ź�ǿ��
		uf=1;	
	  }	  
   }      
}	   

/****************************************************************************
* ��    �ƣ�void GPGSA_DAT(void)
* ��    �ܣ�GPS NMEA GPGSAЭ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void GPGSA_DAT(void){	
   unsigned char i,i1=0,uf=0;  
   for(i=0;i<Rec_Len;i++){
   	  if(RxBuffer4[i]==0x2c){				    //Э��������ݼ����-����
	  	i1++;
		uf=0;
	  }	 
	  if(i1==1&&uf==0){	                        //GPGSA ģʽ ��M = �ֶ��� A = �Զ���   	      
		uf=1;	
   	  }	
	  else if(i1==2&&uf==0){	                //��λ��ʽ 1 = δ��λ�� 2 = ��ά��λ�� 3 = ��ά��λ��
	    GPS_3D=RxBuffer4[i+1]-0x30;
		i++;		
		uf=1;	
   	  }	
	  else if((i1==3||i1==4||i1==5||i1==6||i1==7||i1==8||i1==9||i1==10||i1==11||i1==12||i1==13||i1==14)&&uf==0){ //��Ч�źŵ����Ǳ��
	  	if(RxBuffer4[i+1]==0x2c) star_run[i1-3]=0x2c;
		else star_run[i1-3]=(RxBuffer4[i+1]-0x30)*10+(RxBuffer4[i+2]-0x30);
		i=i+2;	
		uf=1;	
   	  }		  
   }	  
}

/****************************************************************************
* ��    �ƣ�void GPGGA_DAT(void)
* ��    �ܣ�GPS NMEA GPGGAЭ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void GPGGA_DAT(void){	
   unsigned char i,i1=0,uf=0;  
   for(i=0;i<Rec_Len;i++){
   	  if(RxBuffer4[i]==0x2c){					//Э��������ݼ����-����
	  	i1++;
		uf=0;
	  }	 
	  if(i1==1&&uf==0){	                          	      
		uf=1;	
   	  }	
	  else if(i1==9&&uf==0){	                //GPS �����ĺ��θ߶�
	    if(RxBuffer4[i+1]==0x2c) GPS_ATLI=0;	//����˴��Ƕ��š���ʾ��Ч
	    else GPS_ATLI=atoi(&RxBuffer4[i+1]);	//����ֵ	
		  uf=1;	
   	  }	 
   }	  
}

