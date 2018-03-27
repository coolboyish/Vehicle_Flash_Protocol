/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	������ӿں���
History:
	<author>	<time>		<desc>

************************************************************************/

#define PROTOCOL_DLL_EXPORTS

#include <string.h>
#include <stdlib.h>

/*#define NDEBUG*/
#include <assert.h>

#include "protocol_interface.h"

#include "../function/dtc_lib.h"
#include "../function/clear_dtc_lib.h"
#include "../function/ds_lib.h"
#include "../function/infor_lib.h"
#include "../function/active_ecu_lib.h"
#include "../function/idle_link_lib.h"
#include "../function/freeze_lib.h"
#include "../function/actuator_test_lib.h"
#include "../SpecialFunction/special_function.h"
#include "../function/quit_system_lib.h"
#include "../InitConfigFromXml/init_config_from_xml_lib.h"
#include "../public/public.h"
#include "../ECUReprogram/reprogram_function.h"


//ȫ�ֱ����������˳���ѭ��
bool g_bCancelWaitDlg = true;
/*************************************************
Description:	ע�ᷢ������ص�����
Input:
	receive_cmd_callback	���������ָ�룬��Ӧ�ó�����
Return:	void
Others:
*************************************************/
void regist_send_cmd_callback( SEND_CMD_CALLBACK send_cmd_callback )
{
	send_cmd = send_cmd_callback;
}

/*************************************************
Description:	ע���������ص�����
Input:
	receive_cmd_callback	�������������Ӧ�ó�����
Return:	void
Others:
*************************************************/
void regist_receive_cmd_callback( RECEIVE_CMD_CALLBACK receive_cmd_callback )
{
	receive_cmd = receive_cmd_callback;
}
/*************************************************
Description:	ע��������д�֡ͷ����ص�����
Input:
receive_cmd_callback	�������������Ӧ�ó�����
Return:	void
Others:
*************************************************/
void regist_receive_all_cmd_callback( RECEIVE_ALL_CMD_CALLBACK receive_all_cmd_callback )
{
	receive_all_cmd = receive_all_cmd_callback;
}
/*************************************************
Description:	ע����ʱ�ص�����
Input:
	time_delay_callback		�������������Ӧ�ó�����
Return:	void
Others:
*************************************************/
void regist_time_delay( TIME_DELAY time_delay_callback )
{
	time_delay_ms = time_delay_callback;
}
/*************************************************
Description:	ע�������λ������ص�����
Input:
update_ui_callback		������λ������ص���������Ӧ�ó�����
Return:	void
Others:
*************************************************/
void regist_update_ui_callback( UPDATE_UI update_ui_callback )
{
	update_ui = update_ui_callback;
}
/*************************************************
Description:	����VCI
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:	���Է���������ʧ�����FAIL
*************************************************/
int setting_vci( void* pIn, void* pOut )
{
	STRUCT_CHAIN_DATA_INPUT* pstParam = ( STRUCT_CHAIN_DATA_INPUT* )pIn;
	byte *pcOutTemp = ( byte * )pOut;

	int iVciConfigOffset = 0;
	int iProtocolConfigOffset = 0;

	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );

	iVciConfigOffset = atoi( pstParam->pcData );//��ȡVCI����ƫ��
	g_p_stVCI_params_config = g_p_stVciParamsGroup[iVciConfigOffset];

	pstParam = pstParam->pNextNode;//��ȡVCI����ģ���

	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );

	pstParam = pstParam->pNextNode;//��ȡЭ������ƫ��

	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );

	iProtocolConfigOffset = atoi( pstParam->pcData );

	pstParam = pstParam->pNextNode;//��ȡЭ������ƫ��

	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );

	g_stInitXmlGobalVariable.m_cProtocolConfigType = ( byte )atoi( pstParam->pcData ); //��ȡЭ������ģ���(����)

	//ѡ��Э�����ã�������ӦЭ��ѡ����Ӧ����
	select_protocol_config( iProtocolConfigOffset, g_stInitXmlGobalVariable.m_cProtocolConfigType );

	if( package_and_send_vci_config() ) //������ճɹ������óɹ��򷵻�SUCCESS
	{
		general_return_status( SUCCESS, NULL, 0, pcOutTemp );
		pcOutTemp[2] = 0;//����ʾ
	}
	else
	{
		general_return_status( FAILE, NULL, 0, pcOutTemp );
		pcOutTemp[2] = 0;//����ʾ
	}

	return 0;
}
/*************************************************
Description:	����ECU
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int active_ECU( void* pIn, void* pOut )
{
	process_active_ECU( pIn, pOut );

	return 0;
}

/*************************************************
Description:	��ȡ��ǰ������
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int read_current_Dtc( void* pIn, void* pOut )
{
	process_read_current_Dtc( pIn, pOut );

	return 0;
}

/*************************************************
Description:	��ȡ��ʷ������
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int read_history_Dtc( void* pIn, void* pOut )
{
	process_read_history_Dtc( pIn, pOut );

	return 0;
}


/*************************************************
Description:	���������
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int clear_Dtc( void* pIn, void* pOut )
{

	process_clear_Dtc( pIn, pOut );

	return 0;
}


/*************************************************
Description:	������֡������
Input:	pIn		���������������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int read_freeze_frame_DTC( void* pIn, void* pOut )
{
	process_read_freeze_frame_DTC( pIn, pOut );

	return 0;
}


/*************************************************
Description:	������֡������
Input:	pIn		�б���DTC��ID
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int read_freeze_frame_DS( void* pIn, void* pOut )
{
	process_read_freeze_frame_DS( pIn, pOut );

	return 0;
}

/*************************************************
Description:	��������
Input:	pIn		DS��ID
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int read_data_stream( void* pIn, void* pOut )
{

	process_read_data_stream( pIn, pOut );

	return 0;
}


/*************************************************
Description:	���汾��Ϣ
Input:	pIn		�汾��Ϣ���ID
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int read_Ecu_information( void* pIn, void* pOut )
{
	process_read_ECU_information( pIn, pOut );

	return 0;
}

/*************************************************
Description:	ִ��������
Input:	pIn		�������ID���������͵ĸ�������ֵ��
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int actuator_test( void* pIn, void* pOut )
{
	process_actuator_test( pIn, pOut );

	return 0;
}


/*************************************************
Description:	���⹦��
Input:	pIn		��ִ�й����йص���������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int special_function( void* pIn, void* pOut )
{
	process_special_function( pIn, pOut );
	return 0;
}
/*************************************************
Description:	ECUˢд
Input:	pIn		��ִ�й����йص���������
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int reprogram_function( void* pIn, void* pOut )
{
	repro_config_insert_Framing_response(1);//�ڷ�������֮ǰ���ò�֡���ʱ����2S  0����  1����
	process_reprogram_function( pIn, pOut );
	repro_config_insert_Framing_response(0);//�ڷ�������֮ǰ���ò�֡���ʱ����2S  0����  1����
	return 0;
}
/*************************************************
Description:	�˳�ϵͳ
Input:	pIn		����
Output:	pOut	������ݵ�ַ
Return:	����
Others:
*************************************************/
int quit_system( void* pIn, void* pOut )
{

	process_quit_system( pIn, pOut );

	return 0;
}
/*************************************************
Description:	��xml��ȡϵͳ����
Input:
		iConfigType		��������
		PIn				������������
Output:	����
Return:	����
Others:
*************************************************/

