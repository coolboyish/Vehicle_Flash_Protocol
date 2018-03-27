/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�����˳�ϵͳ������
History:
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "quit_system_lib.h"
#include "..\command\command.h"
#include "..\public\public.h"
#include "..\interface\protocol_define.h"
#include "..\InitConfigFromXml\init_config_from_xml_lib.h"
#include "..\public\protocol_config.h"
#include <assert.h>
#include "..\SpecialFunction\special_function.h"
#include "idle_link_lib.h"
#include "ds_lib.h"
#include "active_ecu_lib.h"
#include "..\interface\protocol_interface.h"
#include <memory.h>
 
 void static send_quit_cmd(byte SendCmd, byte CmdNum);

STRUCT_SELECT_FUN stQuitSystemFunGroup[] =
{
	{GENERAL_QUIT_SYSTEM, process_general_quit_system},
};

/*************************************************
Description:	��ȡ�����˳�ϵͳ����
Input:
	cType		��������
Output:	����
Return:	pf_general_function ����ָ��
Others:
*************************************************/
pf_general_function get_quit_system_fun( byte cType )
{
	int i = 0;

	for( i = 0; i < sizeof( stQuitSystemFunGroup ) / sizeof( stQuitSystemFunGroup[0] ); i++ )
		if( cType == stQuitSystemFunGroup[i].cType )
			return stQuitSystemFunGroup[i].pFun;

	return 0;
}

/*************************************************
Description:	�˳�ϵͳ
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
void process_quit_system( void* pIn, void* pOut )
{
	pf_general_function pFun = NULL;

	if( g_iActiveECUStatus == ACTIVE_ECU_SUCCESS ) //����ɹ����ִ���˳�����
	{
		set_idle_link( IDLE_LINK_STOP );
		pFun = get_quit_system_fun( g_p_stProcessFunConfig->cQuitSystemFunOffset );

		assert( pFun );

		pFun( pIn, pOut );
	}

	if (g_stInitXmlGobalVariable.m_cProtocolConfigType == SEA1939_CONFIG)
	{
		//reset_VCI();
		package_and_send_vci_config();

	}

	free_xml_config_space();
}
/*************************************************
Description:	��ͨ�˳�ϵͳ����
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
void process_general_quit_system( void* pIn, void* pOut )
{
 
	STRUCT_CHAIN_DATA_INPUT* pstParam = ( STRUCT_CHAIN_DATA_INPUT* )pIn;
 	int iReceiveValidLen = 0;//���յ�����Ч�ֽڳ���
 	uint32 u32CmdData[4] = {0};//��������
	int iCmdSum = 0;
	int i = 0;
 	
	iCmdSum = get_string_type_data_to_uint32( u32CmdData, pstParam->pcData, pstParam->iLen );

	if( 0 == iCmdSum )
	{
		
		general_return_status( SUCCESS, NULL, 0, (byte *)pOut);
		return ;
	}

	for( i = 0; i < iCmdSum; i++ )
	{
		send_quit_cmd(u32CmdData[i], iCmdSum);
	}

	general_return_status( SUCCESS, NULL, 0, pOut );	
}

/*************************************************
Description:	�˳���������
Input:	��
Output:	��
Return:	����
Others:
*************************************************/
void static send_quit_cmd(byte SendCmd, byte CmdNum)
{
	STRUCT_CMD stSendCmd = {0};
	UNN_2WORD_4BYTE uFrameLen;
	bool bReturnStatus = false;
	byte *pcSendCache = NULL;
	byte cReceiveCache[5] = {0};//���յĻ���
	byte cCheckNum = 0;
	int i = 0;	

	stSendCmd.cReserved		= g_stInitXmlGobalVariable.m_p_stCmdList[SendCmd].cReserved;
	stSendCmd.iCmdLen			= g_stInitXmlGobalVariable.m_p_stCmdList[SendCmd].iCmdLen;
	stSendCmd.pcCmd			= ( byte* )malloc( sizeof( byte ) * stSendCmd.iCmdLen );
	memcpy( stSendCmd.pcCmd, g_stInitXmlGobalVariable.m_p_stCmdList[SendCmd].pcCmd, stSendCmd.iCmdLen );
	
	if (g_p_stVCI_params_config->cCommunicationType == 0x00) //K�߼���У���
	{
		pre_process_cmd(&stSendCmd);
		uFrameLen.u32Bit = 1 + 2 + 1 + stSendCmd.iCmdLen + 1;
		pcSendCache = ( byte * )malloc( ( uFrameLen.u32Bit ) * sizeof( byte ) ); //����֡�Ļ���

		pcSendCache[0] = FRAME_HEAD_NORMAL_FRAME;
		pcSendCache[1] = uFrameLen.u8Bit[1];
		pcSendCache[2] = uFrameLen.u8Bit[0];
		pcSendCache[3] = g_p_stISO14230Config->cTimeBetweenBytes;

		memcpy( &pcSendCache[4], stSendCmd.pcCmd, stSendCmd.iCmdLen );
	}
	else if (g_p_stVCI_params_config->cCommunicationType == 0x01)
	{
		pre_process_cmd(&stSendCmd);
		uFrameLen.u32Bit = 1 + 2 + 1 + stSendCmd.iCmdLen + 1 + 2;

		pcSendCache = ( byte * )malloc( ( uFrameLen.u32Bit ) * sizeof( byte ) ); //����֡�Ļ���

		pcSendCache[0] = FRAME_HEAD_NORMAL_FRAME;
		pcSendCache[1] = uFrameLen.u8Bit[1];
		pcSendCache[2] = uFrameLen.u8Bit[0];
		pcSendCache[3] = g_p_stISO14230Config->cTimeBetweenBytes;

		pcSendCache[4] = 0x00;
		pcSendCache[5] = 0xff;

		memcpy( &pcSendCache[6], stSendCmd.pcCmd, stSendCmd.iCmdLen );
	}
	else
	{
		uFrameLen.u32Bit = 1 + 2 + 1 + stSendCmd.iCmdLen + 1;
		pcSendCache = ( byte * )malloc( ( uFrameLen.u32Bit ) * sizeof( byte ) ); //����֡�Ļ���

		pcSendCache[0] = FRAME_HEAD_NORMAL_FRAME;
		pcSendCache[1] = uFrameLen.u8Bit[1];
		pcSendCache[2] = uFrameLen.u8Bit[0];
		pcSendCache[3] = 0x0A;

		memcpy( &pcSendCache[4], stSendCmd.pcCmd, stSendCmd.iCmdLen );
    }

	for( i = 0; ( i < ( int )uFrameLen.u32Bit - 1 ); i++ )
	{
		cCheckNum += pcSendCache[i];
	}
	pcSendCache[ uFrameLen.u32Bit - 1] = cCheckNum;
	send_cmd( pcSendCache, uFrameLen.u32Bit );
	if (CmdNum != 1)
	{
		receive_all_cmd(cReceiveCache, 5, 2000);
	}
	free( pcSendCache );
}


/*************************************************
Description:	�ͷű����xml��ȡ���õĿռ�
Input:	��
Output:	��
Return:	����
Others:
*************************************************/
void free_xml_config_space( void )
{
	free_vci_config_space();
	free_active_ecu_config_space();
	free_protocol_config_space();
	free_specific_command_config_space();
	free_read_dtc_config_space();
	free_freeze_dtc_config_space();
	free_freeze_ds_config_space();
	free_process_fun_config_space();
	free_security_access_config_space();
	free_information_config_space();
	free_freeze_DS_formula_config_space();
	free_idle_link_command_config_space();
	free_idle_link_config_space();
	free_general_DS_formula_config_space();
}
