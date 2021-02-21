/**
  *************************************************************************
  * @file    main.c
  * @author  HYX.Huaqian
  * @version V1.0.0
  * @date    2021/02/17
  * @brief
  *************************************************************************
  * @attention
  *
  * <h2><center>&Copyright(c) 2014-2021 Xiamen HuaQian Studio</center></h2>
  *
  *************************************************************************
*/

/*************************     ϵͳͷ�ļ�����     *************************/
#include "stm8l15x_clk.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_usart.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_itc.h"
#include "stm8l15x_flash.h"
#include "common_cfg.h"
/*************************     ˽��ͷ�ļ�����     *************************/


/*************************      ���������ռ�      *************************/


/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
extern "C" {
#endif 
#endif

	/*************************        ȫ��ö��        *************************/

	/*************************       ȫ�ֺ궨��       *************************/

	/*************************     ȫ�ֽṹ�嶨��     *************************/

	/*************************      ȫ�ֱ�������      *************************/

	/*************************        ��������        *************************/

	/*************************        ����ʵ��        *************************/

	/**
	  * ***********************************************************************
	  * @brief	��ʼ���ܽ�
	  *
	  *	@param  :
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
#if 1
	/* ���Ե�Ƭ���Ƿ��������� */
	void p_init_gpio(void)
	{
		/* ��ʼ��PA��Ϊ���١����졢�͵�ƽ */
		GPIO_Init(LED_SHAKE_PORT, LED_SHAKE_PIN, GPIO_Mode_Out_PP_High_Fast);
		GPIO_Init(LED_REGISTER_PORT, LED_REGISTER_PIN, GPIO_Mode_Out_PP_High_Fast);
		GPIO_Init(LED_GPS_PORT, LED_GPS_PIN, GPIO_Mode_Out_PP_High_Fast);
		GPIO_Init(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_In_PU_No_IT);    // ���ؽ�����Ϊ��������
		GPIO_Init(GPS_EN_PORT, GPS_EN_PIN, GPIO_Mode_Out_PP_High_Slow);    // ����GPS LNA�Ŵ����ߵ�ƽ
	}
#endif // 1

	/**
	  * ***********************************************************************
	  * @brief	��ʼ��ϵͳʱ��
	  *
	  *	@param  :
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_clk(void)
	{
		/* CLK�Ĵ�����λ */
		CLK_DeInit();

		/* ��Ƶ��1,����Ƶ */
		CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

		/* ʹ��ʱ���л� */
		CLK_SYSCLKSourceSwitchCmd(ENABLE);

		/* ʹ���ڲ�����ʱ��HSI */
		CLK_HSICmd(ENABLE);

		/* ��ʱ���л�ΪHSI */
		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);

		/* �ȴ�LSIʱ��Դ�ȶ� */
		while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ʼ����ʱ��
	  *
	  *	@param  ��������: ��������˼��
	  *
	  * @retval uint8_t:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_timer(void)
	{
		/* ʹ�ܶ�ʱ��ʱ�� */
		CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
		CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
		CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
		
		/*
		���ö�ʱ��2ʱ��ģʽ(����)
		16M/128/1250
		��ʱ���ж�10ms
		*/
		TIM2_TimeBaseInit(TIM2_Prescaler_128, TIM2_CounterMode_Up, (uint16_t)1249);
		TIM2_ClearFlag(TIM2_FLAG_Update);    // �������жϱ�־λ
		TIM2_ITConfig(TIM2_IT_Update, ENABLE);    // ʹ������ж�

		/*
		���ö�ʱ��3ʱ��ģʽ(��ʱ��)
		16M/128/1250
		��ʱ���ж�10ms
		*/
		TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, (uint16_t)1249);
		TIM3_ClearFlag(TIM3_FLAG_Update);    // �������жϱ�־λ
		TIM3_ITConfig(TIM3_IT_Update, ENABLE);    // ʹ������ж�

		/*
		���ö�ʱ��4ʱ��ģʽ(LED)
		16M/1024/(155+1) = 100Hz
		�ж�һ��10ms
		*/
		TIM4_TimeBaseInit(TIM4_Prescaler_1024, (uint16_t)155);
		TIM4_ClearFlag(TIM4_FLAG_Update);    // �������жϱ�־λ
		TIM4_ITConfig(TIM4_IT_Update, ENABLE);    // ʹ������ж�

		/* �ر�ȫ���ж� */
		disableInterrupts();

		/* �����ж����ȼ� �������ȼ���Ҫ�޸� */
		ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_IRQn, ITC_PriorityLevel_2);
		ITC_SetSoftwarePriority(TIM3_UPD_OVF_TRG_BRK_IRQn, ITC_PriorityLevel_1);
		ITC_SetSoftwarePriority(TIM4_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_3);

		/* ��ȫ���ж� */
		enableInterrupts();

		/* ʹ�ܶ�ʱ�� */
		TIM4_Cmd(ENABLE);
		TIM3_Cmd(ENABLE);
		TIM2_Cmd(ENABLE);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ʼ��EEPROM
	  *
	  *	@param  void: 
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_eeprom(void)
	{
		/*��ʼ��EEPROM*/
		FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
		FLASH_WaitForLastOperation(FLASH_MemType_Data);
		FLASH_Unlock(FLASH_MemType_Data);
		while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
		FLASH_WaitForLastOperation(FLASH_MemType_Data);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ʼ��LED
	  *
	  *	@param  ��������: ��������˼��
	  *
	  * @retval uint8_t:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_led(void)
	{
		/* ȫ��Ϩ�� */
		GPIO_Init(LED_SHAKE_PORT, LED_SHAKE_PIN, GPIO_Mode_Out_PP_High_Slow);
		GPIO_Init(LED_REGISTER_PORT, LED_REGISTER_PIN, GPIO_Mode_Out_PP_High_Slow);
		GPIO_Init(LED_GPS_PORT, LED_GPS_PIN, GPIO_Mode_Out_PP_High_Slow);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ʼ�����Ź�
	  *
	  *	@param  ��������: ��������˼��
	  *
	  * @retval uint8_t:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_IWDG(void)
	{
#ifdef USE_IWDG
		/* ��ȡOption byte �ж������Ź����õ����� */
		if (FLASH_ReadByte(0x0004808) != 0x02)    //�жϿ��Ź�������״̬�Ƿ�ֹͣ
		{
			FLASH->CR2 = 0x80;    //����OPT����ѡ���ֽڽ���д����ʹ��
			FLASH_Unlock(FLASH_MemType_Data);    //flash �Ƚ���
			/* Wait until Data EEPROM area unlocked flag is set*/
			while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);

			FLASH_ProgramByte(0x0004808, 0x02);    //�޸�Option byte�е�OPT[3]
			FLASH_WaitForLastOperation(FLASH_MemType_Data);
			FLASH_Lock(FLASH_MemType_Data);    //flash ��������
			FLASH->CR2 = 0x00;    //����OPT����ѡ���ֽڽ���д�����ָ���ֹ
		}
		/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
		IWDG_Enable();

		/* IWDG timeout equal to 214 ms (the timeout may varies due to LSI frequency
		dispersion) */
		/* Enable write access to IWDG_PR and IWDG_RLR registers */
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

		/* IWDG configuration: IWDG is clocked by LSI = 38KHz */
		IWDG_SetPrescaler(IWDG_Prescaler_256);

		/* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
		/* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI
						= (254 + 1) * 32 / 38 000
					    = 214.7 ms */
		IWDG_SetReload(254);

		/* Reload IWDG counter */
		IWDG_RELOAD();

#endif // USE_IWDG

	}

	/**
	  * ***********************************************************************
	  * @brief	��ʼ��BC20����
	  *
	  *	@param  BaudRate: ������
	  *
	  * @retval uint8_t:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_BC20_uart(uint32_t BaudRate)
	{
		/* ��ʼ��RXDΪ�������룬TXDΪ��©��� */
		GPIO_Init(BC20_RXD_PORT, BC20_RXD_PIN, GPIO_Mode_In_FL_No_IT);    // ��������
		GPIO_Init(BC20_TXD_PORT, BC20_TXD_PIN, GPIO_Mode_Out_OD_HiZ_Fast);    // ��©���

		/* �򿪴���1ʱ�� peripheral(��Χ��) */
		CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);

		/* ��ʼ������1 */
		USART_Init(USART1, BaudRate, USART_WordLength_8b, USART_StopBits_1, \
			USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));

		/* �ر�ȫ���ж� */
		disableInterrupts();

		/* ��������ж����ȼ� */
		ITC_SetSoftwarePriority(USART1_RX_IRQn, ITC_PriorityLevel_2);

		/* ��ȫ���ж� */
		enableInterrupts();

		/* ʹ�ܴ���1�����жϣ� �رտ��кͷ����ж�*/
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

		/* ʹ�ܴ���1 */
		USART_Cmd(USART1, ENABLE);

	}

	/**
	  * ***********************************************************************
	  * @brief	�ж��豸�Ƿ���Ҫ��ʽ��
	  *
	  *	@param  :
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_init_judge_reset(void)
	{
		uint8_t     rx_status = 1;
		uint8_t     tx_status = 1;
		uint8_t     i = 0, j = 0;

		GPIO_Init(BC20_RXD_PORT, BC20_RXD_PIN, GPIO_Mode_Out_PP_High_Fast);    // ��������Ϊ��
		GPIO_Init(BC20_TXD_PORT, BC20_TXD_PIN, GPIO_Mode_In_FL_No_IT);    // ��������Ϊ����
		GPIO_SetBits(BC20_RXD_PORT, BC20_RXD_PIN);

		delay_10ms(50);

		if (GPIO_ReadInputDataBit(BC20_TXD_PORT,BC20_TXD_PIN))
		{
			/* �رտ��ƵƵĶ�ʱ�� */
			TIM4_Cmd(DISABLE);

			GPIO_ResetBits(LED_SHAKE_PORT, LED_SHAKE_PIN);    //�����𶯵�
			GPIO_ResetBits(LED_REGISTER_PORT, LED_REGISTER_PIN);    //����NB������
			GPIO_ResetBits(LED_GPS_PORT, LED_GPS_PIN);    //����GPS״̬��

			for (i = 0;i < 30;++i) 
			{
				if (rx_status)
				{
					GPIO_ResetBits(BC20_RXD_PORT, BC20_RXD_PIN);
					rx_status = 0;
				}
				else
				{
					GPIO_SetBits(BC20_RXD_PORT, BC20_RXD_PIN);
					rx_status = 1;
				}
				
				delay_10ms(10);

				if (GPIO_ReadInputDataBit(BC20_TXD_PORT,BC20_TXD_PIN))
				{
					tx_status = 1;
				}
				else
				{
					tx_status = 0;
				}

				if (rx_status == tx_status) 
				{
					++j;
				}

				/* ��״̬ */
				if (i > 10 && i < 20) 
				{
					GPIO_ResetBits(LED_REGISTER_PORT, LED_REGISTER_PIN);    //����NB������
				}
				else if (i > 20) 
				{
					GPIO_ResetBits(LED_SHAKE_PORT, LED_SHAKE_PIN);    //�����𶯵�
				}
			}

			if (j >= 25) 
			{
				GPIO_ResetBits(LED_GPS_PORT, LED_GPS_PIN);    //����GPS״̬��
				p_ccfg_reset();    // ��ʽ��
				p_proto_gen_server_key();    //���ɷ�������Կ
			}

			GPIO_SetBits(LED_SHAKE_PORT, LED_SHAKE_PIN);    //�����𶯵�
			GPIO_SetBits(LED_REGISTER_PORT, LED_REGISTER_PIN);    //����NB������
			GPIO_SetBits(LED_GPS_PORT, LED_GPS_PIN);    //����GPS״̬��

			TIM4_Cmd(ENABLE);

		}
	}


	/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
}
#endif 
#endif