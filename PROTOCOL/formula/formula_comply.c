/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:
	1.�����ȡ������״̬������
	2.���崦��汾��Ϣ��ʾ��ʽ������
	3.���崦����ͨ���������㺯����
	4.���崦����֡���������㺯����
	5.���崦��ȫ�����㷨������

History:
	<author>	<time>		<desc>
************************************************************************/

#include <string.h>
#include "formula_comply.h"
#include "formula.h"
#include "../command/command.h"
#include "../public/public.h"
#include "../InitConfigFromXml/init_config_from_xml_lib.h"
#include "../public/protocol_config.h"
#include "../ECUReprogram/reprogram_function.h"

int	g_iDefaultSessionCmdIndex[]		= {1, 0};
int	g_iExtendSessionCmdIndex[]		= {1, 0};
int	g_iRequestSeedCmdIndex[]		= {1, 0};
int	g_iSendKeyCmdIndex[]			= {1, 0};
int g_iActiveECURequestSeedOffset;
int g_iActiveECUSendKeyOffset;

//�������״̬λΪ1����ʾ
byte *DTCStatusBitTrueArry[] =
{
	"ID_STR_DTC_STATUS_BIT0_TRUE",
	"ID_STR_DTC_STATUS_BIT1_TRUE",
	"ID_STR_DTC_STATUS_BIT2_TRUE",
	"ID_STR_DTC_STATUS_BIT3_TRUE",
	"ID_STR_DTC_STATUS_BIT4_TRUE",
	"ID_STR_DTC_STATUS_BIT5_TRUE",
	"ID_STR_DTC_STATUS_BIT6_TRUE",
	"ID_STR_DTC_STATUS_BIT7_TRUE"

};
//�������״̬λΪ0����ʾ
byte *DTCStatusBitFalseArry[] =
{
	"ID_STR_DTC_STATUS_BIT0_FALSE",
	"ID_STR_DTC_STATUS_BIT1_FALSE",
	"ID_STR_DTC_STATUS_BIT2_FALSE",
	"ID_STR_DTC_STATUS_BIT3_FALSE",
	"ID_STR_DTC_STATUS_BIT4_FALSE",
	"ID_STR_DTC_STATUS_BIT5_FALSE",
	"ID_STR_DTC_STATUS_BIT6_FALSE",
	"ID_STR_DTC_STATUS_BIT7_FALSE"
};
/*************************************************
Description:	��ù�����״̬
Input:
	cDctStatusData	������״̬�ֽ�
	cDtcMask		������maskֵ

Output:	pcOut	��������ַ
Return:	int		�ù�����֧�ֵ�״̬����
Others:
*************************************************/

int get_Dtc_status( byte cDctStatusData, byte *pcOut, byte cDtcMask )
{
	int i = 0;
	int iSupportStatusCounter = 0;//֧�ֵ�״̬����
	byte temp_Status = 0;
	byte temp_SupportStatus = 0;
	bool bFirst = true;
  
	while( i < 8 )
	{
		temp_SupportStatus = ( ( cDtcMask >> i ) & 0x01 );
		temp_Status = ( ( cDctStatusData >> i ) & 0x01 );

		if( 0x01 == temp_SupportStatus )//λΪ1
		{

			if( i > 0 && !bFirst )
			{
				*pcOut = ',';
				pcOut++;
			}

			bFirst = true;//��һ�ν�����Ϊ��

			if( 0x01 == temp_Status )
			{

				memcpy( pcOut, DTCStatusBitTrueArry[i], strlen( DTCStatusBitTrueArry[i] ) );
				pcOut += strlen( DTCStatusBitTrueArry[i] );

			}
			else//λΪ0
			{
				memcpy( pcOut, DTCStatusBitFalseArry[i], strlen( DTCStatusBitFalseArry[i] ) );
				pcOut += strlen( DTCStatusBitFalseArry[i] );

			}


			iSupportStatusCounter++;
		}

		i++;

	}

	*pcOut = '\0';

	return iSupportStatusCounter;
}





