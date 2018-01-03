#include "sys.h"
#include "backleg.h"
#include "function.h"
#include "led.h"
#include "motor.h"
#include "delay.h"
#include "check.h"




void BackRun(u8 dir,u8 angle)
{
	u8 len;
	u16 arr_now;              //��ǰһ������������
	
//ʵ����λ��ʵʱ��ʾ��������ǰ�˶�״̬
	static u8 k;             //����k�Ŷ���
	static u8 kj;
	u8 i=0;
	u8 j=0;	
	u16 arr_send;              //��ǰһ����������ֵ
	static u8 back_limit_flag; //֧�����е�����λ����1�����ͼ���λ��ͼƬ
	
	//�������ܣ�ֻ�������ҷ������ܸ�λ�󣬲��ܽ���֧��
	if((body_left_flag==0)&&(body_right_flag==0)&&(lock_flag==1))
	{	
		if(dir==1)		//���Ϊ֧������
		{
			if(back_angle_to_arr(angle)>back_runed_arr)  //����ʱ���Ƚ�ֵΪ���м���װ��ֵ��У׼��ĵ���װ��ֵ�Ĳ�ֵ��������ֵ����Ϊ0
			{				
				back_dir_flag=1;      
				if(back_flag==0)
				{
					back_flag=1;
					delay_ms(200);
					u2_printf("back_flag==1");
					delay_ms(200);
					u2_printf("BackStart");
					delay_ms(200);
					u2_printf("Cartoon_Back_1");
					delay_ms(200);
				}
				Motor_1_START(1);                                                          //֧������
				TIM10_Init(back_angle_to_arr(angle)-back_runed_arr,timer10_freq); //�򿪶�ʱ��
			}
		}
		else if(dir==0)
		{
			if(back_runed_arr>0)    //����ʱ���Ƚ�ֵΪУ׼��ĵ���װ��ֵ��0,�Ҳ���Ϊ0
			{
				back_dir_flag=0; 
				if(1==back_limit_flag)
				{
					back_limit_flag=0;
					delay_ms(200);
					u2_printf("Cartoon_Back_20");
					delay_ms(200);
				}
				Motor_1_START(0);                                                          //֧������
				TIM10_Init(back_runed_arr,timer10_freq); //�򿪶�ʱ��
			}
		}
			memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);			
			USART2_RX_LEN=0;
		
		__HAL_TIM_CLEAR_FLAG(&TIM10_Handler, TIM_SR_CC1IF); // ����жϱ�־λ
		
	 	if(((back_runed_arr!=back_angle_to_arr(angle))&&(1==dir))||((0!=back_runed_arr)&&(0==dir)))
		{					
			while(!(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF)))  //�ȴ���ʱʱ�䵽��ʱ�䵽����ѭ��   
			{		
					//ָֹͣ��
				if(UsartCheck2("BackUpNew","BackDownNew"))
					{
						u2_printf("Stop\r\n");
						break;
					}

				 arr_send=__HAL_TIM_GET_COUNTER(&TIM10_Handler);      //��ǰһ������ֵ
				//���䶯��ָ��
				if(dir==1)
				{
					j=(back_runed_arr+arr_send)/(back_angle_to_arr(angle)/19);
				}
				else
				{
					j=abs(back_runed_arr,arr_send)/(back_angle_to_arr(angle)/19);
				}
				k=back_picture_k;
				if(	k!=j)
				{	
					kj=abs(k,j);					
					if(kj<2)
					{
						k=j;   back_picture_k=k;					
						switch (k)
						{						
							case 1:	u2_printf("Cartoon_Back_2");									
									break;
							case 2:	u2_printf("Cartoon_Back_3");
									break;					
							case 3:	u2_printf("Cartoon_Back_4");
									break;					
							case 4:	u2_printf("Cartoon_Back_5");
									break;	
							case 5:	u2_printf("Cartoon_Back_6");
									break;	
							case 6:	u2_printf("Cartoon_Back_7");
									break;
							case 7:	u2_printf("Cartoon_Back_8");
									break;
							case 8:	u2_printf("Cartoon_Back_9");
									break;						
							case 9:	u2_printf("Cartoon_Back_10");
									break;												
							case 10:u2_printf("Cartoon_Back_11");
									break;
							case 11:u2_printf("Cartoon_Back_12");									
									break;
							case 12:u2_printf("Cartoon_Back_13");
									break;					
							case 13:u2_printf("Cartoon_Back_14");
									break;					
							case 14:u2_printf("Cartoon_Back_15");
									break;	
							case 15:u2_printf("Cartoon_Back_16");
									break;	
							case 16:u2_printf("Cartoon_Back_17");
									break;
							case 17:u2_printf("Cartoon_Back_18");
									break;
							case 18:u2_printf("Cartoon_Back_19");
									break;		
						}
					}				
				}	
			}		
		Motor_1_STOP();    //���ֹͣ
		TIM10_Stop();      //�رն�ʱ��
		//�жϸ�λ
		if((__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))&&(dir==0))   //�ж��Ƿ��ڸ�λ״̬����λ״̬��ǰ�������еĶ�ʱ������
		{				
			arr_now=0;         //��ʱ���ڸ�λ״̬����״ֵ̬����Ϊ0��
			back_flag=0;
			delay_ms(200);
			u2_printf("back_flag==0");
			delay_ms(200);			
		}		
		else
		{
			arr_now=__HAL_TIM_GET_COUNTER(&TIM10_Handler);    //��ȡ��ǰ����ֵarr_now				
			back_flag=1;
		}
      //ͨ���������ж������ۼ�		
		if(	dir==1)        //�����֧�����У�����+
		{
			if(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))
			{  
				back_runed_arr=back_angle_to_arr(angle);
				back_limit_flag=1; 
				delay_ms(200);
				u2_printf("Cartoon_Back_20");
				delay_ms(200);
				u2_printf("BackLim");
				delay_ms(200);
			}
			else
			{  back_runed_arr+=arr_now;	}				
		}
		else                //�����֧�����У�����-
		{
			if(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))
			{	
				back_runed_arr=0;
				delay_ms(200);
				u2_printf("Cartoon_Back_1");				
				delay_ms(200);
				u2_printf("BackRes");
				delay_ms(200);
			}
			else
			{
				back_runed_arr-=arr_now;
			}						
		}		
		__HAL_TIM_CLEAR_FLAG(&TIM10_Handler, TIM_SR_CC1IF); // ����жϱ�־λ
	}
}
	else
	{
		LedAlm(300,"BackInterfere");
	}	
}

