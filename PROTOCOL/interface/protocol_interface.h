/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�������ӿں���
History:
	<author>	<time>		<desc>

************************************************************************/


#ifndef _INTERFACE_H
#define _INTERFACE_H


#include "protocol_define.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WIN32
#ifdef PROTOCOL_DLL_EXPORTS
#	define PROTOCOL_API __declspec(dllexport)
#else
#	define PROTOCOL_API __declspec(dllimport)
#endif
#else
#	define PROTOCOL_API
#endif


	// ****************************************************************************
	typedef int ( *SEND_CMD_CALLBACK )( const byte*, int ); // ��������ص�����ָ��
	SEND_CMD_CALLBACK send_cmd;
	PROTOCOL_API void regist_send_cmd_callback( SEND_CMD_CALLBACK send_cmd_callback ); // ע�ᷢ������ص�����

	typedef int ( *RECEIVE_CMD_CALLBACK )( byte*, int nLen, int nTimeOut ); // ���ղ���֡ͷ����ص�����ָ��
	RECEIVE_CMD_CALLBACK receive_cmd;
	PROTOCOL_API void regist_receive_cmd_callback( RECEIVE_CMD_CALLBACK receive_cmd_callback ); // ע���������ص�����

	typedef int ( *RECEIVE_ALL_CMD_CALLBACK )( byte*, int nLen, int nTimeOut ); // ���մ�֡ͷ����ص�����ָ��
	RECEIVE_ALL_CMD_CALLBACK receive_all_cmd;
	PROTOCOL_API void regist_receive_all_cmd_callback( RECEIVE_ALL_CMD_CALLBACK receive_all_cmd_callback ); // ע���������ص�����

	typedef void ( *TIME_DELAY )( int ); //��ʱ�ص�����ָ��
	TIME_DELAY time_delay_ms;
	PROTOCOL_API void regist_time_delay( TIME_DELAY time_delay_callback ); // ע����ʱ�ص�����

	typedef int ( *UPDATE_UI )( int , byte* , int ); // ������λ������ص�����
	UPDATE_UI update_ui;
	PROTOCOL_API void regist_update_ui_callback( UPDATE_UI update_ui_callback ); // ע�������λ������ص�����

	// ����VCI
	PROTOCOL_API int setting_vci( void* pIn, void* pOut );

	// ����ECU
	PROTOCOL_API int active_ECU( void* pIn, void* pOut );

	// �����빦��
	PROTOCOL_API int read_current_Dtc( void* pIn, void* pOut );
	PROTOCOL_API int read_history_Dtc( void* pIn, void* pOut );
	PROTOCOL_API int clear_Dtc( void* pIn, void* pOut );

	// ����֡����
	PROTOCOL_API int read_freeze_frame_DTC( void* pIn, void* pOut );
	PROTOCOL_API int read_freeze_frame_DS( void* pIn, void* pOut );

	// ��������
	PROTOCOL_API int read_data_stream( void* pIn, void* pOut );

	// ���汾��Ϣ
	PROTOCOL_API int read_Ecu_information( void* pIn, void* pOut );

	// ִ��������
	PROTOCOL_API int actuator_test( void* pIn, void* pOut );

	// ���⹦��
	PROTOCOL_API int special_function( void* pIn, void* pOut );
	
	// ˢд����
	PROTOCOL_API int reprogram_function( void* pIn, void* pOut );

	// �˳�ϵͳ
	PROTOCOL_API int quit_system( void* pIn, void* pOut );

	//��ȡ��������
	PROTOCOL_API int init_config_from_xml( int iConfigType, void* pIn );

	//��ȡ���⹦������
	PROTOCOL_API int special_config_function( void* pIn, void* pOut );

	//�ȴ����а�ťȡ��
	PROTOCOL_API void cancelWaitDlg(void* pIn, void* pOut);

#undef PROTOCOL_API

#ifdef __cplusplus
}
#endif

#endif/* _INTERFACE_H */