/*************************************************
Description:	����汾��Ϣ��ʾ��ʽ
Input:
	pcSource	ȡֵ��ַ
	cIvalidLen	��Ч����
	cStyle		��ʾ��ʽ

Output:	pcOut	��������ַ
Return:	void
Others:
*************************************************/

void process_inform_format( const byte* pcSource, byte cIvalidLen, byte cStyle, byte* pcOut )
{
	switch( cStyle )
	{
	case 'A'://ASCII�뷽ʽ����
		DisASCII( pcSource, cIvalidLen, pcOut );
		break;

	case 'H':
	case 'B':
		DisHex( pcSource, cIvalidLen, pcOut );
		break;

	case 'D':
		break;

	default:
		break;
	}
}
/************************************************************************/
/* �������������ַ�����ʽ�õ���                                         */
/************************************************************************/
/*************************************************
typedef struct _STRUCT_STRING_UNIT
{
	byte cCompareData;		//Ҫ�Ƚϵ�����
	byte* pcOutputString;	//Ҫ������ַ���
} STRUCT_STRING_UNIT;
*************************************************/

STRUCT_STRING_UNIT stStringUintArray000[] =
{
	0x00, "ID_STR_DS_CSTRING_000",//��	Off
	0x01, "ID_STR_DS_CSTRING_001",//��	On
};

/*************************************************
typedef struct _STRUCT_DIS_STRING
{
byte cCompareDataSum;					//��Ҫ�Ƚ����ݸ���
byte* pcDefaultOutputString;			//������Ҫ��ʱ������ַ���
STRUCT_STRING_UNIT *pstStringUnitGroup;	//����������
} STRUCT_DIS_STRING;
*************************************************/
STRUCT_DIS_STRING stDisStringArray[] =
{
	{0x02, "----", stStringUintArray000}, //00  bit.0=1:�� bit.0=0:��
};
/*************************************************
Description:	����������ID�������������㹫ʽ
Input:
	iDsId		��������ID
	pcDsSource	ȡֵ��ַ

Output:	pcDsValueOut	��������ַ
Return:	void
Others:
DisplayString(pcDsSource,stDisStringArraypcDsSource,stDisStringArray,0,0xff,0,pcDsValueOut);
*************************************************/

void process_normal_ds_calculate( int iDsId, const byte* pcDsSource, byte* pcDsValueOut )
{
	switch( iDsId )
	{
	case   0:
		OneByteOperation( pcDsSource, 0, 0, 1, 10, "%4.1f", pcDsValueOut ); //x/10
		break;
	default:
		break;
	}
}
/*************************************************
Description:	�汾��Ϣ������
Input:
iDsId
pcDsSource	ȡֵ��ַ

Output:	pcDsValueOut	��������ַ
Return:	void
Others:
DisplayString(pcDsSource,stDisStringArraypcDsSource,stDisStringArray,0,0xff,0,pcDsValueOut);
*************************************************/
void process_normal_infor_calculate( int iDsId, const byte* pcDsSource, byte* pcDsValueOut )
{
	switch( iDsId )
	{
	case   0:
		OneByteOperation( pcDsSource, 0, 0, 1, 10, "%4.1f", pcDsValueOut ); //x/10
		break;
	default:
		break;
	}
}
/*************************************************
Description:	���ݶ���֡������ID�������������㹫ʽ
Input:
	iDsId		��������ID
	pcDsSource	ȡֵ��ַ

Output:	pcDsValueOut	��������ַ
Return:	void
Others:
*************************************************/

void process_freeze_ds_calculate( int iDsId, const byte* pcDsSource, byte* pcDsValueOut )
{
	switch( iDsId )
	{
	case   0:
		DisplayString( pcDsSource, stDisStringArray, 1, 0X01, 0, pcDsValueOut ); //ACC.0=1~Present   ACC.0=0~Not Present
		break;
	default:
		break;
	}
}