void LegUpRun(u8 dir,u8 angle)
{
	u16 arr_now;              //��ǰһ������������
	u8 len;                   //WiFi���ڽ����ַ�������
//ʵ����λ��ʵʱ��ʾ��������ǰ�˶�״̬
	static u8 k=0;            //�����k�Ŷ���ָ��
	static u8 kj;
	u8 i=0;
	u8 j=0;	
	u16 arr_send;               //��ǰһ����������ֵ
	static u8 leg_up_limit_flag;//���������е�����λ����1�����ͼ���λ��ͼƬ
	
	//�������ܣ�ֻ�������ҷ������ܸ�λ�������ȸ�λ����ܽ���������
	if((body_left_flag==0)&&(body_right_flag==0)&&(leg_down_flag==0)&&(lock_flag==1))
	{		
		if(dir==1)  //����������	
		{				   
			if(leg_angle_to_arr(angle)>leg_up_runed_arr)             //����ʱ���Ƚ�ֵΪ���м���װ��ֵ��У׼��ĵ���װ��ֵ�Ĳ�ֵ��������ֵ����Ϊ0
			{
				if(leg_up_flag==0)
				{
					leg_up_flag=1;
					delay_ms(200);					
					u2_printf("leg_up_flag==1");
					delay_ms(200);
					u2_printf("LegUpStart");
					delay_ms(200);
					u2_printf("Cartoon_Leg_Up_1");
					delay_ms(200);
				}				
				Push_Rod_Start(0);
				TIM10_Init(leg_angle_to_arr(angle)-leg_up_runed_arr,timer10_freq);  //��ʱ������=(freq1*freq_time1_1)/90mhz					
			} 
		}
		else if(dir==0)  //����������
		{			
			if(leg_up_runed_arr>0)                          //����ʱ���Ƚ�ֵΪУ׼��ĵ���װ��ֵ��0,�Ҳ���Ϊ0
			{
				if(leg_up_limit_flag==1)
				{
					leg_up_limit_flag=0;
					delay_ms(200);
					u2_printf("Cartoon_Leg_Up_8");
					delay_ms(200);
				}										
				Push_Rod_Start(1);
				TIM10_Init(leg_up_runed_arr,timer10_freq);  //��ʱ������=(freq1*freq_time1_1)/90mhz			
			}
		}
		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);			
		USART2_RX_LEN=0;				
		__HAL_TIM_CLEAR_FLAG(&TIM10_Handler, TIM_SR_CC1IF);  //����жϱ�־λ
	 	if(((leg_up_runed_arr!=leg_angle_to_arr(angle))&&(1==dir))||((0!=leg_up_runed_arr)&&(0==dir)))
		{
			while(!(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF) ) )
			{
					//ָֹͣ��
					if(UsartCheck2("LegUpUpNew","LegUpDownNew"))
					{
						u2_printf("Stop\r\n");
						break;
					}
				 arr_send=__HAL_TIM_GET_COUNTER(&TIM10_Handler);
				//�жϴ��䶯��ָ��
				if(dir==1)
				{
					j=(leg_up_runed_arr+arr_send)/(leg_angle_to_arr(leg_up_angle_max)/7);
				}
				else
				{
					j=abs(leg_up_runed_arr,arr_send)/(leg_angle_to_arr(leg_up_angle_max)/7);
				}
				k=leg_up_picture_k;
				if(	k!=j)
				{				
					kj=abs(k,j);					
					if(kj<2)
					{
						k=j;    leg_up_picture_k=k;						
						switch (k)
						{						
							case 1:	u2_printf("Cartoon_Leg_Up_2");									
									break;
							case 2:	u2_printf("Cartoon_Leg_Up_3");
									break;					
							case 3:	u2_printf("Cartoon_Leg_Up_4");
									break;					
							case 4:	u2_printf("Cartoon_Leg_Up_5");
									break;	
							case 5:	u2_printf("Cartoon_Leg_Up_6");
									break;	
							case 6:	u2_printf("Cartoon_Leg_Up_7");
									break;
						}
					}					
				}				
			}				//�ȴ���ʱʱ�䵽��ʱ�䵽����ѭ��
	
		Push_Rod_Stop();    //�Ƹ�ֹͣ
		TIM10_Stop();       //�رն�ʱ��
		//�жϸ�λ
		if((__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))&&(dir==0)) //�ж��Ƿ��ڸ�λ״̬����λ״̬��ǰ�������еĶ�ʱ������
		{
			arr_now=0;                  //��ʱ���ڸ�λ״̬����״ֵ̬����Ϊ0��
			leg_up_flag=0;
			delay_ms(200);
			u2_printf("leg_up_flag==0");
			delay_ms(200);
		}
		else
		{
			arr_now=__HAL_TIM_GET_COUNTER(&TIM10_Handler);       
			leg_up_flag=1;
		}			
		 //ͨ���������ж������ۼ�	
		if(dir==1)    //��������������У�����+
		{
			if(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))
			{
				leg_up_runed_arr=leg_angle_to_arr(angle);
				leg_up_limit_flag=1;	
				delay_ms(200);
				u2_printf("Cartoon_Leg_Up_8");
				delay_ms(200);
				u2_printf("LegUpLim");
				delay_ms(200);
			}
			 else
			{
				leg_up_runed_arr=leg_up_runed_arr+arr_now;
			}	
		}
		else     //��������������У�����-
		{
			if(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))
			{
				leg_up_runed_arr=0;	
				delay_ms(200);
				u2_printf("Cartoon_Leg_Up_1");
				delay_ms(200);
				u2_printf("LegUpRes");
				delay_ms(200);
			}
			 else
			{
				leg_up_runed_arr=leg_up_runed_arr-arr_now;
			}			
		}	
		 __HAL_TIM_CLEAR_FLAG(&TIM10_Handler, TIM_SR_CC1IF); //����жϱ�־λ	 		
	 }
   }	
	else
	{
		LedAlm(300,"LegUpInterfere");
	}
}


