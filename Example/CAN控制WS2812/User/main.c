/**
  ******************************************************************************
  * @file    main.c 
  * @author  Apex yuan
  * @version V1.0.0
  * @date    2019-8-16
  * @brief   Main program body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t tTime[2] = {0}; //���ڼ�¼ʱ������


uint32_t colorArry[256] = {0x00ff00, 0x0f0000, 0x0000ff, 0x61ff00,0xffffff}; //ɫ����������Ȱ�
uint32_t color_num[2];
uint32_t last_color_num[2];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
/* ��·PWM�ư���Գ��� */  
//int main(void)
//{
//  bsp_init();
//  
//  
//  while(1)
//  {
//   
////    //ws281x_theaterChase(EAR, EAR_PIXEL_NUM, 0x000055, 10);
////    delay_ms(300);    
//    //ws281x_colorWipe(EYE,EYE_PIXEL_NUM,0xaa0000, 200);
//    //ws281x_theaterChase(EYE, EYE_PIXEL_NUM, 0x550000, 200); 
//    //ws281x_theaterChase(EYE, EYE_PIXEL_NUM,0x550000,30);
//    //ws281x_theaterChase(EAR, EAR_PIXEL_NUM,0x000055,30);
//    //delay_ms(1000);
////    
//    for(uint8_t i = 0; i < EYE_PIXEL_NUM; ++i)
//    {
//      ws281x_setPixelColor(EYE,i,0x0000aa);
//    }
//    
//    for(uint8_t i = 0; i < EAR_PIXEL_NUM; ++i)
//    {
//      ws281x_setPixelColor(EAR,i,0xaa0000);
//    }
//    ws281x_show(0);
//    //ws281x_show(1);
//    
//    delay_ms(300);
//    ws281x_closeAll(0);
////    delay_ms(10);
//    //ws281x_closeAll(1);
//    delay_ms(300);
//  }
//}
  
