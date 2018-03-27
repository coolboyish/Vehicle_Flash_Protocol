/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�������⹦�ܴ�������ͨ�õ���غ���
History:
	<author>	<time>		<desc>

************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "reprogram_function.h"
#include "..\InitConfigFromXml\init_config_from_xml_lib.h"
#include "..\interface\protocol_interface.h"
#include "..\function\idle_link_lib.h"
#include "..\SpecialFunction\special_function.h"
#include <time.h>

/*************************************************
Description:	���¼���ϵͳ
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
bool enter_system(void)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	bool bSendStatus = false;
	byte SystemCongig[] = {0x80,0x00,0x1a,0x80,0x01,0x00,0x00,0x06,0x0e,0x00,0x07,0xa1,0x20,0x01,0xc2,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x07,0xe8,0xaa}; //ϵͳ����
	byte FcCmdCongig[]  = {0x91,0x00,0x11,0x04,0x00,0x10,0x07,0xe0,0x30,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0xd5}; //30֡����
	byte EnterCmd[]     = {0xe0,0x00,0x0f,0xaa,0x07,0xe0,0x02,0x10,0x03,0x00,0x00,0x00,0x00,0x00,0x95};
	int ipCmdIndex[2] = {1, 0};
	byte cBufferOffset = 0;//����ƫ��
	int iReceiveResult = TIME_OUT;

	//����BCMϵͳ����
	send_cmd( SystemCongig, SystemCongig[2] );

	if( ( bool )receive_all_cmd( cReceiveTemp, 5, 3000 ) )
	{
		if( cReceiveTemp[3] == 0x00 )
		{
			bSendStatus = true;
		}
	}

	time_delay_ms(30);

	//����BCM30֡����
	send_cmd( FcCmdCongig, FcCmdCongig[2] );

	if( ( bool )receive_all_cmd( cReceiveTemp, 5, 3000 ) )
	{
		if( cReceiveTemp[3] == 0x00 )
		{
			bSendStatus = true;
		}
	}

	time_delay_ms(30);

	set_idle_link(1);	//��ʼ���Ϳ���

	time_delay_ms(30);

	//����BCM��������
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[2].cBufferOffset;
	ipCmdIndex[1] = 2;

	iReceiveResult = send_and_receive_cmd( ipCmdIndex );

	if( iReceiveResult != SUCCESS )
	{
		return false;
	}

	return true;
}
/*************************************************
Description:	ASCIIת��ΪHEX
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
byte asc_to_hex(byte FileData)
{
	if ((FileData >= '0')&&(FileData <='9'))
	{
		FileData -= 0x30;
	}
	else if((FileData >= 'A')&&(FileData <='F'))
	{
		FileData -= 0x37;
	}
	else if((FileData >= 'a')&&(FileData <='f'))
	{
		FileData -= 0x57;
	}

	return FileData;
}
/*************************************************
Description:	ASCIIת��ΪHEX
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
byte hex_to_asc(byte FileData)
{
	if ((FileData >= 0x00)&&(FileData <=0x09))
	{
		FileData += 0x30;
	}
	else if((FileData >= 0x0A)&&(FileData <=0x0F))
	{
		FileData += 0x37;
	}
	else if((FileData >= 0x0a)&&(FileData <=0x0f))
	{
		FileData += 0x57;
	}

	return FileData;
}
/*************************************************
Description:	�����ֽںϲ�Ϊһ��
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
byte merge_two_byte(byte FileData1,byte FileData2)
{
	byte FileData;

	FileData = (asc_to_hex(FileData1) << 4)| asc_to_hex(FileData2);

	return FileData;
}
/*************************************************
Description:	�����ֽںϲ�Ϊһ���������м��ܴ���
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
byte merge_two_byte_and_secret(byte *FileData1,byte *FileData2)
{
	byte FileData;

	_Encryption(FileData1,1);	//���ݼ��ܴ���
	_Encryption(FileData2,1);	//���ݼ��ܴ���

	FileData = (asc_to_hex(*FileData1) << 4)| asc_to_hex(*FileData2);

	return FileData;
}

/*************************************************
Description:	ֹͣ��VDI��������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
byte send_cmd_checksum(byte *buffer,int len)
{
	int i;

	buffer[len -1] = 0;

	for( i = 0; i < len - 1; i++ )
	{
		buffer[len -1] += buffer[i];
	}

	return true;
}
/*************************************************
Description:	��VDI�������ӷ�ʽ��USB������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_send_link_mode(void* pOut)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	int	iReceiveStatus	= FAILE;
	byte cCheckNum = 0;
	int CmdLen = 0;
	byte VDILinkMode[] = {0xC7,0x00,0x07,0x01,0x01,0x01,0x00};
	
	VDILinkMode[5] = g_VDILinkStatus;
	
	CmdLen = VDILinkMode[1]<<8|VDILinkMode[2];
	send_cmd_checksum(VDILinkMode ,CmdLen);

	send_cmd( VDILinkMode, CmdLen );

	//���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
	
	if (iReceiveStatus != SUCCESS)
	{
		special_return_status( PROCESS_OK | NO_JUMP | HAVE_TIP, NULL, "ID_STR_ECU_REPROGRAM_FAIL", 0, pOut );
		return iReceiveStatus;
	}

	special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success", NULL, 0, pOut );

	return iReceiveStatus;
}
/*************************************************
Description:	��ʼˢдָ��
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
// int repro_start_cmd(void)
// {
// 	byte cReceiveTemp[20] = {0}; //���ջ���
// 	int	iReceiveStatus	= FAILE;
// 	byte cCheckNum = 0;
// 	byte VDIStartCmd[] = {0xC7,0x00,0x07,0x01,0x02,0x01,0x00};
// 	int CmdLen = 0;
// 
// 	CmdLen = VDIStartCmd[1]<<8|VDIStartCmd[2];
// 	send_cmd_checksum(VDIStartCmd ,CmdLen);
// 
// 	send_cmd( VDIStartCmd, VDIStartCmd[2] );
// 
// 	//���������ȷ��֡
// 	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
// 
// 	if (iReceiveStatus != SUCCESS)
// 	{
// 		return iReceiveStatus;
// 	}
// 	
// 	g_VDIUploadSchedule = true;//VDI�ϴ�ˢд���ȣ�����ͨѶʱ��
// 	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
// 	g_VDIUploadSchedule = false;//VDI�ϴ�ˢд���ȣ�����ͨѶʱ��
// 	return iReceiveStatus;
// }

/*************************************************
Description:	����У��͡���������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_checksum_and_cmd_sum(byte CurrentBlock)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	bool bSendStatus = false;
	byte cCheckNum = 0;
	byte VDICmdChecksum[] = {0xC7,0x00,0x0E,0x01,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	int CmdLen = 0;
	int	iReceiveStatus	= FAILE;

	VDICmdChecksum[5] = g_ProgramData[CurrentBlock].AllDataChecksum.u8Bit[3];
	VDICmdChecksum[6] = g_ProgramData[CurrentBlock].AllDataChecksum.u8Bit[2];
	VDICmdChecksum[7] = g_ProgramData[CurrentBlock].AllDataChecksum.u8Bit[1];
	VDICmdChecksum[8] = g_ProgramData[CurrentBlock].AllDataChecksum.u8Bit[0];

	VDICmdChecksum[9]  = g_ProgramData[CurrentBlock].FrameTotalNumber.u8Bit[3];
	VDICmdChecksum[10] = g_ProgramData[CurrentBlock].FrameTotalNumber.u8Bit[2];
	VDICmdChecksum[11] = g_ProgramData[CurrentBlock].FrameTotalNumber.u8Bit[1];
	VDICmdChecksum[12] = g_ProgramData[CurrentBlock].FrameTotalNumber.u8Bit[0];

	CmdLen = VDICmdChecksum[1]<<8|VDICmdChecksum[2];
	send_cmd_checksum(VDICmdChecksum ,CmdLen);

	send_cmd( VDICmdChecksum, CmdLen );

	time_delay_ms(500);
	//���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
	if (iReceiveStatus != SUCCESS)
	{
		return iReceiveStatus;
	}

	return iReceiveStatus;
}
/*************************************************
Description:	��ʼ��VDI��������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_start_transfer_data(void)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	int	iReceiveStatus	= FAILE;
	byte cCheckNum = 0;
	byte VDIStartCmd[] = {0xC7,0x00,0x07,0x01,0x03,0x01,0x00};
	int CmdLen = 0;

	CmdLen = VDIStartCmd[1]<<8|VDIStartCmd[2];
	send_cmd_checksum(VDIStartCmd ,CmdLen);
	send_cmd( VDIStartCmd, CmdLen );

	//���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );

	return iReceiveStatus;
}

/*************************************************
Description:	ֹͣ��VDI��������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_stop_transfer_data(void)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	int	iReceiveStatus	= FAILE;
	byte cCheckNum = 0;
	byte VDIStopCmd[] = {0xC7,0x00,0x07,0x01,0x03,0x02,0x00};
	int CmdLen = 0;

	CmdLen = VDIStopCmd[1]<<8|VDIStopCmd[2];
	send_cmd_checksum(VDIStopCmd ,CmdLen);

	send_cmd( VDIStopCmd, VDIStopCmd[2] );

	//���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );

	return iReceiveStatus;
}

/*************************************************
Description:	��֡����ʱ�쳣��������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_config_negative_response(byte NegativeControlStatus)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	int	iReceiveStatus	= FAILE;
	byte cCheckNum = 0;
	int CmdLen = 0;
	byte ConfigNegative[] = {0xC6,0x00,0x23,0x01,0x01,0x02,0x02,0x7F,0x02,0x78,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x10,0x27,0x10,0x01,0x7F,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x38};
	
	ConfigNegative[4] = NegativeControlStatus;
	
	CmdLen = ConfigNegative[1]<<8|ConfigNegative[2];
	send_cmd_checksum(ConfigNegative ,CmdLen);

	send_cmd( ConfigNegative, CmdLen );

	//���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );

	return iReceiveStatus;
}
/*************************************************
Description:ˢд��֡��������
Input:
pIn		������ECUˢд�йص���������
�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_config_insert_Framing_response(byte NegativeControlStatus)
{
	byte cReceiveTemp[20] = {0}; //���ջ���
	int	iReceiveStatus	= FAILE;
	byte cCheckNum = 0;
	int CmdLen = 0;
	byte ConfigNegative[] = {0x90,0x00,0x1b,0x02,0x01,0xff,0x01,0x00,0x07,0xD0,0x00,0x00,0x00,0x00,0x01,0x0a,0x07,0xdf,0x02,0x3e,0x80,0x00,0x00,0x00,0x00,0x00,0x3c};

	ConfigNegative[6] = NegativeControlStatus;
	CmdLen = ConfigNegative[1]<<8|ConfigNegative[2];
	send_cmd_checksum(ConfigNegative ,CmdLen);

	send_cmd( ConfigNegative, CmdLen );
	////���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
	return iReceiveStatus;
}
/*************************************************
Description:	��ǰ��������ˢд�ļ�����һ��
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_config_current_file_block(byte CurrentFileBlock)
{
	byte	cReceiveCache[20] = {0}; //���ջ���
	int		iReceiveStatus	= FAILE;
	byte	cCheckNum = 0;
	int		CmdLen = 0;
	byte	ConfigFileBlock[] = {0xC7,0x00,0x07,0x01,0x06,0x00,0x00};

	ConfigFileBlock[5] = CurrentFileBlock;
	
	CmdLen = ConfigFileBlock[1]<<8|ConfigFileBlock[2];
	send_cmd_checksum(ConfigFileBlock ,CmdLen);

	send_cmd( ConfigFileBlock, CmdLen );

	//���������ȷ��֡
	iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveCache, g_p_stISO15765Config->u16ECUResTimeout );

	return iReceiveStatus;
}
/*************************************************
Description:���ļ���ȡ��ˢд�ļ��İ汾��
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/

int accept_file_ECU_version(void* pOut)
{
	byte ECUVersionOriginal[10];
	byte CompareEcuVersionResult = 0;

	if(g_ProgramData[0].cDataFlag)
	{
		memcpy( ECUVersionOriginal, g_ProgramData[0].pcData+36, 8 );

		//�Ƚ�ƽ̨��	P903V762  P1074752
		CompareEcuVersionResult = memcmp(ECUVersionOriginal,ECUVersionReceive,5);

		if (CompareEcuVersionResult)	//ƽ̨�Ų�һ�£�����FAILE
		{
			special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_REPROGRAM_FILE_ECU_VERSION", 0, pOut );
			return FAILE;
		}
		else
		{
			//�Ƚϰ汾��
			CompareEcuVersionResult = memcmp(ECUVersionOriginal+5,ECUVersionReceive+5,3);

			if (CompareEcuVersionResult)
			{
				g_ChooseReprogramBlock = 0;//����ͬ��������ȫˢ
			}
			else
			{
				g_ChooseReprogramBlock = 2;//��ͬ��ֻˢд��������
			}

			//�ͻ�ѡ����ǿ��ˢд����ˢд������
			if (u8CalibrationFunctionJudge[0] == 1)
			{
				g_ChooseReprogramBlock = 0;
			}
		}
	}
	return SUCCESS;
}

/*************************************************
Description:����HEX�ļ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int switch_Secrethex_Hex(byte *FilePath)  //�������ļ�ת����ԭʼ�ļ�
{
	int j = 0;
	int bStatus = FAILE;
	FILE *FilePointer = NULL;
	FILE *FilePointer1 = NULL;
	int AddressLen = 0;
	char HeadFrameData[255] = {0};//һ������
	uint32 temp = 0;
	int Num =0;
	int nCount = 0;
	/*���ļ�*/
	FilePointer = fopen( FilePath, "r" );
	AddressLen = (int)strlen(FilePath);
	if (NULL == FilePointer)
	{
		free( FilePath );
		return FAILE;
	}
	g_ReprogramFilePath1 =( byte* )malloc(AddressLen + 1) ;

	for (j=0;;j++)
	{
		if (FilePath[j] == '.')
		{
			FilePath[j-1] += 1 ;
			break;
		}
	}
	memcpy(g_ReprogramFilePath1,FilePath,AddressLen);
	g_ReprogramFilePath1[AddressLen] = '\0';
	FilePointer1 = fopen(g_ReprogramFilePath1,"w");
	while(!( feof(FilePointer) ))
	{
		fgets(HeadFrameData,255,FilePointer);
		Num = (int)strlen(HeadFrameData);
		if (temp != 0)
		{
			Encryption(HeadFrameData,Num,g_Secretkey,(int)strlen(g_Secretkey),nCount); //���ܺ���
			fputs(HeadFrameData,FilePointer1);
			fflush(FilePointer1);   //���������
		}
		temp ++;
	}
	if (feof(FilePointer))
	{
		fclose(FilePointer1);
		FilePointer1 = NULL;
		fclose(FilePointer);
		FilePointer = NULL;
	}
	bStatus = repro_make_file_data_HEX(NULL ,g_ReprogramFilePath1);
	remove(g_ReprogramFilePath1);
	if(NULL != g_ReprogramFilePath1)
		free(g_ReprogramFilePath1 );
	return bStatus;
}
//���ܺ���
void Encryption(char* vDataBuffer,const int nSize,const char* pSeed,const int nSeedLen,int nCount)
{
	char cTmp;
	int i =0;
	for (i = 0 ; i < nSize ; i++)
	{
		if (vDataBuffer[i] == '\r' || vDataBuffer[i] == '\n')
		{
			nCount = 0;
			continue;
		}
		cTmp = vDataBuffer[i] ^  *(pSeed + nCount % nSeedLen);
		if ( cTmp == '\r' || cTmp == '\n' || cTmp == '\0')
			vDataBuffer[i] = vDataBuffer[i];
		else
			vDataBuffer[i] = vDataBuffer[i] ^  *(pSeed + nCount % nSeedLen);

		nCount++;
	}
}