void LegDownRun(u8 dir,u8 angle)
{
	u16 arr_now;         //��ǰһ������������   
	u8 len;              //���յ��ַ�������
	
//ʵ����λ��ʵʱ��ʾ��������ǰ�˶�״̬
	static u8 k=0;       //���͵�K�Ŷ���ָ��
	static u8 kj=0;
	u8 i=0;
	u8 j=0;	
	u16 arr_send;        //��ǰһ������������
	static u8 leg_down_limit_flag;//���������е�����λ����1�����ͼ���λ��ͼƬ
	
	//�������ܣ�ֻ�������ҷ������ܸ�λ�������ȸ�λ����ܽ���������
	if((body_left_flag==0)&&(body_right_flag==0)&&(leg_up_flag==0)&&(lock_flag==1))
	{
		if(dir==0)   //����������
		{				
			if(leg_angle_to_arr(angle)>leg_down_runed_arr)             //����ʱ���Ƚ�ֵΪ���м���װ��ֵ��У׼��ĵ���װ��ֵ�Ĳ�ֵ��������ֵ����Ϊ0
			{
				if(leg_down_flag==0)
				{
					leg_down_flag=1;
					delay_ms(200);
					u2_printf("leg_down_flag==1");
					delay_ms(200);
					u2_printf("LegDownStart");
					delay_ms(200);
					u2_printf("Cartoon_Leg_Down_1");
					delay_ms(200);
				}							
				Push_Rod_Start(1);
				TIM10_Init(leg_angle_to_arr(angle)-leg_down_runed_arr,timer10_freq);  //��ʱ������=(freq1*freq_time1_1)/90mhz			
				leg_down_flag=1;
			}	
		}
		if(dir==1)  //����������
		{
			
			if(leg_down_runed_arr>0)     //����ʱ���Ƚ�ֵΪ���м���װ��ֵ��У׼��ĵ���װ��ֵ�Ĳ�ֵ��������ֵ����Ϊ0
			{
				if(leg_down_limit_flag==1)
				{
					leg_down_limit_flag=0;
					delay_ms(200);
					u2_printf("Cartoon_Leg_Down_20");
					delay_ms(200);
				}							
				Push_Rod_Start(0);
				TIM10_Init(leg_down_runed_arr,timer10_freq);  //��ʱ������=(freq1*freq_time1_1)/90mhz			
			}
		}
		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);			
		USART2_RX_LEN=0;	
		
		__HAL_TIM_CLEAR_FLAG(&TIM10_Handler, TIM_SR_CC1IF);    //����жϱ�־λ	

	   if(((leg_down_runed_arr!=leg_angle_to_arr(angle))&&(1==dir))||((0!=leg_down_runed_arr)&&(0==dir)))
	   {			
			while(!(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF) ) )
			{			
				
					//ָֹͣ��
					if(UsartCheck2("LegDownUpNew","LegDownDownNew"))
					{
						u2_printf("Stop\r\n");
						break;
					}			
				
				 arr_send=__HAL_TIM_GET_COUNTER(&TIM10_Handler);
				//���Ͷ���ָ��
				if(dir==1)
				{
					j=(leg_down_runed_arr+arr_send)/(leg_angle_to_arr(leg_down_angle_max)/19);
				}
				else
				{
					j=abs(leg_down_runed_arr,arr_send)/(leg_angle_to_arr(leg_down_angle_max)/19);
				}
				k=leg_down_picture_k;
				if(	k!=j)
				{				
					kj=abs(k,j);
					if(kj<2)
					{
						k=j;   leg_down_picture_k=k;
						switch (k)
						{						
							case 1:	u2_printf("Cartoon_Leg_Down_2");									
									break;
							case 2:	u2_printf("Cartoon_Leg_Down_3");
									break;					
							case 3:	u2_printf("Cartoon_Leg_Down_4");
									break;					
							case 4:	u2_printf("Cartoon_Leg_Down_5");
									break;	
							case 5:	u2_printf("Cartoon_Leg_Down_6");
									break;	
							case 6:	u2_printf("Cartoon_Leg_Down_7");
									break;
							case 7:	u2_printf("Cartoon_Leg_Down_8");
									break;
							case 8:	u2_printf("Cartoon_Leg_Down_9");
									break;						
							case 9:	u2_printf("Cartoon_Leg_Down_10");
									break;												
							case 10:u2_printf("Cartoon_Leg_Down_11");
									break;	
							case 11:u2_printf("Cartoon_Leg_Down_12");									
									break;
							case 12:u2_printf("Cartoon_Leg_Down_13");
									break;					
							case 13:u2_printf("Cartoon_Leg_Down_14");
									break;					
							case 14:u2_printf("Cartoon_Leg_Down_15");
									break;	
							case 15:u2_printf("Cartoon_Leg_Down_16");
									break;	
							case 16:u2_printf("Cartoon_Leg_Down_17");
									break;
							case 17:u2_printf("Cartoon_Leg_Down_18");
									break;
							case 18:u2_printf("Cartoon_Leg_Down_19");
									break;																		
						}
					}				
				}				 
			}				   //�ȴ���ʱʱ�䵽��ʱ�䵽����ѭ��	      
			Push_Rod_Stop();   //�Ƹ�ֹͣ
			TIM10_Stop();      //�رն�ʱ��
			//�жϸ�λ
			if((__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))&&(dir==0))
			{
				arr_now=0;                 //��ʱ���ڸ�λ״̬����״ֵ̬����Ϊ0��
				leg_down_flag=0;
				delay_ms(200);			
				u2_printf("leg_down_flag==0");
				delay_ms(200);
			}
			else
			{			
				arr_now=__HAL_TIM_GET_COUNTER(&TIM10_Handler);     
				leg_down_flag=1;
			}			
			//ͨ���������ж������ۼ�
			if(dir==1)    //��������������У�����+
			{
				if(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))
				{
					leg_down_runed_arr=leg_angle_to_arr(angle);
					leg_down_limit_flag=1;	   
					delay_ms(200);
					u2_printf("Cartoon_Leg_Down_20");
					delay_ms(200);
					u2_printf("LegDownLim");
					delay_ms(200);
				}
				 else
				{
					leg_down_runed_arr=leg_down_runed_arr+arr_now;
				}	
			}
			else		//��������������У�����-
			{
				if(__HAL_TIM_GET_FLAG(&TIM10_Handler, TIM_SR_CC1IF))
				{
					leg_down_runed_arr=0;	
					delay_ms(200);
					u2_printf("Cartoon_Leg_Down_1");
					delay_ms(200);
					u2_printf("LegDownRes");
					delay_ms(200);
				}
				 else
				{
					leg_down_runed_arr=leg_down_runed_arr-arr_now;
				}			
			}
			__HAL_TIM_CLEAR_FLAG(&TIM10_Handler, TIM_SR_CC1IF); // ����жϱ�־λ	 					
		}
	}
	else
	{
		LedAlm(300,"LegDownInterfere");
	}
}