/* �����������еķ�0��Сֵ��ȫΪ���򷵻�0 */  
int minimum(int num1,int num2,int num3)
{ 
    if(num1 == 0)
    {
      if(num2 == 0)
      {
        if(num3 == 0)
          return 0;
        else
          return num3;
      }
      else
      {
        if(num3 == 0)
          return num2;
        else
          return (num2 < num3) ? num2 : num3;
      }
    }
    else
    {
      if(num2 == 0)
      {
        if(num3 == 0)
          return num1;
        else
          return (num1 < num3) ? num1 : num3;
      }
      else
      {
        if(num3 == 0)
          return (num1 < num2) ? num1 : num2;
        else
        {
          int min = (num1 < num2) ? num1 : num2;
          min = (min < num3) ? min : num3;

          return min;
        }
      }
    }

    
}  
  
  
int main(void)
{
  bsp_init();
  
  
  printf("\r\nStart:");
  can_tx_msg.StdId = 0x0;
  can_tx_msg.ExtId = 0xFFFF;
  can_tx_msg.IDE = CAN_ID_EXT;
  can_tx_msg.RTR = CAN_RTR_DATA;
  can_tx_msg.DLC = 8;
//  for(uint8_t i = 0; i < 8; ++i)
//  {
//    can_tx_msg.Data[i] = i;
//  }
  can_tx_msg.Data[0] = 0x03;
  can_tx_msg.Data[1] = 0x04;
  can_tx_msg.Data[2] = 0x00;
  can_tx_msg.Data[3] = 0x04;
  can_tx_msg.Data[4] = 0xff;
  can_tx_msg.Data[5] = 0x00;
  can_tx_msg.Data[6] = 0x00;
  can_tx_msg.Data[7] = 0xff;
  
  
  CAN_Transmit(CAN1, &can_tx_msg);
  /* Wait until one of the mailboxes is empty */
  while((CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP0) !=RESET) || \
        (CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP1) !=RESET) || \
        (CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP2) !=RESET));
  printf("\r\n�������");
 
  if(can_rx_flag == 1)
  {
    printf("\r\n%#x",can_rx_msg.ExtId);
    printf("\r\n%d",can_rx_msg.IDE);
    printf("\r\n%d",can_rx_msg.RTR);
    printf("\r\n%d",can_rx_msg.FMI);
    for(uint8_t i = 0; i < 8; ++i)
    {
      printf("\r\n%d",can_rx_msg.Data[i]);
    }
    can_rx_flag = 0;
  }
  
  /* Infinite loop */
  while (1)
  {
    /*���ջ������������������㣬Ϊ�´ν���������׼��*/
    if(usart1_rx_cnt >= 8)
    {
      for(uint8_t i = 0; i < 8; ++i)
      {
        can_tx_msg.Data[i] = usart1_rx_buffer[i];
      }
      CAN_Transmit(CAN1, &can_tx_msg);
      /* Wait until one of the mailboxes is empty */
      while((CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP0) !=RESET) || \
            (CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP1) !=RESET) || \
            (CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP2) !=RESET));
      
      usart1_rx_cnt = 0; //��ռ���
    } 

    if(can_rx_flag == 1)
    {
      printf("\r\n%#x",can_rx_msg.ExtId);
      printf("\r\n%d",can_rx_msg.IDE);
      printf("\r\n%d",can_rx_msg.RTR);
      printf("\r\n%d",can_rx_msg.FMI);
      for(uint8_t i = 0; i < 8; ++i)
      {
        printf("\r\n%d",can_rx_msg.Data[i]);
      }
      can_rx_flag = 0;
    }
    //delay_ms(200);
    /*�������ݴ���*/
    if(can_rx_msg.ExtId == 0xFFFF && can_rx_msg.DLC == 8) 
    {
      /* ��ȡɫ�� */
      color_num[EYE] = can_rx_msg.Data[1];
      color_num[EAR] = can_rx_msg.Data[3];
      
      /* EYE process */
      switch(can_rx_msg.Data[0])
      {
        case 0x00:
          if(ws281x_status[EYE] == WS281x_ON)
          {
            ws281x_closeAll(EYE);
          }
          break;
        case 0x01:
          for(uint16_t i = 0; i < EYE_PIXEL_NUM; ++i)
          {
            ws281x_setPixelColor(EYE, i, colorArry[color_num[EYE]]);
          }
          ws281x_show(EYE);
          ws281x_status[EYE] = WS281x_ON;
          break;
        case 0x02:
          if(millis() - tTime[EYE] >= 500) //500ms��ʱ
          {
            if(ws281x_status[EYE] == WS281x_OFF) //��ǰ�Ƶ�״̬Ϊ��
            {
              for(uint16_t i = 0; i < EYE_PIXEL_NUM; ++i)
              {
                ws281x_setPixelColor(EYE, i, colorArry[color_num[EYE]]);
              }
              ws281x_show(EYE);
              ws281x_status[EYE] = WS281x_ON;
            }
            else
            {
              ws281x_closeAll(EYE);
              ws281x_status[EYE] = WS281x_OFF;
            }
            tTime[EYE] = millis();
          }
          break;
          /* ����˸���κ��� */
        case 0x03:
        {
          static uint8_t count = 0;
          
          if(count <= 2) //��˸
          {
            if(millis() - tTime[EYE] >= 500) //500ms��ʱ
            {
              if(ws281x_status[EYE] == WS281x_OFF) //��ǰ�Ƶ�״̬Ϊ��
              {
                for(uint16_t i = 0; i < EYE_PIXEL_NUM; ++i)
                {
                  ws281x_setPixelColor(EYE, i, colorArry[color_num[EYE]]);
                }
                ws281x_show(EYE);
                ws281x_status[EYE] = WS281x_ON;
              }
              else
              {
                ws281x_closeAll(EYE);
                ws281x_status[EYE] = WS281x_OFF;
              }
              tTime[EYE] = millis();
              count++;
            }
            
          }
          else //����
          {
            for(uint16_t i = 0; i < EYE_PIXEL_NUM; ++i)
            {
              ws281x_setPixelColor(EYE, i, colorArry[color_num[EYE]]);
            }
            ws281x_show(EYE);
            ws281x_status[EYE] = WS281x_ON;
          }
          break;
        }
      
          /* ����ʵ�ֺ�ɫ�����Ƶ�Ч�� */
        case 0x04:
        {
          static uint8_t r = 0;
          static uint8_t flag = 0;
          if(millis() - tTime[EYE] >= 5)
          {
            for(uint16_t j = 0; j < EYE_PIXEL_NUM; ++j)
            {
              ws281x_setPixelRGB(EYE, j, r, 0, 0);
              
            }
            ws281x_show(EYE);
            if(flag == 0)
            {
              r+=2;
              if(r >= 254)
            {
              flag = 1;
              //r = 0;
            }
            }
            
            if(flag == 1)
            {
              r-=2;
              if(r <= 0)
            {
              flag = 0;
              //r = 0;
            }
            }
            
            
            tTime[EYE] = millis();
          }
          break;
        }  
        /* ʵ��������ɫ�ĺ�����Ч���������һ������ */
        case 0x05:
        {
          static uint8_t count = 0;
          static uint8_t flag = 0;
          static uint8_t r, g, b;
          uint8_t l, m, n;
          float min;
          
          min = (uint8_t)minimum(colorArry[color_num[EYE]] >> 8, colorArry[color_num[EYE]] >> 16, colorArry[color_num[EYE]]);
          l = (uint8_t)(colorArry[color_num[EYE]] >> 8) / min;
          m = (uint8_t)(colorArry[color_num[EYE]] >> 16) / min;
          n = (uint8_t)(colorArry[color_num[EYE]] ) / min;
          //r = g = b =0; //���rgb��ʱ������Ϊ����Ӧ��
        
          if(millis() - tTime[EYE] >= 5)
          { 

            if(flag == 0)
            {
              count++;
              r += l;
              g += m;
              b += n;
              if(count >= min)
              {
                flag = 1;
              }
            }
            if(flag == 1)
            {
              count--;
              r -= l;
              g -= m;
              b -= n;
              if(count <= 0)
              {
                flag = 0;
              }
            }
            for(uint16_t j = 0; j < EYE_PIXEL_NUM; ++j)
            {
              ws281x_setPixelRGB(EYE, j, r, g, b);            
            }
            ws281x_show(EYE);
            tTime[EYE] = millis();
          }
        
          break;
        }
        default:
          break;
      }
        
      /* EAR process */
      switch(can_rx_msg.Data[2])
      {
        case 0x00:
          if(ws281x_status[EAR] == WS281x_ON)
          {
            ws281x_closeAll(EAR);
          }
          break;
        case 0x01:
          for(uint16_t i = 0; i < EAR_PIXEL_NUM; ++i)
          {
            ws281x_setPixelColor(EAR, i, colorArry[color_num[EAR]]);
          }
          ws281x_show(EAR);
          ws281x_status[EAR] = WS281x_ON;
          break;
        case 0x02:
          if(millis() - tTime[EAR] >= 500) //500ms��ʱ
          {
            if(ws281x_status[EAR] == WS281x_OFF) //��ǰ�Ƶ�״̬Ϊ��
            {
              for(uint16_t i = 0; i < EAR_PIXEL_NUM; ++i)
              {
                ws281x_setPixelColor(EAR, i, colorArry[color_num[EAR]]);
              }
              ws281x_show(EAR);
              ws281x_status[EAR] = WS281x_ON;
            }
            else
            {
              ws281x_closeAll(EAR);
              ws281x_status[EAR] = WS281x_OFF;
            }
            tTime[EAR] = millis();
          }
          break;
          
            /* ����˸���κ��� */
        case 0x03:
        {
          static uint8_t count = 0;
          
          if(count <= 2) //��˸
          {
            if(millis() - tTime[EAR] >= 500) //500ms��ʱ
            {
              if(ws281x_status[EAR] == WS281x_OFF) //��ǰ�Ƶ�״̬Ϊ��
              {
                for(uint16_t i = 0; i < EAR_PIXEL_NUM; ++i)
                {
                  ws281x_setPixelColor(EAR, i, colorArry[color_num[EAR]]);
                }
                ws281x_show(EAR);
                ws281x_status[EAR] = WS281x_ON;
              }
              else
              {
                ws281x_closeAll(EAR);
                ws281x_status[EAR] = WS281x_OFF;
              }
              tTime[EAR] = millis();
              count++;
            }
           
          }
          else //����
          {
            for(uint16_t i = 0; i < EAR_PIXEL_NUM; ++i)
            {
              ws281x_setPixelColor(EAR, i, colorArry[color_num[EAR]]);
            }
            ws281x_show(EAR);
            ws281x_status[EAR] = WS281x_ON;
          }
          break;
        }
          
        /* ʵ��������ɫ�ĺ�����Ч���������һ������ */
        case 0x05:
        {
          static uint8_t count = 0;
          static uint8_t flag = 0;
          static uint8_t r, g, b;
          uint8_t l, m, n;
          float min;
          
          min = (uint8_t)minimum(colorArry[color_num[EAR]] >> 8, colorArry[color_num[EAR]] >> 16, colorArry[color_num[EAR]]);
          l = (uint8_t)(colorArry[color_num[EAR]] >> 8) / min;
          m = (uint8_t)(colorArry[color_num[EAR]] >> 16) / min;
          n = (uint8_t)(colorArry[color_num[EAR]] ) / min;
          //r = g = b =0; //���rgb��ʱ������Ϊ����Ӧ��
        
          if(millis() - tTime[EAR] >= 5)
          { 

            if(flag == 0)
            {
              count++;
              r += l;
              g += m;
              b += n;
              if(count >= min)
              {
                flag = 1;
              }
            }
            if(flag == 1)
            {
              count--;
              r -= l;
              g -= m;
              b -= n;
              if(count <= 0)
              {
                flag = 0;
              }
            }
            for(uint16_t j = 0; j < EAR_PIXEL_NUM; ++j)
            {
              ws281x_setPixelRGB(EAR, j, r, g, b);            
            }
            ws281x_show(EAR);
            tTime[EAR] = millis();
          }
        
          break;
        }
          
        default:
          break;
      }
    }
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