/*************************************************
Description:����ˢд�ļ�����---HEX    �ж��Ƿ��Ǽ����ļ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int judge_file_secret_hex(void* pOut, byte *Filepath)
{
	byte i = 0;
	int bStatus = FAILE;
	char HeadFrameData[250] = {0};//һ������
	char cData[5] = {0};
	/*���ļ�*/
	FILE *FilePointer = NULL;
	FilePointer = fopen( Filepath, "r" );
	if (NULL == FilePointer)
	{
		free( Filepath );
		return FAILE;
	}
	for (i = 0; i < 5; i++)
	{
		fgets(HeadFrameData,250,FilePointer);
		if(HeadFrameData[0] == ':')
		{
			fclose(FilePointer);
			FilePointer = NULL;
			bStatus = repro_make_file_data_HEX(pOut, Filepath);//ȡǰ���У����������':'��ͷ֤����ԭʼ�ļ�
			return bStatus;
		}
	}

	fclose(FilePointer);
	FilePointer = NULL;
	bStatus = switch_Secrethex_Hex(pOut);//���ǰ5��û��':'֤���Ǽ����ļ�
	return bStatus;
}
/*************************************************
Description:����ˢд�ļ�����---HEX
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_make_file_data_HEX(void* pOut, byte *Filepath)
{
	int j = 0;
	int i = 0;
	FILE *FilePointer = NULL;
	char HeadFrameData[10] = {0};//һ������
	byte DataType = 0; 
	byte DataLen = 0;
	byte CurrentBlock = 0;
	UNN_2WORD_4BYTE unnBaseAddress = {0};	//ˢд�ļ��Ļ���ַ
	byte *ValueData = NULL;
	int  CurrentDataCount;

	/*���ļ�*/
	FilePointer = fopen( Filepath, "r" );
	if (NULL == FilePointer)
	{
		free( Filepath );
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_OPEN_FILE_FAIL", 0, pOut );
		return FAILE;
	}

	while (1)
	{
		fgets(HeadFrameData,10,FilePointer);

		DataType = merge_two_byte(HeadFrameData[7],HeadFrameData[8]);
		DataLen = merge_two_byte(HeadFrameData[1],HeadFrameData[2]);

		ValueData = ( byte * )malloc( sizeof( byte ) *(DataLen*2+5)  );

		fgets(ValueData,DataLen*2 + 5, FilePointer);

		switch (DataType)  //��������
		{
		case 0x02:
		case 0x04:	//����ַ
			unnBaseAddress.u8Bit[3] = merge_two_byte(ValueData[0],ValueData[1]);
			unnBaseAddress.u8Bit[2] = merge_two_byte(ValueData[2],ValueData[3]);			
			break;
		case 0x00:	//����
				unnBaseAddress.u8Bit[1] = merge_two_byte(HeadFrameData[3],HeadFrameData[4]); 
				unnBaseAddress.u8Bit[0] = merge_two_byte(HeadFrameData[5],HeadFrameData[6]);

				if ( unnBaseAddress.u32Bit == 0x8000BF80  )
				{
					for ( i=0,j=0; i<19,j <38; j+=2,i++ )
					{
						g_LogisticID[i] = merge_two_byte( ValueData[2+j],ValueData[3+j] );

					}
				}

	 			for (CurrentBlock=g_ChooseReprogramBlock; CurrentBlock<REPROGRAM_FILE_BLOCKS_NUMBER; CurrentBlock++)
	 			{
	 				if (unnBaseAddress.u32Bit == g_ProgramData[CurrentBlock].cBeginAddress.u32Bit)
	 				{
	 					g_ProgramData[CurrentBlock].cDataFlag = true;
	 				}
	 			}
				for (CurrentBlock=0; CurrentBlock<REPROGRAM_FILE_BLOCKS_NUMBER; CurrentBlock++)
				{
					if (g_ProgramData[CurrentBlock].cDataFlag)
					{
						CurrentDataCount = ReceiveFileByteNumber;	//��¼��ǰ����������λ�ã��洢�ļ��а汾��ʱʹ��

						for (j=0; j<DataLen*2;)
						{
							g_ProgramData[CurrentBlock].pcData[ReceiveFileByteNumber++] = merge_two_byte(ValueData[0+j],ValueData[1+j]);
							j+=2;

							if (g_ProgramData[CurrentBlock].cValueDataLen.u32Bit == ReceiveFileByteNumber)	//�Ƿ��������ݴ���
							{
								if ( CurrentBlock == REPROGRAM_FILE_BLOCKS_NUMBER )
								{
									if (accept_file_ECU_version(pOut) != SUCCESS)	//��ȡˢд�ļ��еİ汾�ţ����ڶ�ȡ����ECU�汾�Ž��бȽ�
									{
										return FAILE;
									}
								}

								g_ProgramData[CurrentBlock].cDataFlag = false;
								ReceiveFileByteNumber = 0;
								break;
							}
						}
					}
				}
			break;

		case 0x01:	//�ļ�����
			fclose(FilePointer);
			FilePointer = NULL;

			if (ValueData != NULL)
			{
				free(ValueData);
				ValueData = NULL;
			}
			return SUCCESS;

		default:
			break;
		}
	}
	return SUCCESS;
}

