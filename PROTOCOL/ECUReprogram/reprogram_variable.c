/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�������⹦���õ��ı���
History:
	<author>	<time>		<desc>

************************************************************************/
#include "reprogram_function.h"
#include <stdio.h>

uint32 ReceiveFileByteNumber = 0;
byte g_VDILinkStatus = 0x00;//0x01:USB���ӷ�ʽ	0x02:�������ӷ�ʽ
byte g_NegativeCode[3] = {0};	//���������Ӧֵ
byte * g_ReprogramDllFilePath_Decryption = NULL;//�ļ�����Dll·��
byte * g_ReprogramDllFilePath_Key = NULL; //�ļ�����Dll·��
byte * g_ReprogramDllFilePath[5] = {0}; //�ļ�����Dll·��
byte * g_ReprogramFilePath = NULL;	//�ļ�·��
byte * g_ReprogramFilePath1 = NULL;	//�ļ�·��1
byte * g_ReprogramFilePath2 = NULL;	//�ļ�·��2 ����Fp�ļ�

STRUCT_REPROGRAM_DATA g_ProgramData[REPROGRAM_FILE_BLOCKS_NUMBER] = {false,
											  0,
											  0,
											  0,
											  0,
											  0,
											  0,
											  NULL};
byte g_ChooseReprogramBlock = 0; //ָ��������һ������
bool g_DataSendOver = false;	//�����Ƿ��ѷ���
uint16 g_seedCMDBF[2];
byte g_LogisticID[18] = {0};
bool g_VDIUploadSchedule = false;//VDI�ϴ�ˢд���ȣ�����ͨѶʱ��
byte *pcPackageCmdCache = NULL;	//����ú������洢��
byte g_CurrentBlockNumber = 0;	//��ǰ��������
byte g_ReprogramFileType = 0;	//��ѡˢд�ļ���ʽ 1:RPM 2:HEX(ԭ��) 3:EOL 
byte ECUVersionReceive[10] = {0};	//ECU�汾�� 
char *g_Secretkey = "20080808" ;
UNN_2WORD_4BYTE CheckSum_Address = {0};