/************************************************************************/
/* �����ǰ�ȫ������㹫ʽ                                               */
/************************************************************************/
uint32 seedToKey(uint32 seed, uint32 MASK)
{
	uint32 key = 0;
	uint8 i;


	if(seed != 0)
	{
		for (i = 0; i < 30; i++)
		{
			if (seed & 0x80000000)
			{
				seed = seed << 1;
				seed = seed ^ MASK;
			}
			else
			{
				seed = seed << 1;
			}
		}

		key = seed;
	}

	return key;
}

uint8 ems_calculate(uint8 *Group)
{
	uint32 seed;
	uint32 Key;
	const    uint32   Learmask = 0x2459D1A3;
	seed = (uint32)(*Group << 24) | (uint32)(*(Group + 1) << 16) | (uint32)(*(Group + 2) << 8)  | (uint32)(*(Group + 3));

	Key = seedToKey(seed, Learmask);
	*Group = (uint8)(Key >> 24);
	*(Group + 1) = (uint8)(Key >> 16);
	*(Group + 2) = (uint8)(Key >> 8);
	*(Group + 3) = (uint8)Key;
	return 4;

}

/*************************************************
Description:	���ݰ�ȫ�ȼ�����ȫ�㷨
Input:	cAccessLevel	��ȫ�ȼ�

Output:	pOut	��������ַ
Return:	bool	�㷨ִ��״̬���ɹ���ʧ�ܣ�
Others:	��������ʵ�ֻ���ϵͳ����
*************************************************/

bool process_security_access_algorithm( byte cAccessLevel, void* pOut )
{
	bool bProcessSingleCmdStatus = false;
	byte cBufferOffset = 0;//����ƫ��
	uint16 cRequestSeedCmdOffset = 0;
	uint16 cSendKeyCmdOffset = 0;

	byte cDataArray[10] = {0};
	byte cNeedBytes = 0;

	//���ݰ�ȫ�ȼ�ȷ������ƫ��
	switch( cAccessLevel )
	{
	case 0:
	case 1:
		cRequestSeedCmdOffset	= g_iRequestSeedCmdIndex[1];
		cSendKeyCmdOffset		= g_iSendKeyCmdIndex[1];
		break;
	case 2:
		cRequestSeedCmdOffset	= g_seedCMDBF[0];
		cSendKeyCmdOffset		= g_seedCMDBF[1];
		break;

	default:
		break;
	}

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[ cRequestSeedCmdOffset ].cBufferOffset;
	bProcessSingleCmdStatus = process_single_cmd_without_subsequent_processing( cRequestSeedCmdOffset, pOut );
	if( !bProcessSingleCmdStatus )
	{
		return false;
	}

	memcpy( cDataArray, ( g_stBufferGroup[cBufferOffset].cBuffer + 2 ), 4 );
	if ((0x00 == cDataArray[0])&&(0x00 == cDataArray[1])&&(0x00 == cDataArray[2])&&(0x00 == cDataArray[3]))
	{
		return true;
	}

	//if ((cDataArray[0] == 0)&&(cDataArray[1] == 0)&&(cDataArray[2] == 0)&&(cDataArray[3] == 0))
	//{
	//	return true;
	//}

	//���ݰ�ȫ�ȼ�ȷ�����㹫ʽ
	switch( cAccessLevel )
	{
	case 0:
	case 1:
		cNeedBytes = ems_calculate( cDataArray );
		break;
	case 2:
		cNeedBytes = ems_calculate( cDataArray );
		break;
	default:
		break;
	}

	memcpy( ( g_stInitXmlGobalVariable.m_p_stCmdList[cSendKeyCmdOffset].pcCmd + 5 + g_CANoffset), cDataArray, cNeedBytes );

	bProcessSingleCmdStatus = process_single_cmd_without_subsequent_processing( cSendKeyCmdOffset, pOut );

	if( !bProcessSingleCmdStatus )
	{
		return false;
	}

	return true;

}