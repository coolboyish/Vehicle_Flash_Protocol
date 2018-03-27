/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	���⹦�ܵĺ��������ļ��������еĴ�����д�� process_special_function �ϱߡ���������д����������
                �������⹦�ܵĲ�������һ���ļ��ڽ��С�
History:
	<author>    ��ѧ��
	<time>		2015��5��18��14:08:25
	<desc>      �ļ������ ����ģ�壬�������ο�
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "special_function.h"
#include "..\InitConfigFromXml\init_config_from_xml_lib.h"
#include <assert.h>

/*************************************************
Description:	�궨������
Input:
pIn		���������⹦���йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
bool process_judge_yes_no( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int cBufferOffset = 0;//�洢һ������ƫ����

	u8CalibrationFunctionJudge[cSpecialCmdData[1]] = cSpecialCmdData[2];  //�洢ÿһ����Ƿ�ѡ��

	switch( cSpecialCmdData[2] )
	{
	case 0:	//��
		special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "false", NULL, 0, pOut );
		break;

	case 1:	//��
		special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "true", NULL, 0, pOut );
		break;

	default:
		break;
	}

	return true;
}
/*************************************************
Description:	��տͻ�ѡ���ǿ��ˢд���
Input:
pIn		���������⹦���йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
bool process_calibration_exit( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;

	for (i=0; i<20; i++)
	{
		u8CalibrationFunctionJudge[i] = 0xff;
	}

	special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "return", NULL, 0, pOut );

	return true;
}

/*************************************************
Description:	���⹦�ܴ�����
Input:
pIn		���������⹦���йص���������
�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
void process_special_function( void* pIn, void* pOut )
{

	byte cSpecialCmdData[40] = {0};//������⹦����������
	STRUCT_CHAIN_DATA_INPUT* pstParam = ( STRUCT_CHAIN_DATA_INPUT* )pIn;
	byte FunctionSlect = 0;
	bool bStatus = false;

	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );
	FunctionSlect = atoi( pstParam->pcData );

	get_string_type_data_to_byte( cSpecialCmdData, pstParam->pcData, pstParam->iLen );
	//ָ����һ�����
	pstParam = pstParam->pNextNode;

	/* ���⹦�ܵĺ������ */
	switch( cSpecialCmdData[0] )
	{
	case 0 ://�ж��ǻ��ı�־λ
		bStatus = process_judge_yes_no( cSpecialCmdData, pstParam, pOut );
		break;
	case 1:	//�˳����ܣ���ջ���
		bStatus = process_calibration_exit( cSpecialCmdData, pstParam, pOut );
		break;

	default:
		break;
	}

}

