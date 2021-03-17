/**
  *************************************************************************
  * @file    common_cfg.c
  * @author  HYX.Huaqian
  * @version V1.0.0
  * @date    2021/02/18 - 02/19
  * @brief
  *************************************************************************
  * @attention
  *
  * <h2><center>&Copyright(c) 2014-2021 Xiamen HuaQian Studio</center></h2>
  *
  *************************************************************************
*/

/*************************     ϵͳͷ�ļ�����     *************************/
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
	st_ccfg_config       g_ccfg_config = { 0 };
	st_run_parameter     g_run_paramter = { 0 };
	st_buff              g_buff = { 0 };
	uint32_t __IO        g_vu32_1s_timer = 0;
	uint32_t __IO        g_vu32_10ms_timer = 0;
	uint32_t             g_bc20_init_timeout = 0;    // bc20ģ���ʼ��ʧ�ܵĳ�ʱ���
	uint32_t             g_bc20_net_timeout = 0;     // bc20ģ���޷����ӷ������ĳ�ʱ���
	LED_STATUS_TypeDef   led_status;

	/*************************        ��������        *************************/

	/*************************        ����ʵ��        *************************/

	/**
	  * ***********************************************************************
	  * @brief	��ʱ����(10ms)
	  *
	  *	@param  ms: ��λ 10ms
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void delay_10ms(uint32_t ms)
	{
		ms = GET_10MS_TIMER() + ms;
		while (GET_10MS_TIMER() < ms);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ʱ����(1s)
	  *
	  *	@param  s: ��λ 1s
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void delay_s(uint8_t s)
	{
		s = GET_1S_TIMER + s;
		while (GET_1S_TIMER() < s);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ս��ջ�����
	  *
	  *	@param  R_or_T: ѡ����շ��ͻ��������߽��ܻ�����
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_clear_recv_buff(void)
	{
		g_buff.m_flg_recv_lock = FALSE;
		g_buff.m_recv_index = 0x00;
		memset(g_buff.m_recv_buff, 0x00, sizeof(uint8_t) * MAX_RECV_BUFF_SIZE);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ս��ջ�����
	  *
	  *	@param  R_or_T: ѡ����շ��ͻ��������߽��ܻ�����
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_clear_send_buff(void)
	{
		g_buff.m_flg_send_lock = FALSE;
		g_buff.m_send_index = 0x00;
		memset(g_buff.m_send_buff, 0x00, sizeof(uint8_t) * MAX_SEND_BUFF_SIZE);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ʽ���豸(�ָ�Ĭ������)
	  *
	  *	@param  void: 
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_ccfg_reset(void)
	{
		softuart_printf("CCFG RESET\r\n");
		/* ��յ绰�б� */
		for (uint8_t i = 0;i < 8;i++) 
		{
			memset(g_ccfg_config.m_tel_list[i], 0x00, MAX_TEL_NUMBER_SIZE + 1);
			g_ccfg_config.m_tel_list[i][0] = '#';
		}

		/* ��������APNĬ������Ϊ CTNB*/
		memset(g_ccfg_config.m_dev_APN, 0x00, sizeof(g_ccfg_config.m_dev_APN));
		memcpy(g_ccfg_config.m_dev_APN, STR(DEFAULT_DEV_APN), STRLEN(DEFAULT_DEV_APN));

		memset(g_ccfg_config.m_dev_guid, 0x00, sizeof(g_ccfg_config.m_dev_guid));
		memcpy(g_ccfg_config.m_dev_guid, STR(DEFAULT_DEV_GUID), STRLEN(DEFAULT_DEV_GUID));

		memset(g_ccfg_config.m_dev_iden_id, 0x00, sizeof(g_ccfg_config.m_dev_iden_id));
		memcpy(g_ccfg_config.m_dev_iden_id, STR(DEFAULT_DEV_IDEN_ID), STRLEN(DEFAULT_DEV_IDEN_ID));

		memset(g_ccfg_config.m_dev_pwd, 0x00, sizeof(g_ccfg_config.m_dev_pwd));
		memcpy(g_ccfg_config.m_dev_pwd, STR(DEFAULT_DEV_PWD), STRLEN(DEFAULT_DEV_PWD));

		/* Ĭ�Ϸ����� */
		g_ccfg_config.m_ser_ip[0] = IP_1;
		g_ccfg_config.m_ser_ip[1] = IP_2;
		g_ccfg_config.m_ser_ip[2] = IP_3;
		g_ccfg_config.m_ser_ip[3] = IP_4;
		g_ccfg_config.m_ser_port = PORT;

		g_ccfg_config.m_en_lto5_rep = DEFAULT_1TO_REP;
		g_ccfg_config.m_en_gps_rep = DEFAULT_GPS_REP;
		g_ccfg_config.m_en_tim_rep = DEFAULT_TIME_REP;                       // ʹ�ܶ�ʱ�ϱ�
		g_ccfg_config.m_en_pwr_off_delay = DEFAULT_PWR_0FF_DELAY;            // ʹ����ʱ�ػ�
		g_ccfg_config.m_en_vol_check = DEFAULT_VOL_CHECK;                    // ʹ�ܵ�ѹ���
		g_ccfg_config.m_en_sw1 = DEFAULT_SWITCH_1;                           // ʹ��ˮ������1
		g_ccfg_config.m_en_sw2 = DEFAULT_SWITCH_2;                           // ʹ��ˮ������2

		/* Ĭ����ֵ���� *//*���ڵ��籣��ĸ������ò���*/
		g_ccfg_config.m_tim_pwr_delay = DEFAULT_TIM_PWR_DELAY;               // �ػ���ʱʱ�� 30����*60��=1800
		g_ccfg_config.m_tim_sleep = DEFAULT_TIM_SLEEP;                       // ��ʱ����ʱ����,30��*24Сʱ=720
		g_ccfg_config.m_tim_gps_rep = DEFAULT_TIM_GPS_REP;                   // �ϱ�GPS���
		g_ccfg_config.m_tim_heartbeat = DEFAULT_TIM_HEARTBEAT;               // �ޱ����ϱ����������60s
		g_ccfg_config.m_tim_alm_continue = DEFAULT_TIM_ALM_CONTINUE;         // ��������������ϱ�ʱ�� 60��*9����=540
		g_ccfg_config.m_tim_alm_interval = DEFAULT_TIM_ALM_INTERAL;          // GPS�������
		g_ccfg_config.m_cnt_sw_trigger = DEFAULT_CNT_SW_TRIGGER;             // ����״̬�´���������ˮ�����ش�������
		g_ccfg_config.m_tim_no_signal_delay = DEFAULT_TIM_NO_SIGNAL_DELAY;   // 2017-2-18Ĭ���������ź���ʱ����ʱ��Ϊ0

		/* EEPROM������Ч */
		g_ccfg_config.m_cfg_verify = 0x12341234;

		/* Ĭ������ *//*���ڳ�������ʱ�ĸ������ò���*/
		g_run_paramter.m_tim_pwr_off_delay = DEFAULT_TIM_PWR_DELAY * 60;
		g_run_paramter.m_tim_gps_rep = DEFAULT_GPS_REP * 60;
		g_run_paramter.m_tim_heardbeat = DEFAULT_TIM_HEARTBEAT;
		g_run_paramter.m_tim_alm_continue = DEFAULT_TIM_ALM_CONTINUE * 60;

		/* ȫ��д�� */
		p_ccfg_write_config((uint8_t*)&g_ccfg_config, sizeof(st_ccfg_config));

		/* ���ɼ������ID */
		p_proto_gen_server_key();
	}

	/**
	  * ***********************************************************************
	  * @brief	��������
	  *
	  *	@param  void: 
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_ccfg_load(void)
	{
		/* ����������� */
		memset(&g_ccfg_config, 0x00, sizeof(g_ccfg_config));

		/* ����ȫ������ */
		p_ccfg_read_config((uint8_t*)&g_ccfg_config, sizeof(st_ccfg_config));

		/* ��������Ƿ����� */
		if (g_ccfg_config.m_cfg_verify != 0x12341234) 
		{
			softuart_printf("load data is not nomal\r\n");
			p_ccfg_reset();    // ���ݲ�����������Ĭ������
		}

		/* ������úϷ��� */
		for (uint8_t i = 0;i < MAX_TEL_LIST;i++) 
		{
			if (g_ccfg_config.m_tel_list[i][0] == 0x00) 
			{
				g_ccfg_config.m_tel_list[i][0] = '#';
				g_ccfg_config.m_tel_list[i][1] = 0x00;
			}
		}

		/*������������豸���*/
		if (g_ccfg_config.m_dev_guid[0] == 0x00) 
		{
			memcpy(g_ccfg_config.m_dev_guid, STR(DEFAULT_DEV_GUID), STRLEN(DEFAULT_DEV_GUID));
		}

		if (g_ccfg_config.m_dev_pwd[0] == 0x00) 
		{
			memcpy(g_ccfg_config.m_dev_pwd, STR(DEFAULT_DEV_PWD), STRLEN(DEFAULT_DEV_PWD));
		}

		if (g_ccfg_config.m_dev_iden_id[0] = 0x00) 
		{
			memcpy(g_ccfg_config.m_dev_iden_id, STR(DEFAULT_DEV_IDEN_ID), STRLEN(DEFAULT_DEV_IDEN_ID));
		}

		/* ���һ����,����Ĭ�ϰ�Сʱ���ͻ�Ҫ�� */
		if (g_ccfg_config.m_tim_gps_rep < MIN_TIM_GPS_REP)
		{
			g_ccfg_config.m_tim_gps_rep = DEFAULT_TIM_GPS_REP;
		}

		/* ���񶯱���״̬����ʱ�ػ�ʱ�䣬���һ����,����Ĭ�ϰ�Сʱ���ͻ�Ҫ�� */
		if (g_ccfg_config.m_tim_pwr_delay < MIN_TIM_PWR_DELAY) 
		{
			g_ccfg_config.m_tim_pwr_delay = DEFAULT_TIM_PWR_DELAY;
		}

		/* ... *//*Ĭ������״̬�¶�ʱ�ϱ���ʱ��  30��*24Сʱ=720*/
		if ((g_ccfg_config.m_tim_sleep > MAX_TIM_SLEEP) || (g_ccfg_config.m_tim_sleep == 0)) 
		{
			g_ccfg_config.m_tim_sleep = DEFAULT_TIM_SLEEP;
		}

		/* ����һ�Σ�����Ĭ��3�� */ /*����״̬�´���������ˮ�����ش�������*/
		if ((g_ccfg_config.m_cnt_sw_trigger < 1) || (g_ccfg_config.m_cnt_sw_trigger > 9)) 
		{
			g_ccfg_config.m_cnt_sw_trigger = DEFAULT_CNT_SW_TRIGGER;
		}

		/* ���һ���ӣ�����һ���� *//*û�б���״̬���������ļ��*/
		if (g_ccfg_config.m_tim_heartbeat < 60) 
		{
			g_ccfg_config.m_tim_heartbeat = DEFAULT_TIM_HEARTBEAT;
		}

		/* ���һ���ӣ�����9���� */ /*��������������ϱ�ʱ��*/
		if (g_ccfg_config.m_tim_alm_continue < MIN_TIM_ALM_CONTINUE) 
		{
			g_ccfg_config.m_tim_alm_continue = DEFAULT_TIM_ALM_CONTINUE;
		}

		/* ���15�룬����15�� */ /*GPS�������*/
		if (g_ccfg_config.m_tim_alm_interval < 15) 
		{
			g_ccfg_config.m_tim_alm_interval = DEFAULT_TIM_ALM_INTERAL;
		}

		/* ���ɼ������ID */
		p_proto_gen_server_key();

		/* �����ʱʱ�� */
		g_run_paramter.m_tim_gps_rep = g_ccfg_config.m_tim_gps_rep;
		g_run_paramter.m_tim_pwr_off_delay = g_ccfg_config.m_tim_pwr_delay * 60;
		g_run_paramter.m_tim_heardbeat = g_ccfg_config.m_tim_heartbeat;
		g_run_paramter.m_tim_alm_continue = g_ccfg_config.m_tim_alm_continue * 60;

		/* ����������� */
		p_task_clear();
	}

	/**
	  * ***********************************************************************
	  * @brief	ɾ��ĳһ�鱨���绰
	  *
	  *	@param  x: �ڼ��鱨���绰
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_ccfg_del_tel(uint8_t x)
	{
		if (x <= 7)     // 0 ~ 7 һ��8��
		{
			/* ������� */
			memset(g_ccfg_config.m_tel_list[x], 0x00, sizeof(g_ccfg_config.m_tel_list[0]));
			g_ccfg_config.m_tel_list[x][0] = '#';

			p_ccfg_write_config((uint8_t*)g_ccfg_config.m_tel_list[x], sizeof(g_ccfg_config.m_tel_list[0]));
		}
	}

	/**
	  * ***********************************************************************
	  * @brief	д������
	  *
	  *	@param  uint8_t *cfg_ptr : g_ccfg_config������Ԫ�ص�ָ��
	  * @param  uint16_t size    : g_ccfg_config������Ԫ�صĴ�С
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_ccfg_write_config(uint8_t* cfg_ptr, uint16_t size) 
	{
		uint16_t addr = cfg_ptr - (uint8_t*)&g_ccfg_config + EEPROM_BASE_Addr;

		FLASH_Unlock(FLASH_MemType_Data);

		for (uint16_t i = 0;i < size;i++) 
		{
			FLASH_ProgramByte(addr++, *cfg_ptr++);
			FLASH_WaitForLastOperation(FLASH_MemType_Data);
		}

		FLASH_Lock(FLASH_MemType_Data);
	}

	/**
	  * ***********************************************************************
	  * @brief	��ȡ����
	  *
	  *	@param  uint8_t * cfg_ptr:   g_ccfg_config������Ԫ�ص�ָ��
	  * @param  uint16_t size:       g_ccfg_config������Ԫ�صĴ�С
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void p_ccfg_read_config(uint8_t* cfg_ptr, uint16_t size)
	{
		uint16_t addr = cfg_ptr - (uint8_t*)&g_ccfg_config + EEPROM_BASE_Addr;

		for (uint16_t i = 0;i < size;i++) 
		{
			*cfg_ptr++ = FLASH_ReadByte(addr++);
		}
	}

	/**
	  * ***********************************************************************
	  * @brief	��������[�豸�ڿ���������Ҫִ�е�����(����˯�߻��ѣ�]
	  *
	  *	@param  void: 
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void dev_boot_routine(void)
	{
		g_run_paramter.m_heartbeat_respond = FALSE;

		/* �����󣬼�¼bc20�Ŀ�ʼ����ʱ�䣬���ڳ�ʱ�ж� */
		g_bc20_init_timeout = GET_1S_TIMER();
		g_bc20_net_timeout = GET_10MS_TIMER();
		g_run_paramter.m_gps_ok = FALSE;

		g_run_paramter.m_tim_heardbeat = g_ccfg_config.m_tim_heartbeat;

		softuart_printf("m_flg_alarm is %d\r\n", g_run_paramter.m_flg_alarm);
		/* ������δ�������ϱ�ʱ�䲻��ͬ */
		if (TRUE == g_run_paramter.m_flg_alarm) 
		{
			g_run_paramter.m_tim_gps_rep = DEFAULT_TIM_GPS_REP * 60;    // ����������Ϊ 15����
		}
		else
		{
			g_run_paramter.m_tim_gps_rep = 180;
		}

		/* ���µ��α�������ʱ�� */
		g_run_paramter.m_tim_alm_continue = 540;

		/* ������Ҫ���˯��ʱ�� */
		g_run_paramter.m_tim_sleep = 0;
		g_run_paramter.m_tim_passby = 0;

		/* ��յ�ѹ����ֵ */
		g_run_paramter.m_cnt_low_vol = 0;

		g_run_paramter.m_dev_voltage = 33;
		led_status = NB_NEY_UNREG;

		/*������ʹ��GPS LNA�Ŵ����ܽ�*/
		GPIO_SetBits(GPS_EN_PORT, GPS_EN_PIN);

	}

	/**
	  * ***********************************************************************
	  * @brief	�ػ�����[�豸�ڹػ�ǰ������Ҫִ�е�����(����˯�߻��ѣ�]
	  *
	  *	@param  void: 
	  *
	  * @retval void:
	  *
	  * @attention	: none
	  * ***********************************************************************
	  */
	void dev_shutdown_routine(void)
	{
#ifndef USE_PSM
		/* �ػ�����״̬����� */
		g_run_paramter.m_flg_nb_net = FALSE;
		g_run_paramter.m_flg_nb_init = FALSE;
		g_run_paramter.m_flg_nb_gps = FALSE;

#endif // !USE_PSM

		/* ����˯��ǰ���ˮ�����ش����Ĵ��� */
		g_run_paramter.m_switch_trigger_cnt = 0;

		/* �������״̬ */
		g_run_paramter.m_flg_alarm = FALSE;

		/* ʱ�������,��ֹ��� */
		g_vu32_10ms_timer = 0;
		IWDG_UPDATA_TIME();

		g_run_paramter.m_flg_wake_dev = FALSE;
		g_run_paramter.m_flg_nb_rep = FALSE;

		/*�ػ�ǰ����GPS LNA�Ŵ����ܽ�*/
		GPIO_ResetBits(GPS_EN_PORT, GPS_EN_PIN);

	}


	/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
}
#endif 
#endif