/*************************************************
Description:����ˢд�ļ�����---��������EOLˢд�ļ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_make_file_data_eol(void* pOut)
{
	FILE *FilePointer = NULL;
	char HeadFrameData[10] = {0};//һ������
	byte DataType = 0; 
	byte DataLen = 0;
	byte CurrentBlock = 0;
	UNN_2WORD_4BYTE unnBaseAddress = {0};	//ˢд�ļ��Ļ���ַ
	byte *ValueData = NULL;
	int i = 0;
	uint32 m = 0;

	/*���ļ�*/
	FilePointer = fopen( g_ReprogramFilePath, "rb" );
	if (NULL == FilePointer)
	{
		free( g_ReprogramFilePath );
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_OPEN_FILE_FAIL", 0, pOut );
		return FAILE;
	}

	fgets(HeadFrameData,7,FilePointer);	//��ȡ8���ֽڵ�ַ

	for (i=0; i<REPROGRAM_FILE_BLOCKS_NUMBER; i++)
	{		
		fgets(HeadFrameData,9,FilePointer);	//��ȡ8���ֽڵ�ַ

		if (0 == i)
		{
			CurrentBlock = 0;
		}
		else if (1 == i)
		{
			CurrentBlock = 2;
		}
		else if (2 == i)
		{
			CurrentBlock = 1;
		}

		for (m=0; m<g_ProgramData[CurrentBlock].cValueDataLen.u32Bit; m++)
		{
			g_ProgramData[CurrentBlock].pcData[m] = fgetc(FilePointer);
		}

		if (CurrentBlock == 0)
		{
			g_ProgramData[0].cDataFlag = true;
		}
		if (accept_file_ECU_version(pOut) != SUCCESS)	//��ȡˢд�ļ��еİ汾�ţ����ڶ�ȡ����ECU�汾�Ž��бȽ�
		{
			return FAILE;
		}
		g_ProgramData[0].cDataFlag = false;
	}

	fclose(FilePointer);
	FilePointer = NULL;

	return SUCCESS;
}
/*************************************************
Description:����ˢд�ļ�����---rpm
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_initialize_rpm_file(void* pOut)
{
	int i = 0;
	int j = 0;
	FILE *FilePointer = NULL;
	char FrameHeadData[10] = {0};//֡ͷ����
	byte CurrentBlock = 0;
	UNN_2WORD_4BYTE unnBaseAddress = {0};	//ˢд�ļ��Ļ���ַ
	byte FileData1 = 0;//�ļ�����
	byte FileData2 = 0;//�ļ�����
	byte FrameHead = 0;//֡ͷ
	byte FrameLenHigh = 0;//֡������λ
	byte FrameLenLow = 0;//֡������λ
	int FrameLen = 0;
	byte ECUVersionFile[10];
	byte Len = 0;
	byte temp[10];
	byte CompareEcuVersionResult = 0;

	/*���ļ�*/
	FilePointer = fopen( g_ReprogramFilePath, "r" );
	if (NULL == FilePointer)
	{
		free( g_ReprogramFilePath );
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_OPEN_FILE_FAIL", 0, pOut );
		return FAILE;
	}

	while (1)
	{
		if (fgetc(FilePointer) == 'S')
		{
			switch (fgetc(FilePointer))
			{
			case '0':	//�ļ���Ϣ
				fgets(temp,3,FilePointer);	//ȡ����
				Len = merge_two_byte(temp[0],temp[1]);
				fgets(temp,Len+1,FilePointer);	//ȡ����
			
				fgets(temp,3,FilePointer);	//ȡ����
				Len = merge_two_byte(temp[0],temp[1]);
				fgets(ECUVersionFile,Len+1,FilePointer);	//ȡ�ļ��е�ECU�汾��
				
				CompareEcuVersionResult = memcmp(ECUVersionFile,ECUVersionReceive,8);

				if (CompareEcuVersionResult)	//����ͬ����ʾ�ļ���ECU�汾��һ�£�������ѡ����ȷ���ļ���
				{
					special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_REPROGRAM_FILE_ECU_VERSION", 0, pOut );
					return FAILE;
				}

				break;
			case '1':	//������Ϣ
				fgets(FrameHeadData,9,FilePointer);	//ȡ��һ֡�����ǰ�����ֽڣ���ȡ����

				CurrentBlock = merge_two_byte_and_secret(FrameHeadData+0,FrameHeadData+1);

				g_ProgramData[CurrentBlock].FrameTotalNumber.u32Bit++;	//ͳ��ÿһ�����ݵ���֡��
				break;
			default:
				break;
			}
		}

		if( feof( FilePointer ) )	//�ļ�ȡ��
		{
			fclose(FilePointer);
			FilePointer = NULL;	

			return SUCCESS;
		}
	}
}
/*************************************************
Description:	�������֡
Input:
	cFrameHead		����֡ͷ
	pstFrameContent	��������ṹ��ָ��
	cReservedByte	�����б����ֽ�

Output:	none
Return:	bool	���ط���״̬���ɹ���ʧ�ܣ�
Others:	�ú����᳢�Է������Σ������շ�װ��
		�ظ��������жϷ���״̬��
*************************************************/
int package_frame( const byte cFrameHead, STRUCT_CMD* pstFrameContent, const byte cReservedByte )
{
	UNN_2WORD_4BYTE uFrameLen;
	byte cCheckNum = 0;
	int i = 0;

	uFrameLen.u32Bit = 1 + 2 + 1 + pstFrameContent->iCmdLen + 1;

	pcPackageCmdCache = ( byte * )malloc( ( uFrameLen.u32Bit ) * sizeof( byte ) ); //����֡�Ļ���

	pcPackageCmdCache[0] = cFrameHead;
	pcPackageCmdCache[1] = uFrameLen.u8Bit[1];
	pcPackageCmdCache[2] = uFrameLen.u8Bit[0];
	pcPackageCmdCache[3] = cReservedByte;

	memcpy( &pcPackageCmdCache[4], pstFrameContent->pcCmd, pstFrameContent->iCmdLen );

	for( i = 0; ( i < ( int )uFrameLen.u32Bit - 1 ); i++ )
	{
		cCheckNum += pcPackageCmdCache[i];
	}

	pcPackageCmdCache[ uFrameLen.u32Bit - 1] = cCheckNum;

	return SUCCESS;
}
/*************************************************
Description:	�����ݰ�������λ����ʽ���д��
Input:
	cCmdIndex		��������
	pcReceiveBuffer	���ECU�ظ����ݵĻ���

Output:	none
Return:	int	�շ�����ʱ��״̬
Others:	����ISO15765Э�鴦��
*************************************************/
int reprogram_make_cmd_by_iso_15765( STRUCT_CMD *pstSendCmd, byte* pcReceiveBuffer )
{
	int		i = 0;
	int		j = 0;
	int		iReceiveStatus = FAILE;
	byte	cIso15765ReservedByte	= g_p_stISO15765Config->cReserved;
	uint16	u16ECUResTimeout		= g_p_stISO15765Config->u16ECUResTimeout;
	bool	bSendStatus = false;
	int		iCmdLen = 0;
	bool	bMultiFrame = false;//��֡��־
	int 	iReceiveFCStatus = FAILE;//�ȴ�FC֡��־
	int	cFrameSum = 0;
	STRUCT_CMD stCopySendCmd = {0};
	byte	FrameTypeOffset = 0;
	byte	cBufferOffset = 0;
	const byte ExtendCAN = 2;
	const byte StandardCAN = 0;
	int LastBytesLen = 0;
	byte m = 0;

	cBufferOffset = pstSendCmd->cBufferOffset;

	if( pstSendCmd->iCmdLen > 2 && ( pstSendCmd->pcCmd[2 + g_CANoffset] & 0x10 ) == 0x10 )
	{
		bMultiFrame = true;
		iCmdLen = pstSendCmd->iCmdLen;

		cFrameSum = ( iCmdLen - 10 - g_CANoffset ) / 7;  // ��֡�ĸ�����
		cFrameSum += ( ( iCmdLen - 10 - g_CANoffset ) % 7 == 0 ) ? 0 : 1;
	}
	
	//��֡����װ��һ֡
	if( bMultiFrame )
	{
		stCopySendCmd.iCmdLen = pstSendCmd->iCmdLen + cFrameSum * (4 + g_CANoffset) + 5;   //�����
		stCopySendCmd.pcCmd = ( byte* )malloc( sizeof( byte ) * stCopySendCmd.iCmdLen + 6 );

		stCopySendCmd.pcCmd[0] = ( byte )g_p_stISO15765Config->cMultiframestime;	//��֡֡ʱ����

		stCopySendCmd.pcCmd[1] = 0x07; //֡����  0x01: CAN��֡��ʽ����һ֡��30֡�ظ��� 0x02:��һ֡�ȴ��ظ�һ֡  0x04:ˢд�������ֽ�֡��
			
		if (stCopySendCmd.pcCmd[1] == 0x01)
		{
			stCopySendCmd.pcCmd[2] = cFrameSum + 1;
			
			FrameTypeOffset = 0;
		}
		else if(stCopySendCmd.pcCmd[1] == 0x07)
		{
			//֡����
			if (cFrameSum + 1 > 0xff)
			{
				stCopySendCmd.pcCmd[2] = ((cFrameSum + 1) >> 8) & 0xFF;
			}
			else
			{
				stCopySendCmd.pcCmd[2] = 0x00;
			}

			stCopySendCmd.pcCmd[3] = (cFrameSum + 1) & 0xFF;
			
			FrameTypeOffset = 1;
		}

		stCopySendCmd.pcCmd[3 + FrameTypeOffset] = 10 + g_CANoffset; //��һ֡������ֽڸ���

		memcpy( stCopySendCmd.pcCmd + 4 + FrameTypeOffset, pstSendCmd->pcCmd, 10 + g_CANoffset );

		for( i = 0; i < cFrameSum; i++ )
		{
			if(g_CANoffset == StandardCAN)
			{
				stCopySendCmd.pcCmd[14+ FrameTypeOffset + i * 11 + 1] = stCopySendCmd.pcCmd[4 + FrameTypeOffset]; //ID
				stCopySendCmd.pcCmd[14+ FrameTypeOffset + i * 11 + 2] = stCopySendCmd.pcCmd[5 + FrameTypeOffset]; //ID
				stCopySendCmd.pcCmd[14+ FrameTypeOffset + i * 11 + 3] = 0x20 | ( ( i + 1 ) & 0x0F );
			}
			else if(g_CANoffset == ExtendCAN)
			{
				stCopySendCmd.pcCmd[16+ FrameTypeOffset + i * 13 + 1] = stCopySendCmd.pcCmd[4 + FrameTypeOffset]; //ID
				stCopySendCmd.pcCmd[16+ FrameTypeOffset + i * 13 + 2] = stCopySendCmd.pcCmd[5 + FrameTypeOffset]; //ID
				stCopySendCmd.pcCmd[16+ FrameTypeOffset + i * 13 + 3] = stCopySendCmd.pcCmd[6 + FrameTypeOffset]; //ID
				stCopySendCmd.pcCmd[16+ FrameTypeOffset + i * 13 + 4] = stCopySendCmd.pcCmd[7 + FrameTypeOffset]; //ID
				stCopySendCmd.pcCmd[16+ FrameTypeOffset + i * 13 + 5] = 0x20 | ( ( i + 1 ) & 0x0F );
			}

			if( i != cFrameSum - 1 ) //����������һ֡
			{
				memcpy( stCopySendCmd.pcCmd + (14 + g_CANoffset) + FrameTypeOffset + i * (11 + g_CANoffset) + (4 + g_CANoffset), pstSendCmd->pcCmd + (10 + g_CANoffset) + i * 7, 7 );
				stCopySendCmd.pcCmd[14 + g_CANoffset + FrameTypeOffset + i * (11 + g_CANoffset)] = 2 + g_CANoffset + 1 + 7; //ÿ֡������ֽڸ���

			}
			else
			{
				LastBytesLen = iCmdLen - (10 + g_CANoffset) - i * 7;

				if (LastBytesLen < 7)                                                                                                                        
				{                                                                                                                                                                    
					stCopySendCmd.iCmdLen += 7-LastBytesLen;                                                                                                 
				}                                                                                                                                                                    

				memcpy( stCopySendCmd.pcCmd + (14 + g_CANoffset) + FrameTypeOffset + i * (11 + g_CANoffset) + (4 + g_CANoffset), pstSendCmd->pcCmd + (10 + g_CANoffset) + i * 7, 7 );
				
				if (LastBytesLen < 7)                                                                                                                        
				{
					for (m=0; m< 7-LastBytesLen; m++)
					{
						stCopySendCmd.pcCmd[stCopySendCmd.iCmdLen - (7-LastBytesLen) + m] = 0;
					}
				}

				stCopySendCmd.pcCmd[14 + g_CANoffset + FrameTypeOffset + i * (11 + g_CANoffset)] = 2 + g_CANoffset + 1 + 7; //ÿ֡������ֽڸ���                                     
			}
			////�����6���ֽڵĲ�0x00
			//if ( stCopySendCmd.pcCmd[14 + g_CANoffset + i * (11 + g_CANoffset)] < 0x0A )
			//{

			//	stCopySendCmd.pcCmd = realloc( stCopySendCmd.pcCmd, stCopySendCmd.iCmdLen + 10 - stCopySendCmd.pcCmd[14 + g_CANoffset + i * (11 + g_CANoffset)] );
			//	stCopySendCmd.iCmdLen  += 10 - stCopySendCmd.pcCmd[14 + g_CANoffset + i * (11 + g_CANoffset)];

			//	for ( j = 10 - stCopySendCmd.pcCmd[14 + g_CANoffset + i * (11 + g_CANoffset)]; j > 0; j-- )
			//	{
			//		stCopySendCmd.pcCmd[ (14 + g_CANoffset) + i * (11 + g_CANoffset) + (5 + g_CANoffset) + 6 - j] = 0xAA;
			//	}
			//	stCopySendCmd.pcCmd[14 + g_CANoffset  + i * (11 + g_CANoffset)] = 0x0A;
			//}
		}

		bSendStatus = package_frame( FRAME_HEAD_MULTI_FRAME, &stCopySendCmd, cIso15765ReservedByte );
		
		//�ͷ��ڴ�
		if( NULL != stCopySendCmd.pcCmd )
		{
			free( stCopySendCmd.pcCmd );
			stCopySendCmd.pcCmd = NULL;
		}
		return bSendStatus;
	}

	// ��֡
	bSendStatus = package_frame( FRAME_HEAD_NORMAL_FRAME, pstSendCmd, cIso15765ReservedByte );
	
	return bSendStatus;
}
/*************************************************
Description:	�������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int make_reprogram_cmd(byte *StoreSendCmd, int ReceiveFileByteNumber, byte SendDataFrameConter)
{
	STRUCT_CMD stSendCmdCache = {0};
	int iStatus = FAILE;
	byte cReceiveBuffer[20] = {0};

	if (ReceiveFileByteNumber <= 5)	//��֡
	{
		stSendCmdCache.iCmdLen = ReceiveFileByteNumber + 7;	//��ȡ��֡����
		stSendCmdCache.pcCmd = ( byte* )malloc( sizeof( byte ) * stSendCmdCache.iCmdLen );
	
		stSendCmdCache.pcCmd[0] = 0x07;
		stSendCmdCache.pcCmd[1] = 0xE0;

		stSendCmdCache.pcCmd[2] = ReceiveFileByteNumber + 2;
		stSendCmdCache.pcCmd[3] = 0x36;
		stSendCmdCache.pcCmd[4] = SendDataFrameConter;
		
		memcpy(stSendCmdCache.pcCmd + 7,StoreSendCmd,ReceiveFileByteNumber);
	}
	else
	{
		stSendCmdCache.iCmdLen = ReceiveFileByteNumber + 6 + g_CANoffset; //��ȡ��֡����
		stSendCmdCache.pcCmd = ( byte* )malloc( sizeof( byte ) * stSendCmdCache.iCmdLen );
		
		stSendCmdCache.pcCmd[0] = 0x07;
		stSendCmdCache.pcCmd[1] = 0xE0;

		if (ReceiveFileByteNumber + 2 > 0xff)
		{
			stSendCmdCache.pcCmd[2 + g_CANoffset] = 0x10 | (((ReceiveFileByteNumber +2) >> 8) & 0x0F);
		}
		else
		{
			stSendCmdCache.pcCmd[2 + g_CANoffset] = 0x10;
		}

		stSendCmdCache.pcCmd[3 + g_CANoffset] = (ReceiveFileByteNumber + 2) & 0xFF;

		stSendCmdCache.pcCmd[4 + g_CANoffset] = 0x36;
		stSendCmdCache.pcCmd[5 + g_CANoffset] = SendDataFrameConter;

		memcpy(stSendCmdCache.pcCmd + 6 + g_CANoffset,StoreSendCmd,ReceiveFileByteNumber);		
	}

	stSendCmdCache.cBufferOffset = 3;

	iStatus = reprogram_make_cmd_by_iso_15765( ( STRUCT_CMD * )&stSendCmdCache, cReceiveBuffer );

	if(NULL != stSendCmdCache.pcCmd)
		free( stSendCmdCache.pcCmd );

	return iStatus;
}
/*************************************************
Description: ԭ���ļ���ת��Ϊrpm�ļ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int separate_and_send_file_data(byte FileData,FILE *FilePointer)
{
	byte temp[2];
	byte i;

	temp[0] = hex_to_asc((FileData>>4) & 0x0f);
	temp[1] = hex_to_asc(FileData & 0x0f);

	for (i=0; i<2; i++)
	{
		_Encryption(temp+i,1);	//���ݼ��ܴ���
		fputc(temp[i],FilePointer);
	}

	return true;
}

/*************************************************
Description: ԭ���ļ���ת��Ϊrpm�ļ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int repro_change_original_file_to_rpm(void)
{
	FILE *FilePointer = NULL;
	int i;
	byte CurrentBlock = 0;
	byte *StoreSendCmd = NULL;
	int iReceiveResult = TIME_OUT;
	int LastDataLen = 0;
	int FrameLen = 0;
	int SendCmdByteCounter;	//ͳ��һ�������˶����ֽ�
	byte SendDataFrameConter = 0;

	for (i=0;;i++)
	{
		if (g_ReprogramFilePath[i] == '.')
		{
			g_ReprogramFilePath = realloc(g_ReprogramFilePath,i+5);
			g_ReprogramFilePath[i+1] = 'r';
			g_ReprogramFilePath[i+2] = 'p';
			g_ReprogramFilePath[i+3] = 'm';
			g_ReprogramFilePath[i+4] = '\0';
			break;
		}
	}

	/*���ļ�*/
	FilePointer = fopen( g_ReprogramFilePath, "w" );
	if (NULL == FilePointer)
	{
		free( g_ReprogramFilePath );
		return FAILE;
	}

	//fputs("S00901.1125000380",FilePointer);	//д��汾��Ϣ
	//fputs("\n",FilePointer);

	for (CurrentBlock=g_ChooseReprogramBlock; CurrentBlock<REPROGRAM_FILE_BLOCKS_NUMBER; CurrentBlock++)
	{
		StoreSendCmd = ( byte * )malloc( REPROGRAM_EACH_BLOCK_SIZE  );

		SendCmdByteCounter = 0;//�ѷ����ֽ�����
		SendDataFrameConter = 0;//��������֡����������

		while (g_ProgramData[CurrentBlock].cValueDataLen.u32Bit - SendCmdByteCounter > REPROGRAM_EACH_BLOCK_SIZE)
		{
			memcpy( StoreSendCmd, g_ProgramData[CurrentBlock].pcData + SendCmdByteCounter, REPROGRAM_EACH_BLOCK_SIZE );
			SendCmdByteCounter = SendCmdByteCounter + REPROGRAM_EACH_BLOCK_SIZE;
			iReceiveResult = make_reprogram_cmd(StoreSendCmd, REPROGRAM_EACH_BLOCK_SIZE, ++SendDataFrameConter);
			if (iReceiveResult != SUCCESS)
			{
				return iReceiveResult;
			}

			FrameLen = pcPackageCmdCache[1]<<8 | pcPackageCmdCache[2];

			fputs("S1",FilePointer);

			separate_and_send_file_data(CurrentBlock,FilePointer);

			for (i = 0; i<FrameLen; i++)
			{
				separate_and_send_file_data(pcPackageCmdCache[i],FilePointer);
			}

			fputs("\n",FilePointer);

			if(NULL != pcPackageCmdCache)
				free( pcPackageCmdCache );
		}

		LastDataLen = g_ProgramData[CurrentBlock].cValueDataLen.u32Bit - SendCmdByteCounter;
		memcpy( StoreSendCmd, g_ProgramData[CurrentBlock].pcData + SendCmdByteCounter, LastDataLen );
		iReceiveResult = make_reprogram_cmd(StoreSendCmd, LastDataLen, ++SendDataFrameConter);
		if (iReceiveResult != SUCCESS)
		{
			return iReceiveResult;
		}

		FrameLen = pcPackageCmdCache[1]<<8 | pcPackageCmdCache[2];

		fputs("S1",FilePointer);

		separate_and_send_file_data(CurrentBlock,FilePointer);

		for (i = 0; i<FrameLen; i++)
		{
			separate_and_send_file_data(pcPackageCmdCache[i],FilePointer);
		}

		fputs("\n",FilePointer);

		if(NULL != pcPackageCmdCache)
			free( pcPackageCmdCache );

		if( NULL != StoreSendCmd )
			free( StoreSendCmd );
	}

	fclose(FilePointer);
	FilePointer = NULL;

	return SUCCESS;
}
/*************************************************
Description: �ļ�����
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
void _Encryption( char* pData, int nLen )
{
	char cTmp;
	int i = 0;

	for ( ; i < nLen; i++ )
	{
		cTmp = *( pData + i );
		cTmp = cTmp ^ SECRET_KEY ;
		*(pData + i) = cTmp;
	}
}
/*************************************************
Description: ��ȡ�������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
void get_programe_date( char *DateTemp )
{
	int year, month, day;
	time_t nowtime;
	struct tm *timeinfo;
	byte temp[4];
	byte i, j;

	time( &nowtime );
	timeinfo = localtime( &nowtime );

	year = timeinfo->tm_year + 1900;
	month = timeinfo->tm_mon + 1;
	day = timeinfo->tm_mday;

	temp[0] = year / 100;
	temp[1] = year % 100;
	temp[2] = month;
	temp[3] = day;

	j = 0;

	for( i = 0; i < 4; i++ )
	{
		DateTemp[j++] = ( ( temp[i] >> 4 & 0x0F ) * 16 + ( temp[i] & 0x0F ) ) / 10 + 0x30;
		DateTemp[j++] = ( ( temp[i] >> 4 & 0x0F ) * 16 + ( temp[i] & 0x0F ) ) % 10 + 0x30;
	}

	return;
}