int init_config_from_xml( int iConfigType, void* pIn )
{
	process_init_config_from_xml( iConfigType, pIn );

	return 0;
}



STRUCT_SELECT_FUN stSetConfigFunGroup[] =
{
	{SET_CONFIG_FC_CAN, process_SET_CONFIG_FC_CMD_CAN},
	{SET_ECU_REPLAY, process_SET_ECU_REPLY},
	{SET_SEND_AND_RECEIVE, get_accord_ecu_cmdnum_send_cmdconfig_data },

};


/*************************************************
Description:	��ȡ�������
Input:
	cType		��������
Output:	����
Return:	pf_general_function ����ָ��
Others:
*************************************************/
pf_general_function get_set_config_fun( byte cType )
{
	int i = 0;

	for( i = 0; i < sizeof( stSetConfigFunGroup ) / sizeof( stSetConfigFunGroup[0] ); i++ )
		if( cType == stSetConfigFunGroup[i].cType )
			return stSetConfigFunGroup[i].pFun;

	return 0;
}

/*************************************************
Description:	 ������������
Input:	PIn		����
Output:	����
Return:	����
Others:
*************************************************/
int special_config_function( void* pIn, void* pOut )
{
	STRUCT_CHAIN_DATA_INPUT* pstParam = ( STRUCT_CHAIN_DATA_INPUT* )pIn;
	pf_general_function pFun = NULL;
	byte cConfigType = 0;
	int iConfigOffset = 0;

	do
	{
		assert( pstParam->pcData );
		assert( pstParam->iLen != 0 );
		iConfigOffset = atoi( pstParam->pcData ); //��ô���������ID

		pstParam = pstParam->pNextNode;
		assert( pstParam->pcData );
		assert( pstParam->iLen != 0 );
		cConfigType = ( byte )atoi( pstParam->pcData ); //��ô���������ģ���

		u32Config_fuc = u32Config_fuc_Group[iConfigOffset];

		pstParam = pstParam->pNextNode;

		pFun = get_set_config_fun( cConfigType );

		assert( pFun );

		pFun( pstParam, pOut );

		if( 1 != *( byte * )pOut )
		{
			return 0;
		}

		pstParam = pstParam->pNextNode;

	}
	while( pstParam != NULL );

	return 0;
}



/*************************************************
Description:	 �ȴ����мӰ�ť���˺��������˳���ѭ��
Input:	PIn		����
Output:	����
Return:	����
Others:
*************************************************/
void cancelWaitDlg(void* pIn, void* pOut)
{
	g_bCancelWaitDlg = false;
}
