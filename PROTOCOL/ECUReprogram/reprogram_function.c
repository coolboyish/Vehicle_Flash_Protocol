/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	ECUˢд�ĺ��������ļ���
History:
	<author>    
	<time>		
	<desc>      
************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "reprogram_function.h"
#include "..\InitConfigFromXml\init_config_from_xml_lib.h"
#include <assert.h>
#include "../interface/protocol_interface.h"
#include "../SpecialFunction/special_function.h"
#include "../function/idle_link_lib.h"





static const uint32 crc32tab[] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
  0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
  0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
  0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
  0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
  0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
  0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
  0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
  0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
  0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
  0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
  0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
  0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
  0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
  0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
  0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
  0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
  0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
  0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
  0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
  0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
  0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
  0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
  0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
  0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
  0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
  0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
  0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
  0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
  0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
  0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
  0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
  0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
  0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
  0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
  0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
  0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
  0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
  0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
  0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
  0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
  0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
  0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
  0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
  0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
  0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
  0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
  0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
  0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
  0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
  0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
  0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL 
};

uint32 crc32( const unsigned char *buf, uint32 size)//CRC�㷨-bufˢд���ݣ�sizeˢд���ݳ���
{
  uint32 i, crc;
  crc = 0xFFFFFFFF;
  for (i = 0; i < size; i++)
    crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
  return crc^0xFFFFFFFF;
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
int repro_start_cmd(int MODE)
{
  byte cReceiveTemp[20] = {0}; //���ջ���
  int	iReceiveStatus	= FAILE;
  int CmdLen = 0;
  byte cCheckNum = 0;
  byte VDIStartCmd[] = {0xC7,0x00,0x07,0x01,0x03,0x01,0x00};
  if (MODE == 3)//�ļ�����
  {
    VDIStartCmd[4] = 0x03;
  }
  else if (MODE == 2)//�ļ���ʼˢд
  {
    VDIStartCmd[4] = 0x02;
  }
  CmdLen = VDIStartCmd[1]<<8|VDIStartCmd[2];
  send_cmd_checksum(VDIStartCmd ,CmdLen);

  send_cmd( VDIStartCmd, VDIStartCmd[2] );

  //���������ȷ��֡
  iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );

  if (iReceiveStatus != SUCCESS)
  {
    return iReceiveStatus;
  }

  g_VDIUploadSchedule = true;//VDI�ϴ�ˢд���ȣ�����ͨѶʱ��
  iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
  g_VDIUploadSchedule = false;//VDI�ϴ�ˢд���ȣ�����ͨѶʱ��

  return iReceiveStatus;
}

//////////////////�ļ�����////////////////////////

void sub_100010D0(byte* Pedi)
{

  uint32 esi = 0xC6EF3720;
  uint32 edi = 0;
  uint32 ebx = 0;
  UNN_2WORD_4BYTE eax,ecx,edx;
  eax.u32Bit = 0;
  ecx.u32Bit = 0;
  edx.u32Bit = 0;


  ecx.u8Bit[1] = Pedi[0];
  eax.u8Bit[0] = Pedi[2];
  ecx.u8Bit[0] = Pedi[1];
  edx.u8Bit[0] = Pedi[3];

  ecx.u32Bit = ecx.u32Bit << 8;
  ecx.u32Bit |= eax.u32Bit;
  eax.u32Bit = 0;
  eax.u8Bit[1] = Pedi[4];
  eax.u8Bit[0] = Pedi[5];
  ecx.u32Bit = ecx.u32Bit << 8;
  ecx.u32Bit |= edx.u32Bit;
  edx.u32Bit = 0;
  edx.u8Bit[0] = Pedi[6];
  eax.u32Bit = eax.u32Bit << 8;
  eax.u32Bit |= edx.u32Bit;
  edx.u32Bit = 0;
  edx.u8Bit[0] = Pedi[7];
  eax.u32Bit = eax.u32Bit << 8;
  eax.u32Bit |= edx.u32Bit;
  edx.u32Bit = 0x20;
  do
  {
    edi = ecx.u32Bit;
    ebx = ecx.u32Bit;
    edi = edi << 4;
    ebx = ebx >> 5;
    edi += 0x11B44904;
    ebx -= 0x5A39152C;
    edi ^= ebx;
    ebx = ecx.u32Bit + esi;
    edi ^= ebx;
    eax.u32Bit -= edi;
    edi = eax.u32Bit;
    ebx = eax.u32Bit;
    edi = edi << 4;
    ebx = ebx >> 5;
    edi -= 0x6B258ADF;
    ebx -= 0x24DD244D;
    edi ^= ebx;
    ebx = eax.u32Bit + esi;
    edi ^= ebx;
    esi += 0x61C88647;
    ecx.u32Bit -= edi;
    edx.u32Bit --;
  }while(edx.u32Bit != 0);

  edx.u32Bit = ecx.u32Bit;
  edx.u32Bit = edx.u32Bit >> 0x18;
  Pedi[0] = edx.u8Bit[0];
  edx.u32Bit = ecx.u32Bit;
  edx.u32Bit = edx.u32Bit >> 0x10;
  Pedi[1] = edx.u8Bit[0];
  edx.u32Bit = ecx.u32Bit;
  Pedi[3] = ecx.u8Bit[0];
  ecx.u32Bit = eax.u32Bit;
  edx.u32Bit = edx.u32Bit >> 8;
  ecx.u32Bit = ecx.u32Bit >> 0x18;
  Pedi[2] = edx.u8Bit[0];
  Pedi[4] = ecx.u8Bit[0];
  edx.u32Bit = eax.u32Bit;
  ecx.u32Bit = eax.u32Bit;
  edx.u32Bit = edx.u32Bit >> 0x10;
  ecx.u32Bit = ecx.u32Bit >> 8;
  Pedi[5] = edx.u8Bit[0];
  Pedi[6] = ecx.u8Bit[0];
  Pedi[7] = eax.u8Bit[0];
}


/////////////////////��ȫ�㷨/////////////////////////////////////
uint32 sub_67941A00( signed int* a1, signed int* a2 ,uint32 seed)
{
  signed int* esi;
  signed int* edi;
  int i = 0;
  uint32 edx;
  uint32 ecx;
  uint32 eax;
  uint32 ebx;
  uint32 table[4]={0};
  uint16 CX;
  esi = a1;
  edi = a2;
  eax = 0xFFEE;
  table[0] = seed&0xff;
  table[1] = (seed>>8)&0xff;
  table[2] = (seed>>16)&0xff;
  table[3] = (seed>>24)&0xff;

  if (esi <= edi)
  {
    do 
    {
      CX = table[i];
      CX = CX << 8;
      ecx = CX;
      esi++;
      edx = 8;
      do 
      {
        ebx = ecx;
        ebx = ebx^eax;
        eax = eax + eax;
        if ( (ebx&0x8000)!=0 )
        {
          eax = eax^0xA001;
          eax = eax&0xffff;
        }
        ecx = ecx + ecx;
        --edx;
      } while (edx!=0);
      i++;
    } while (esi <= edi);
  }
  return eax;
}
//DLL�ӿں���
uint32 UAE_S_Calc_Key(uint32 seed)
{
  int temp;
  uint32 v2;
  uint32 v7;
  uint32 result;

  temp = sub_67941A00( &seed,&seed+3,seed);
  temp = temp&0xff;
  temp = (temp&0x0f) + 1;
  result = seed;
  v2 = 0;
  do 
  {
    v7 = ((result >> (v2&0x0f)) & 1) | (((result >> (31 - (v2&0x0f)))&1)*2);
    if (v7 == 0)
    {
      result = result^0x0993c1c5;
      result = result*2;
    }
    else if ( v7 == 1)
    {
      result = result - 0x1327838b;
    }
    else if ( v7 == 2)
    {
      result = result*2;
      result = result|1;
      result  = result^0x360d847b;

    }
    else
    {
      result = result - 0x360d847c;
    }
    v2++;
    --temp;
  } while (temp != 0);

  return result;
}

uint32 security_calculate_EMS_A151_DLL( byte *Group )
{
  int i = 0;
  UNN_2WORD_4BYTE Seed;
  uint32 SEED;
  uint32 Keytable;

  for ( i = 0; i < 4; i ++ )
  {
    Seed.u8Bit[3-i] = *(Group + i);
  }
  SEED = Seed.u32Bit;
  Keytable = UAE_S_Calc_Key(SEED);


  return Keytable;
}
/*************************************************
Description:	����ˢд�ļ����ļ���ʽΪ.FP��
Input:

Output:	pOut	��������ַ
Return:	bool	�㷨ִ��״̬���ɹ���ʧ�ܣ�
Others:	��������ʵ�ֻ���ϵͳ����
*************************************************/
byte File_decryption(byte *srcFileName, byte *tgtFileName)
{
  byte bTemp[8] = {0};
  byte bStatus = 0;
  int i = 0;
  int j = 0;

  FILE *srcFilePointer = NULL;
  FILE *tgtFilePointer = NULL;

  srcFilePointer = fopen( srcFileName, "rb");
  tgtFilePointer = fopen( tgtFileName, "wb");

  while(1)
  {
    for( i = 0; i < 8; i ++)
    {
      bTemp[i] = fgetc(srcFilePointer);
      if( feof(srcFilePointer) )
      {
        bStatus = 3;
        break;
      }
    }
    if(8 == i)
    {
      sub_100010D0(bTemp);
    }
    for( j = 0; j < i; j++)
    {
      fputc( bTemp[j], tgtFilePointer);
    }
    if ( 3 == bStatus )
    {
      break;
    }
  }

  fclose(tgtFilePointer);
  tgtFilePointer = NULL;
  fclose(srcFilePointer);
  srcFilePointer = NULL;

	return *tgtFileName;
}

unsigned long  Dll_Key(unsigned long Seed)
{
  UNN_2WORD_4BYTE Key = {0};
    UAE_S_Calc_Key(Seed);
	return Key.u32Bit;
}

/*************************************************
Description:	ϵͳ�˳�ʱ�Ĳ���--ɾ���м��ļ����ͷ��ڴ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
void reprogram_exit_operate( void )
{
	int i = 0;

	//�ͷ�ˢд�ļ��ڴ�
	for (i=0; i<REPROGRAM_FILE_BLOCKS_NUMBER; i++)
	{
		if(NULL != g_ProgramData[i].pcData)
			free( g_ProgramData[i].pcData );
	}
	
	//Ϊԭ���ļ��м��ļ�ʱ��ɾ���ļ�
	if (( g_ReprogramFileType == 2 )||( g_ReprogramFileType == 3 ))	//��ѡˢд�ļ���ʽ 1:RPM 2:HEX(ԭ��) 3:EOL
	{
		remove(g_ReprogramFilePath);
	}

	if( g_ReprogramFileType == 4 )  //��ѡˢд�ļ���ʽ :FP(���������ļ�)
	{
		remove(g_ReprogramFilePath);
		remove(g_ReprogramFilePath2);
	}

	//�ͷ��ļ�·���ڴ�
	if(NULL != g_ReprogramFilePath)
		free( g_ReprogramFilePath );
	if(NULL != g_ReprogramFilePath2)
		free( g_ReprogramFilePath2 );
	//�ͷ��ļ�·���ڴ�
}
/*************************************************
Description:	�����˳�����
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_write_exit( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;
	int iCmdSum = 0;
	int iReceiveResult = TIME_OUT;
	byte cBufferOffset = 0;//����ƫ��
	int CMD1101,Cmd2114FF,Cmd14FFFF;
	uint32 u8CmdIndex[30];//���⹦����������
	int ipCmdIndex[2] = {1, 0};
	bool bProcessStatus = false;
	byte active_ecu_cmd_3_11[]={0x80,0x00,0x1A,0x80,0x01,0x00,0x00,0x03,0x0B,0x00,0x01,0xE8,0x48,0x01,0xC2,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x07,0xA1,0xC6};
	byte active_ecu_cmd_6_14[]={0x80,0x00,0x1A,0x80,0x01,0x00,0x00,0x06,0x0E,0x00,0x07,0xA1,0x20,0x01,0xC2,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x07,0xE8,0xAA};
	byte VDI_SET_cmd_3_11[]={0x91,0x00,0x11,0x04,0x00,0x10,0x07,0x21,0x30,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x22};
	byte VDI_SET_cmd_6_14[]={0x91,0x00,0x11,0x04,0x00,0x10,0x07,0xE0,0x30,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0xE1};


	iCmdSum  = get_string_type_data_to_uint32( u8CmdIndex, pstParam->pcData, pstParam->iLen );

	CMD1101 = u8CmdIndex[0];
	Cmd2114FF = u8CmdIndex[1];
	Cmd14FFFF = u8CmdIndex[2];
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD1101].cBufferOffset;
	ipCmdIndex[1] = CMD1101;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[Cmd14FFFF].cBufferOffset;
	ipCmdIndex[1] = Cmd14FFFF;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );

	

	//ͨ�Ų������û���
	send_cmd( active_ecu_cmd_3_11, 26 );
	time_delay_ms(100);
	send_cmd( VDI_SET_cmd_3_11, 17 );
	time_delay_ms(500);

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[Cmd2114FF].cBufferOffset;
	ipCmdIndex[1] = Cmd2114FF;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );

	//ͨ�Ų������û���
	send_cmd( active_ecu_cmd_6_14, 26 );
	time_delay_ms(100);
	send_cmd( VDI_SET_cmd_6_14, 17 );
	time_delay_ms(500);



	reprogram_exit_operate();	//ɾ���м��ļ����ͷ��ڴ�

	special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success", NULL, 0, pOut );
	return iReceiveResult;
}
/*************************************************
Description:	��ȡECUˢд�ļ������͸�ECU
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_send_data(byte CurrentBlock)
{
	byte UpdateProgressTemp;
	byte UpdateProgressLen = 0;
	int UpdateDataNum = 0;
	int DataTotalNum = 0;
	byte UpdateProgress[20] = {0};
	int iReceiveResult = TIME_OUT;
	int LastDataLen = 0;
	int i = 0;
	byte *pcSendCache = NULL;
	FILE *FilePointer = NULL;
	byte CurrentFrameBlock = 0;	//��ǰ֡�Ŀ���
	char FrameHeadData[15] = {0};//֡ͷ����
	byte FileData1[2];//�ļ�����
	byte FileData2[2];//�ļ�����
	byte FrameHead = 0;//֡ͷ
	byte FrameLenHigh = 0;//֡������λ
	byte FrameLenLow = 0;//֡������λ
	int FrameLen = 0;
	byte cReceiveTemp[20] = {0}; //���ջ���
	int	iReceiveStatus	= FAILE;
	int FrameCouter = 0;//���������֡��

	/*���ļ�*/
	FilePointer = fopen( g_ReprogramFilePath, "r" );
	if (NULL == FilePointer)
	{
		free( g_ReprogramFilePath );
		return -1;
	}

	FrameCouter = 0;//���������֡������
	while (1)
	{
		fgets(FrameHeadData,11,FilePointer);	//ȡ��һ֡�����ǰ�����ֽڣ���ȡ֡��

		if (FrameHeadData[0] == 'S')
		{
			switch (FrameHeadData[1])
			{
			case '0':	//�ļ���Ϣ
				break;
			case '1':	//������Ϣ
				CurrentFrameBlock = merge_two_byte_and_secret(FrameHeadData+2,FrameHeadData+3);
				
				if (CurrentFrameBlock == CurrentBlock)
				{
					FrameHead = merge_two_byte_and_secret(FrameHeadData+4,FrameHeadData+5);	//֡ͷ

					FrameLenHigh = merge_two_byte_and_secret(FrameHeadData+6,FrameHeadData+7);

					FrameLenLow = merge_two_byte_and_secret(FrameHeadData+8,FrameHeadData+9);

					FrameLen = FrameLenHigh<<8 | FrameLenLow;	//֡��

					//����ʵ�ʴ�С���ڴ�
					pcSendCache = ( byte * )malloc(sizeof( byte ) * FrameLen);
					pcSendCache[0] = FrameHead;
					pcSendCache[1] = FrameLenHigh;
					pcSendCache[2] = FrameLenLow;
				
					//�������������У���
					for (i=0; i<3; i++)
					{
						g_ProgramData[CurrentBlock].AllDataChecksum.u32Bit += pcSendCache[i];
					}

					for (i=0; i<FrameLen - 3; i++)
					{
						FileData1[0] = fgetc( FilePointer );
						FileData2[0] = fgetc( FilePointer );
						pcSendCache[3+i] = merge_two_byte_and_secret(FileData1,FileData2);
						
						//�������������У���
						g_ProgramData[CurrentBlock].AllDataChecksum.u32Bit += pcSendCache[3+i];
					}
				
					send_cmd( pcSendCache, FrameLen );

					//���������ȷ��֡
					iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );

					if (iReceiveStatus != SUCCESS)
					{
						send_cmd( pcSendCache, FrameLen );//�ط�һ��
						
						//���������ȷ��֡
						iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
						
						if (iReceiveStatus != SUCCESS)
						{
							fclose(FilePointer);
							FilePointer = NULL;
							free( pcSendCache );
							pcSendCache = NULL;

							return iReceiveStatus;
						}
					}

					iReceiveStatus = process_CAN_receive_single_Cmd( cReceiveTemp, g_p_stISO15765Config->u16ECUResTimeout );
					if (iReceiveStatus != SUCCESS)
					{
						fclose(FilePointer);
						FilePointer = NULL;

						free( pcSendCache );
						pcSendCache = NULL;

						return iReceiveStatus;
					}
					
					if(NULL != pcSendCache)
					{
						free( pcSendCache );
						pcSendCache = NULL;
					}
					time_delay_ms( g_p_stISO15765Config->u16TimeBetweenFrames );	//֡���ʱ��

					FrameCouter++;
					
					UpdateProgressTemp = FrameCouter*100/g_ProgramData[CurrentBlock].FrameTotalNumber.u32Bit;
					UpdateProgressLen = sprintf( UpdateProgress, "%d", UpdateProgressTemp);

					//������ʾ��Ϣ
					if (g_VDILinkStatus == 0x01)	//USB
					{
						switch(CurrentBlock)
						{
						case 0x00:
							update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAMING_0" , 21 );
							break;
						case 0x01:
							update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAMING_1" , 21 );
							break;
						case 0x02:
							update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAMING_2" , 21 );
							break;
						case 0x03:
							update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAMING_3" , 21 );
							break;
						case 0x04:
							update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAMING_4" , 21 );
							break;

						default:
							break;
						}
					}
					else if (g_VDILinkStatus == 0x02)	//����
					{
						switch(CurrentBlock)
						{
						case 0x00:
							update_ui( VDI_UPDATE_TIP, "ID_STR_DOWNLOAD_0" , 17 );
							break;
						case 0x01:
							update_ui( VDI_UPDATE_TIP, "ID_STR_DOWNLOAD_1" , 17 );
							break;
						case 0x02:
							update_ui( VDI_UPDATE_TIP, "ID_STR_DOWNLOAD_2" , 17 );
							break;
						case 0x03:
							update_ui( VDI_UPDATE_TIP, "ID_STR_DOWNLOAD_3" , 17 );
							break;
						case 0x04:
							update_ui( VDI_UPDATE_TIP, "ID_STR_DOWNLOAD_4" , 17 );
							break;
						default:
							break;
						}

					}
					//���½���
					update_ui( VDI_UPDATE_PROGRESS, UpdateProgress , UpdateProgressLen );
				}	
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
Description:	��������ǰ������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_send_cmd_before(byte CurrentBlock,uint32* u32CmdIndex,void * pOut)
{
	uint32 CMD3101FF00,CMD3103FF00,CMD340033;
	int bStatus = false;
	byte cBufferOffset = 0;//����ƫ��
	int ipCmdIndex[2] = {1, 0};
	int iReceiveResult = TIME_OUT;
	int j = 0;

	CMD3101FF00 = u32CmdIndex[0];
	CMD3103FF00 = u32CmdIndex[1];
	CMD340033   = u32CmdIndex[2];

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD3101FF00].cBufferOffset;
	ipCmdIndex[1] = CMD3101FF00;

	for (j=0; j<=3; j++)		//����ʼ��ַ
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD3101FF00].pcCmd[8+j] = g_ProgramData[CurrentBlock].cBeginAddress.u8Bit[3-j];
	}

	for (j=0; j<=3; j++)		//�������ַ
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD3101FF00].pcCmd[12+j] = g_ProgramData[CurrentBlock].cEndAddress.u8Bit[3-j];
	}

	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}

	//--------------------------------------------------------------------------------------------------------
	//cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD3103FF00].cBufferOffset;
	//ipCmdIndex[1] = CMD3103FF00;
	//iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	//if( iReceiveResult != SUCCESS )
	//{
	//	general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
	//	return iReceiveResult;
	//}
	//--------------------------------------------------------------------------------------------------------
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD340033].cBufferOffset;
	ipCmdIndex[1] = CMD340033;
	for (j=0; j<=3; j++)		//����ʼ��ַ
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD340033].pcCmd[7+j] = g_ProgramData[CurrentBlock].cBeginAddress.u8Bit[3-j];
	}

	for (j=0; j<3; j++)		//�������ַ
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD340033].pcCmd[11+j] = g_ProgramData[CurrentBlock].cValueDataLen.u8Bit[2-j];
	}

	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}
	return iReceiveResult;
}
/*************************************************
Description:	�������ݺ������
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_send_cmd_after(byte CurrentBlock,uint32* u32CmdIndex,void * pOut)
{
	uint32 CMD37,CMD31010202,CMD3103FF01;
	int bStatus = false;
	byte cBufferOffset = 0;//����ƫ��
	int ipCmdIndex[2] = {1, 0};
	int iReceiveResult = TIME_OUT;
	int j = 0;
	UNN_2WORD_4BYTE Checksum_Value = {0};

	CMD37       = u32CmdIndex[4];
	CMD31010202 = u32CmdIndex[5];
	CMD3103FF01 = u32CmdIndex[6];

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD37].cBufferOffset;
	ipCmdIndex[1] = CMD37;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}
	//--------------------------------------------------------------------------------------------------------
	//cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD31010202].cBufferOffset;
	//ipCmdIndex[1] = CMD31010202;
	//iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	//if( iReceiveResult != SUCCESS )
	//{
	//	general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
	//	return iReceiveResult;
	//}
	//--------------------------------------------------------------------------------------------------------

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD3103FF01].cBufferOffset;
	ipCmdIndex[1] = CMD3103FF01;

	for (j=0; j<=3; j++)		//����ʼ��ַ
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD3103FF01].pcCmd[8+j] = g_ProgramData[CurrentBlock].cBeginAddress.u8Bit[3-j];
	}

	for (j=0; j<=3; j++)		//�������ַ
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD3103FF01].pcCmd[12+j] = g_ProgramData[CurrentBlock].cEndAddress.u8Bit[3-j];
	}

	/* API�ӿ� ��ȡУ������ */
	for ( j=0; j<=3; j++ )
	{
		CheckSum_Address.u8Bit[j] =  g_ProgramData[CurrentBlock].cBeginAddress.u8Bit[3-j];
	}

	Checksum_Value.u32Bit = crc32( g_ProgramData[CurrentBlock].pcData, g_ProgramData[CurrentBlock].cValueDataLen.u32Bit);

	for(j=0; j <=3; j++)        //��У��������
	{
		g_stInitXmlGobalVariable.m_p_stCmdList[CMD3103FF01].pcCmd[16+j] = Checksum_Value.u8Bit[3-j];
	}

	iReceiveResult = send_and_receive_cmd( ipCmdIndex );

	/*if ( g_stBufferGroup[cBufferOffset].cBuffer[4] != 0x71 )
	{
			iReceiveResult = send_and_receive_cmd( ipCmdIndex );

			if ( g_stBufferGroup[cBufferOffset].cBuffer[4] != 0x71 )
			{
				iReceiveResult = send_and_receive_cmd( ipCmdIndex );
				if( iReceiveResult != SUCCESS )
				{
					general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
					return iReceiveResult;
				}	
			}
	}*/

	return iReceiveResult;
}
/*************************************************
Description:	��ȡECUˢд�ļ������͸�ECU----USB
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_send_usb( byte CurrentBlock,uint32* u32CmdIndex,void * pOut )
{
	int j = 0;
	int iReceiveResult = TIME_OUT;
	int bStatus = false;

	//������ǰ����
	iReceiveResult = reprogram_send_cmd_before(CurrentBlock,u32CmdIndex,pOut);

	if (iReceiveResult != SUCCESS )
	{
		return iReceiveResult;
	}

	repro_config_negative_response(1);//������Ӧ���� 0����	1����

	//��������
	iReceiveResult = reprogram_send_data(CurrentBlock);
	
	if (iReceiveResult != SUCCESS)
	{
		if (iReceiveResult == NEGATIVE)
		{
			general_return_status( iReceiveResult, g_NegativeCode, 3, pOut );
			return iReceiveResult;
		}
		special_return_status( PROCESS_OK | NO_JUMP | HAVE_TIP, NULL, "ID_STR_ECU_REPROGRAM_FAIL", 0, pOut );

		return iReceiveResult;
	}

	//�����ݺ�����
	repro_config_negative_response(0);//������Ӧ���� 0����	1����

	iReceiveResult = reprogram_send_cmd_after(CurrentBlock,u32CmdIndex,pOut);

	if (iReceiveResult != SUCCESS )
	{
		return iReceiveResult;
	}

	if (CurrentBlock == REPROGRAM_FILE_BLOCKS_NUMBER - 1)	//���ݴ��������Ϊ��
	{
		special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success_exit", NULL, 0, pOut );
	}

	return SUCCESS;
}
/*************************************************
Description:	��ȡECUˢд�ļ������͸�ECU----����
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_send_bluetooth( byte CurrentBlock,uint32* u32CmdIndex,void * pOut )
{
	int j = 0;
	int iCmdSum = 0;
	int iReceiveResult = TIME_OUT;
	int bStatus = false;
	//���ݴ��͸�VDI
	if (!g_DataSendOver)//û�д��������ʱִ��
	{
		repro_config_negative_response(1);    //������Ӧ���� 0����	1����
		bStatus = repro_start_cmd(3);    //��ʼ����ָ��-����ר��
		iReceiveResult = reprogram_send_data(CurrentBlock);
		
		if (iReceiveResult != SUCCESS)
		{
			if (iReceiveResult == NEGATIVE)
			{
				general_return_status( bStatus, g_NegativeCode, 3, pOut );
				return bStatus;
			}
			special_return_status( PROCESS_OK | NO_JUMP | HAVE_TIP, NULL, "ID_STR_ECU_REPROGRAM_FAIL", 0, pOut );

			return iReceiveResult;
		}

		bStatus = repro_stop_transfer_data();//ֹͣ��VDI��������
		if (bStatus != SUCCESS )
		{
			return bStatus;
		}

		bStatus = repro_checksum_and_cmd_sum(CurrentBlock);//����У��͡���������
		if (bStatus != SUCCESS )
		{
			special_return_status( PROCESS_OK | NO_JUMP | HAVE_TIP, NULL,"ID_STR_FILE_DOWNLOAD_FAIL", 0, pOut );
			return bStatus;
		}
		repro_config_negative_response(0);    //������Ӧ���� 0����	1����
		if (CurrentBlock == REPROGRAM_FILE_BLOCKS_NUMBER - 1)	//���ݴ������
		{
			g_DataSendOver = true;//���ݴ��������Ϊ��
			special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success_enter", NULL, 0, pOut );
			return SUCCESS;
		}
	}

	if (g_DataSendOver)//����ȫ�����͸�VDI��ִ��
	{
		repro_config_negative_response(1);    //������Ӧ���� 0����	1����
		//��������ǰ����
		iReceiveResult = reprogram_send_cmd_before(CurrentBlock,u32CmdIndex,pOut);

		if (iReceiveResult != SUCCESS )
		{
			return iReceiveResult;
		}
		repro_config_negative_response(1);//������Ӧ���� 0����	1����

        bStatus = repro_start_cmd(2);    //��ʼ����ָ��-����ר��	
		//�������ݺ�����
		repro_config_negative_response(0);//������Ӧ���� 0����	1����
		iReceiveResult = reprogram_send_cmd_after(CurrentBlock,u32CmdIndex,pOut);

		if (iReceiveResult != SUCCESS )
		{
			return iReceiveResult;
		}
	
		if (CurrentBlock == REPROGRAM_FILE_BLOCKS_NUMBER - 1)	//ˢд������ɣ������˳�����
		{
			special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success_exit", NULL, 0, pOut );
		}
	}
	return iReceiveResult;
}
/*************************************************
Description:	��ȡECUˢд�ļ������͸�ECU
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_write_send( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int j = 0;
	int iCmdSum = 0;
	int iReceiveResult = TIME_OUT;
	uint32 u32CmdIndex[30];//���⹦����������
	uint32 CMD36XX;
	int bStatus = false;
	byte CurrentBlock = 0;
	
	iCmdSum  = get_string_type_data_to_uint32( u32CmdIndex, pstParam->pcData, pstParam->iLen );

	CMD36XX     = u32CmdIndex[3];

	update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAM_PREPARE", 24 );
	update_ui( VDI_UPDATE_PROGRESS, "0" , 1 );
	
	for (CurrentBlock=g_ChooseReprogramBlock; CurrentBlock<REPROGRAM_FILE_BLOCKS_NUMBER; CurrentBlock++)
	{	
		g_CurrentBlockNumber = CurrentBlock;	//��ȡ��ǰ��������������ͨѶʱ�����պ��������ж���ʾ��Ϣ
		iReceiveResult = repro_config_current_file_block(CurrentBlock);	//�ѵ�ǰ�������Ŀ��ļ�����VDI

		if (iReceiveResult != SUCCESS)
		{
			return iReceiveResult;
		}
		switch (g_VDILinkStatus)
		{
		case 0x01:	//USBģʽ
			bStatus = reprogram_send_usb( CurrentBlock,u32CmdIndex,pOut );
			if (bStatus != SUCCESS)
			{
				return bStatus;
			}	
			break;
		case 0x02:	//����ģʽ
			bStatus = reprogram_send_bluetooth( CurrentBlock,u32CmdIndex,pOut );
			if (bStatus != SUCCESS)
			{
				return bStatus;
			}	
			break;
		default:
			break;
		}
	}
	return SUCCESS;
}
/*************************************************
Description:	D70EMSˢд
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_write_enter( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;
	int iCmdSum = 0;
	int iReceiveResult = TIME_OUT;
	byte cBufferOffset = 0;//����ƫ��
	int CMD1003,CMD31010203,CMD8502,CMD280303,CMDF195,CMD1002,CMD22F1F7,CMD2EF1F7,CMD2707,CMD2708;
	uint32 u8CmdIndex[30];//���⹦����������
	int ipCmdIndex[2] = {1, 0};
	bool bProcessStatus = FAILE;
	UNN_2WORD_4BYTE cDataArray = {0};
	UNN_2WORD_4BYTE seed = {0};
	unsigned long cNeedBytes = 4;
	int j = 0;

	iCmdSum  = get_string_type_data_to_uint32( u8CmdIndex, pstParam->pcData, pstParam->iLen );

	CMD1003     = u8CmdIndex[0];
	CMD31010203 = u8CmdIndex[1];
	CMD8502     = u8CmdIndex[2];
	CMD280303   = u8CmdIndex[3];
	CMDF195     = u8CmdIndex[4];
	CMD1002     = u8CmdIndex[5];
	CMD22F1F7   = u8CmdIndex[6];
	CMD2EF1F7   = u8CmdIndex[7];
	CMD2707     = u8CmdIndex[8];
	CMD2708     = u8CmdIndex[9];

	//cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD1003].cBufferOffset;
	//ipCmdIndex[1] = CMD1003;
	//iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	//if( iReceiveResult != SUCCESS )
	//{
	//	general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
	//	return iReceiveResult;
	//}

	//cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD8502].cBufferOffset;
	//ipCmdIndex[1] = CMD8502;
	//iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	//if( iReceiveResult != SUCCESS )
	//{
	//	general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
	//	return iReceiveResult;
	//}

	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD1002].cBufferOffset;
	ipCmdIndex[1] = CMD1002;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}

	/* ����API�ӿں��� ���м���У�� */
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD2707].cBufferOffset;
	ipCmdIndex[1] = CMD2707;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}

	for (i=0; i<4; i++)
	{
		seed.u8Bit[3-i] = g_stBufferGroup[cBufferOffset].cBuffer[2+i];
	}
// 	memcpy( seed, ( g_stBufferGroup[cBufferOffset].cBuffer + 2 ), 4 );

	if (!( seed.u32Bit == 0 ))
	{
		cDataArray.u32Bit = security_calculate_EMS_A151_DLL(&g_stBufferGroup[cBufferOffset].cBuffer[2]);
// 		memcpy( ( g_stInitXmlGobalVariable.m_p_stCmdList[CMD2708].pcCmd + 6 ), cDataArray.u8Bit, cNeedBytes );
		
		for (i=0; i<4; i++)
		{
			 g_stInitXmlGobalVariable.m_p_stCmdList[CMD2708].pcCmd[5+i]= cDataArray.u8Bit[3-i];
		}
		ipCmdIndex[1] = CMD2708;
		iReceiveResult = send_and_receive_cmd( ipCmdIndex );
		if( iReceiveResult != SUCCESS )
		{
			general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
			return iReceiveResult;
		}
	}
	/* ��ȡ�ļ� ��ȡLogistic ID ��18λ�� */
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD22F1F7].cBufferOffset;
	ipCmdIndex[1] = CMD22F1F7;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}
	memcpy(g_stInitXmlGobalVariable.m_p_stCmdList[CMD2EF1F7].pcCmd + 7, g_LogisticID + 1,18);
	/* ��ȡ�ļ� д��Logistic ID ��18λ�� */
	ipCmdIndex[1] = CMD2EF1F7;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}

	special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success", NULL, 0, pOut );
	return SUCCESS;
}
/*************************************************
Description:����λ������ȡˢд�ļ�·��
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int read_ecu_file_way( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;
	int j = 0;
	int Flag_0 = 10;
	int Flag_1 = 10;
	int CMDF194;
	int iCmdSum = 0;
	int ipCmdIndex[2] = {1, 0};
	uint32 u8CmdIndex[10];//���⹦����������
	byte cBufferOffset = 0;//����ƫ��
	int iReceiveResult = TIME_OUT;
	size_t CheckedLen[2] = {0};


	//��ȡ��ǰ����״̬ 0x00:δ����	0x01:USB���ӷ�ʽ	0x02:�������ӷ�ʽ
	g_VDILinkStatus = atoi( pstParam->pcData );	
	if( g_VDILinkStatus == 0 )
	{
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_VDI_LINK_FAIL", 0, pOut );
		return FAILE;
	}


	pstParam = pstParam->pNextNode;
	assert( pstParam->pcData );
	//assert( pstParam->iLen != 0 );
	//��ȡˢд�ļ�·��
	if( pstParam->iLen == 0 )
	{
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "select_fail", 0, pOut );
		return FAILE;
	}

	g_ReprogramFilePath =( byte*)malloc(pstParam->iLen + 1) ;
	memcpy(g_ReprogramFilePath,pstParam->pcData,pstParam->iLen);
	g_ReprogramFilePath[pstParam->iLen] = '\0';
	g_ReprogramFilePath2 =( byte*)malloc(pstParam->iLen + 1) ;
	memcpy(g_ReprogramFilePath2,pstParam->pcData,pstParam->iLen);
	g_ReprogramFilePath2[pstParam->iLen-1] ++;
	g_ReprogramFilePath2[pstParam->iLen] = '\0';

	//�ж�ECU�汾��
	pstParam = pstParam->pNextNode;
	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );
	iCmdSum  = get_string_type_data_to_uint32( u8CmdIndex, pstParam->pcData, pstParam->iLen );
	CMDF194  = u8CmdIndex[0];
	//��ȡECU�汾
	//cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMDF194].cBufferOffset;
	//ipCmdIndex[1] = CMDF194;
	//iReceiveResult = send_and_receive_cmd( ipCmdIndex );
	//
	//if( iReceiveResult != SUCCESS )
	//{
	//	general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
	//	return iReceiveResult;
	//}
	//memcpy( ECUVersionReceive, g_stBufferGroup[cBufferOffset].cBuffer+3, 4 );
	//general_return_status( iReceiveResult, ECUVersionReceive, 4, pOut );
	pstParam = pstParam->pNextNode;
	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );
	//��ȡˢд����DLL�ļ�·��
	if( pstParam->iLen == 0 )
	{
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "select_fail", 0, pOut );
		return FAILE;
	}
	/* �洢·�� ���ڵ���DLL�� */
	for( i=0; i<5; i++) 
	{
		g_ReprogramDllFilePath[i] =( byte*)malloc(pstParam->iLen + 1) ;
		memcpy(g_ReprogramDllFilePath[i],pstParam->pcData,pstParam->iLen);
		g_ReprogramDllFilePath[i][pstParam->iLen] = '\0'; 
	}
	g_ReprogramDllFilePath_Decryption =( byte*)malloc(pstParam->iLen + 1) ;
	memcpy(g_ReprogramDllFilePath_Decryption,pstParam->pcData,pstParam->iLen);
	g_ReprogramDllFilePath_Decryption[pstParam->iLen] = '\0'; 

	g_ReprogramDllFilePath_Key =( byte*)malloc(pstParam->iLen + 1) ;
	memcpy(g_ReprogramDllFilePath_Key,pstParam->pcData,pstParam->iLen);
	g_ReprogramDllFilePath_Key[pstParam->iLen] = '\0'; 

	//�����ã�������ȫˢ
	g_ChooseReprogramBlock = 0;

	return SUCCESS;
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
int repro_make_file_data(void* pOut)
{
	int i,j;
	int bStatus = FAILE;
	
	g_ReprogramFileType = 0;	//��ѡˢд�ļ���ʽ 1:RPM 2:HEX(ԭ��) 3:EOL

	for (i=0;;i++)
	{
		if (g_ReprogramFilePath[i] == '\0')
		{
			for (j=i; j>4; j--)
			{				
				if ((g_ReprogramFilePath[j-3] == '.')&&(g_ReprogramFilePath[j-2] == 'r')
					&&(g_ReprogramFilePath[j-1] == 'p')&&(g_ReprogramFilePath[j] == 'm'))	
				{		
					g_ReprogramFileType = 1; //�ж��ļ���׺�Ƿ�Ϊ.rpm
					break;
				}
				else if ((g_ReprogramFilePath[j-3] == '.')&&((g_ReprogramFilePath[j-2] == 'h')||(g_ReprogramFilePath[j-2] == 'H'))
														  &&((g_ReprogramFilePath[j-1] == 'e')||(g_ReprogramFilePath[j-1] == 'E'))
														  &&((g_ReprogramFilePath[j] == 'x')||(g_ReprogramFilePath[j] == 'X')))	
				{		
					g_ReprogramFileType = 2; //�ж��ļ���׺�Ƿ�Ϊ.HEX
					break;
				}
				else if ((g_ReprogramFilePath[j-3] == '.')&&((g_ReprogramFilePath[j-2] == 'e')||(g_ReprogramFilePath[j-2] == 'E'))
													      &&((g_ReprogramFilePath[j-1] == 'o')||(g_ReprogramFilePath[j-1] == 'O'))
														  &&((g_ReprogramFilePath[j] == 'l')||(g_ReprogramFilePath[j] == 'L')))	
				{
					g_ReprogramFileType = 3; //�ж��ļ���׺�Ƿ�Ϊ.eol
					break;
				}
				else if ((g_ReprogramFilePath[j-3] == '.')&&((g_ReprogramFilePath[j-2] == 's')||(g_ReprogramFilePath[j-2] == 'S'))
															&&((g_ReprogramFilePath[j-1] == '1')||(g_ReprogramFilePath[j-1] == '1'))
															&&((g_ReprogramFilePath[j] == '9')||(g_ReprogramFilePath[j] == '9')))	
				{		
					g_ReprogramFileType = 2; //�ж��ļ���׺�Ƿ�Ϊ.s19
					break;
				}
				else if ( (g_ReprogramFilePath[j-2] == '.')&&((g_ReprogramFilePath[j-1] == 'f')||(g_ReprogramFilePath[j-1] == 'F'))
															&&((g_ReprogramFilePath[j] == 'p')||(g_ReprogramFilePath[j] == 'P')))
				{
					g_ReprogramFileType = 4; //�ж��ļ���׺�Ƿ�Ϊ.FP
					break;
				}
				else
				{
					g_ReprogramFileType = 0; //�ļ���׺����
				}
			}
			break;
		}
	}

	switch (g_ReprogramFileType)
	{
	case 0:
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_OPEN_FILE_FAIL", 0, pOut );
		return FAILE;

	case 1:	//rpm
		g_ChooseReprogramBlock = 2;	//�Ƕ�ȡ���ļ�ʱ��ֻ��ˢд����������
		break;

	case 2:	//ԭ��HEX
			bStatus = judge_file_secret_hex(pOut, g_ReprogramFilePath);//���浽��������
			if (bStatus !=SUCCESS)
			{
				special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_OPEN_FILE_FAIL", 0, pOut );
				return FAILE;
			}
		break;
	case 3:	//eol
		bStatus = repro_make_file_data_eol(pOut);//����EOL�ļ������浽������

		if (bStatus != SUCCESS)
		{
			return FAILE;
		}
		break;
	case 4://��������.Fp
			/* ���ýӿڣ����ļ����н��ܲ��� */
			
			File_decryption( g_ReprogramFilePath, g_ReprogramFilePath2 );
			
			bStatus = judge_file_secret_hex(pOut, g_ReprogramFilePath2);//���浽��������
			if (bStatus !=SUCCESS)
			{
				special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_OPEN_FILE_FAIL", 0, pOut );
				return FAILE;
			}

		break;
	default:
		break;
	}

	switch (g_ReprogramFileType)
	{
	case 2:
	case 3:
	case 4:
		//�޸��ļ������Դ洢ԭ���ļ�ת������м��ļ�
		for (i=0;;i++)
		{
			if (g_ReprogramFilePath[i] == '\0')
			{
				g_ReprogramFilePath = realloc(g_ReprogramFilePath,i+5);
				g_ReprogramFilePath[i] = '.';			
				g_ReprogramFilePath[i+1] = 'r';
				g_ReprogramFilePath[i+2] = 'p';
				g_ReprogramFilePath[i+3] = 'm';
				g_ReprogramFilePath[i+4] = '\0';
				break;
			}
		}

		bStatus = repro_change_original_file_to_rpm();//ԭ���ļ���ת��Ϊrpm�ļ�
		if (bStatus != SUCCESS)
		{
			special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_FILE_CHANGE_FAIL", 0, pOut );
			return FAILE;
		}
		break;
	default:
		break;
	}

	bStatus = repro_initialize_rpm_file(pOut);//ͳ��rpm�ļ���Ϣ
	if (bStatus != SUCCESS)
	{
		return FAILE;
	}

	return SUCCESS;
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
int initialize_variable(void)
{
	byte CurrentBlock = 0;

	g_DataSendOver = false;
	ReceiveFileByteNumber = 0;

	//��ʼ��ַ��CB,CODE,DATA
	g_ProgramData[0].cBeginAddress.u32Bit  = REPROGRAM_BEGIN_ADDRESS_00;
	g_ProgramData[1].cBeginAddress.u32Bit  = REPROGRAM_BEGIN_ADDRESS_01;
	g_ProgramData[2].cBeginAddress.u32Bit  = REPROGRAM_BEGIN_ADDRESS_02;
	g_ProgramData[3].cBeginAddress.u32Bit  = REPROGRAM_BEGIN_ADDRESS_03;
	g_ProgramData[4].cBeginAddress.u32Bit  = REPROGRAM_BEGIN_ADDRESS_04;
	//������ַ��CB,CODE,DATA
	g_ProgramData[0].cEndAddress.u32Bit  = REPROGRAM_END_ADDRESS_00;
	g_ProgramData[1].cEndAddress.u32Bit  = REPROGRAM_END_ADDRESS_01;
	g_ProgramData[2].cEndAddress.u32Bit  = REPROGRAM_END_ADDRESS_02;
	g_ProgramData[3].cEndAddress.u32Bit  = REPROGRAM_END_ADDRESS_03;
	g_ProgramData[4].cEndAddress.u32Bit  = REPROGRAM_END_ADDRESS_04;

	for (CurrentBlock=0; CurrentBlock<REPROGRAM_FILE_BLOCKS_NUMBER; CurrentBlock++)
	{
		g_ProgramData[CurrentBlock].cValueDataLen.u32Bit = g_ProgramData[CurrentBlock].cEndAddress.u32Bit - g_ProgramData[CurrentBlock].cBeginAddress.u32Bit + 1;
		g_ProgramData[CurrentBlock].pcData = ( byte * )malloc(g_ProgramData[CurrentBlock].cValueDataLen.u32Bit);
		g_ProgramData[CurrentBlock].AllDataChecksum.u32Bit = 0;
		g_ProgramData[CurrentBlock].FrameTotalNumber.u32Bit = 0;
	}
	return SUCCESS;
}
/*************************************************
Description:	��ȡECU�汾��
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_read_version( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;
	int CMDF194;
	int iCmdSum = 0;
	int ipCmdIndex[2] = {1, 0};
	uint32 u8CmdIndex[10];//���⹦����������
	byte cBufferOffset = 0;//����ƫ��
	int iReceiveResult = TIME_OUT;
	int bStatus;

	iCmdSum  = get_string_type_data_to_uint32( u8CmdIndex, pstParam->pcData, pstParam->iLen );

	CMDF194  = u8CmdIndex[0];

	//��ȡECU�汾
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMDF194].cBufferOffset;
	ipCmdIndex[1] = CMDF194;
	iReceiveResult = send_and_receive_cmd( ipCmdIndex );

	if( iReceiveResult != SUCCESS )
	{
		general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
		return iReceiveResult;
	}

	memcpy( ECUVersionReceive, g_stBufferGroup[cBufferOffset].cBuffer + 3, 9 );

	//��ʾECU�汾��Ϣ������ʾ�Ƿ����ˢд����
	bStatus = special_return_status( PROCESS_OK | NO_JUMP | HAVE_TIPS_DATA, NULL, NULL, 2, pOut );
	bStatus = add_data_and_controlsID( bStatus, "test_success", 12, ORIGINAL,"ID_STR_YES", pOut );
	bStatus = add_data_and_controlsID( bStatus, "return", 6, ORIGINAL, "ID_STR_NO", pOut );
	bStatus = add_data_and_controlsID( bStatus, ECUVersionReceive, 0x09, ORIGINAL, "ID_STR_ECU_VERSION", pOut );
	bStatus = add_data_and_controlsID( bStatus, NULL, 0, 0, "ID_STR_REPROGRAM_READ_CHOICE", pOut );

	return iReceiveResult;
}
/*************************************************
Description:	��ȡECUˢд����
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_read_data( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;
	int CMD2323;
	int iCmdSum = 0;
	int ipCmdIndex[2] = {1, 0};
	uint32 u8CmdIndex[10];//���⹦����������
	byte cBufferOffset = 0;//����ƫ��
	int iReceiveResult = TIME_OUT;
	byte CurrentBlock = 0;
	uint32 ReceiveDataNumber;	//�ѽ������ݵĳ���
	uint32 NotReceiveDataNumber;	//ʣ�����ݵĳ���
	UNN_2WORD_4BYTE ReadDataAddress = {0}; //��ȡ���ݵ�ַ
	byte UpdateProgressTemp;
	byte UpdateProgressLen = 0;
	byte UpdateProgress[20] = {0};
	byte flag = 0;
	uint32 BlockSize = 0;

	iCmdSum  = get_string_type_data_to_uint32( u8CmdIndex, pstParam->pcData, pstParam->iLen );

	CMD2323  = u8CmdIndex[0];

	update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAM_PREPARE", 24 );
	update_ui( VDI_UPDATE_PROGRESS, "0" , 1 );

	//��ȡECU����
	cBufferOffset = g_stInitXmlGobalVariable.m_p_stCmdList[CMD2323].cBufferOffset;
	ipCmdIndex[1] = CMD2323;

	for (CurrentBlock=g_ChooseReprogramBlock; CurrentBlock<REPROGRAM_FILE_BLOCKS_NUMBER; CurrentBlock++)
	{
		ReceiveDataNumber = 0;	//��ȡECU���ݼ���
		ReadDataAddress.u32Bit = g_ProgramData[CurrentBlock].cBeginAddress.u32Bit;
		
		while (1)	//ʣ�����ݴ��ڵ���REPROGRAM_READ_DATA_SIZE
		{
			switch (ReadDataAddress.u32Bit)
			{
			case 0x8003FBE2:	//�����������м䣬ȡ������֡����
				BlockSize = 0x22;
				break;
			
			case 0x8007FBB0:    //�����������м䣬ȡ������֡����
				BlockSize = 0x54;
				break;

			case 0x8007FC04:	//��ȡʱ���������ݣ�ECU��֧�ִ˿�Ķ�ȡ����Ĭ��ֵ
			case 0x8003FC04:
				for (i = 0;i < 0xFE00 - 0xFC04; i++)
				{
					g_ProgramData[CurrentBlock].pcData[ReceiveDataNumber++] = 0x00;
					ReadDataAddress.u32Bit++;
				}
				for (i = 0;i < 0xFF00 - 0xFE00; i++)
				{
					g_ProgramData[CurrentBlock].pcData[ReceiveDataNumber++] = 0xAF;
					ReadDataAddress.u32Bit++;
				}
				for (i = 0;i < 0xFFFF - 0xFF00 +1 ; i++)
				{
					g_ProgramData[CurrentBlock].pcData[ReceiveDataNumber++] = 0x00;
					ReadDataAddress.u32Bit++;
				}

				BlockSize = REPROGRAM_READ_DATA_SIZE;
				break;

			default:
				BlockSize = REPROGRAM_READ_DATA_SIZE;
				break;
			}

			NotReceiveDataNumber = g_ProgramData[CurrentBlock].cValueDataLen.u32Bit - ReceiveDataNumber;

			if (NotReceiveDataNumber == 0x00)
			{
				break;
			}
			//��������Ӷ�ȡ���ݵĵ�ַ
			for (i=0; i<3; i++)		
			{
				g_stInitXmlGobalVariable.m_p_stCmdList[CMD2323].pcCmd[7+i] = ReadDataAddress.u8Bit[2-i];
			}
			
			//����������������ݳ���
			g_stInitXmlGobalVariable.m_p_stCmdList[CMD2323].pcCmd[11] = BlockSize;

			iReceiveResult = send_and_receive_cmd( ipCmdIndex );
			if( iReceiveResult != SUCCESS )
			{
				general_return_status( iReceiveResult, g_stBufferGroup[cBufferOffset].cBuffer, 3, pOut );
				return iReceiveResult;
			}
						
			memcpy( g_ProgramData[CurrentBlock].pcData + ReceiveDataNumber, g_stBufferGroup[cBufferOffset].cBuffer + 1, BlockSize );
					
			ReadDataAddress.u32Bit += BlockSize;	//��ȡ���ݵ�ַ�ݼ�
			ReceiveDataNumber += BlockSize;
			
			UpdateProgressTemp = ReceiveDataNumber*100/g_ProgramData[CurrentBlock].cValueDataLen.u32Bit;
			UpdateProgressLen = sprintf( UpdateProgress, "%d", UpdateProgressTemp);
		
			//���½���
			update_ui( VDI_UPDATE_TIP, "ID_STR_REPROGRAM_READ_DATA_TIP1" , 31 );
			update_ui( VDI_UPDATE_PROGRESS, UpdateProgress , UpdateProgressLen );
		}
	}
	
	special_return_status( PROCESS_OK | HAVE_JUMP | NO_TIP, "test_success", NULL, 0, pOut );

	return SUCCESS;
}
/*************************************************
Description:	�����ļ�
Input:
pIn		������ECUˢд�йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
int reprogram_read_save_file( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut )
{
	int i = 0;
	int bStatus = FAILE;
	int FileLen = 0;
	char CurrentData[8];
	char FilePostfix[] = {".rpm"};
	int TempLen = 0;

	//��ȡˢд�ļ�·��
	g_ReprogramFilePath =( byte* )malloc(pstParam->iLen) ;
	if( pstParam->iLen == 0 )
	{
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_SELECT_FILE_FAIL", 0, pOut );
		return FAILE;
	}
	
	FileLen = pstParam->iLen;

	memcpy(g_ReprogramFilePath ,pstParam->pcData,FileLen);

	//��ȡ�ļ�����
	pstParam = pstParam->pNextNode;
	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );
	
	g_ReprogramFilePath = realloc(g_ReprogramFilePath,FileLen + pstParam->iLen);

	memcpy(g_ReprogramFilePath + FileLen, pstParam->pcData, pstParam->iLen);
	
	FileLen += pstParam->iLen;
	
	//�޸��ļ����ƣ����ϵͳ������

	get_programe_date( CurrentData );

	TempLen = sizeof(REPROGRAM_SYSTEM_NAME)/sizeof(REPROGRAM_SYSTEM_NAME[0]) - 1;
	g_ReprogramFilePath = realloc(g_ReprogramFilePath,FileLen + TempLen);

	memcpy(g_ReprogramFilePath + FileLen, REPROGRAM_SYSTEM_NAME, TempLen);

	FileLen += TempLen;
	
	//���»��ߡ�������
	TempLen = 8;

	g_ReprogramFilePath = realloc(g_ReprogramFilePath,FileLen + TempLen + 1);

	g_ReprogramFilePath[FileLen] = '_';
	FileLen++;

	memcpy(g_ReprogramFilePath + FileLen, CurrentData, TempLen); //������

	FileLen += TempLen;
	
	//�Ӻ�׺
	TempLen = sizeof(FilePostfix)/sizeof(FilePostfix[0]) - 1;
	g_ReprogramFilePath = realloc(g_ReprogramFilePath,FileLen + TempLen + 1);

	memcpy(g_ReprogramFilePath + FileLen, FilePostfix, TempLen);

	FileLen += TempLen;

	g_ReprogramFilePath[FileLen] = '\0';

	//�յ���ECU�����ļ���ת��Ϊrpm�ļ�
	bStatus = repro_change_original_file_to_rpm();
	if (bStatus != SUCCESS)
	{
		special_return_status( PROCESS_FAIL | NO_JUMP | HAVE_TIP, NULL, "ID_STR_FILE_CHANGE_FAIL", 0, pOut );
		return FAILE;
	}

	//�ͷ�ˢд�ļ��ڴ�
	for (i=0; i<REPROGRAM_FILE_BLOCKS_NUMBER; i++)
	{
		if(NULL != g_ProgramData[i].pcData)
			free( g_ProgramData[i].pcData );
	}

	//�ͷ��ļ�·���ڴ�
	if(NULL != g_ReprogramFilePath)
		free( g_ReprogramFilePath );

	special_return_status( PROCESS_FAIL | HAVE_JUMP | NO_TIP, "test_success", NULL, 0, pOut );
	return SUCCESS;
}
/*************************************************
Description:	ECUˢд������
Input:
pIn		���������⹦���йص���������
		�ʹ�UI���������
Output:	pOut	������ݵ�ַ
Return:	void
Others:	���ݵ�һ����������ִ�в�ͬ�Ĺ��ܺ���
*************************************************/
void process_reprogram_function( void* pIn, void* pOut )
{
	byte cReprogramCmdData[40] = {0};//������⹦����������
	STRUCT_CHAIN_DATA_INPUT* pstParam = ( STRUCT_CHAIN_DATA_INPUT* )pIn;
	byte FunctionSlect = 0;
	int bStatus = FAILE;

	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );

	get_string_type_data_to_byte( cReprogramCmdData, pstParam->pcData, pstParam->iLen );

	//ָ����һ�����
	pstParam = pstParam->pNextNode;
	assert( pstParam->pcData );
	assert( pstParam->iLen != 0 );

	/* ECUˢд�ĺ������ EMS(����) */
	switch( cReprogramCmdData[0] )
	{
	case 0:		//ˢд����ǰ����	
		switch (g_VDILinkStatus)
		{
		case 0x01:	//USBģʽ
			bStatus = repro_make_file_data(pOut); //�ļ���������ԭ���ļ���ת��Ϊrpm�ļ�
			if (bStatus != SUCCESS)
			{
				return;
			}
			
 			bStatus = repro_send_link_mode(pOut);//��VDI�������ӷ�ʽ��USB������
 			if (bStatus != SUCCESS)
 			{
 				return;
 			}

			bStatus = reprogram_write_enter( cReprogramCmdData, pstParam, pOut );
			if (bStatus != SUCCESS)	//�쳣ʱ��ɾ���м��ļ�
			{
				reprogram_exit_operate();
				return;
			}

			break;
		case 0x02:	//����ģʽ
			if (g_DataSendOver == false)	//����δ���͸�VDI
			{
				bStatus = repro_make_file_data(pOut); //�ļ���������ԭ���ļ���ת��Ϊrpm�ļ�
				if (bStatus != SUCCESS)
				{
					return;
				}

				bStatus = repro_send_link_mode(pOut);//��VDI�������ӷ�ʽ��USB������
				if (bStatus != SUCCESS)
				{
					return;
				}
			}
			else	//������ȫ�����͸�VDI
			{
				time_delay_ms(1000);
				bStatus = reprogram_write_enter( cReprogramCmdData, pstParam, pOut );
				if (bStatus != SUCCESS)	//�쳣ʱ��ɾ���м��ļ�
				{
					reprogram_exit_operate();
					return;
				}
			}			
		}
		break;

	case 1:		//���������ļ�
		bStatus = reprogram_write_send( cReprogramCmdData, pstParam, pOut );
		
		if (bStatus != SUCCESS)	//�쳣ʱ��ɾ���м��ļ�
		{
			reprogram_exit_operate();
			return;
		}
		break;

	case 2:		//ˢд��ϣ��˳�ϵͳ
		bStatus = reprogram_write_exit( cReprogramCmdData, pstParam, pOut );
		break;

	case 3:
		//��ȡ��λ�����͵��ļ�·��������
		bStatus = read_ecu_file_way(cReprogramCmdData, pstParam, pOut);//��ȡ�ļ�·��	
		if (bStatus != SUCCESS)
		{
			return;
		}
		initialize_variable();	//������ʼ��
		//��ʾECU�汾��Ϣ������ʾ�Ƿ����ˢд����
		bStatus = special_return_status( PROCESS_OK | NO_JUMP | HAVE_TIPS_DATA, NULL, NULL, 2, pOut );
 	  	bStatus = add_data_and_controlsID( bStatus, "test_success", 12, ORIGINAL,"ID_STR_YES", pOut );
		bStatus = add_data_and_controlsID( bStatus, "return", 6, ORIGINAL, "ID_STR_NO", pOut );
		bStatus = add_data_and_controlsID( bStatus, NULL, 0, 0, "ID_STR_REPROGRAM_WRITE_CHOICE", pOut );
		break;
	case 4:		//��ȡECU�汾��
		bStatus = reprogram_read_version( cReprogramCmdData, pstParam, pOut );
		break;
	case 5:		//��ȡECUˢд�����ļ�
		g_ChooseReprogramBlock = 2;	//ECUֻ֧�ֵ��������ݵĶ�ȡ
		initialize_variable();	//������ʼ��

		bStatus = reprogram_read_data( cReprogramCmdData, pstParam, pOut );
		break;
	
	case 6:		//�����ȡ����ECU�����ļ�		
		bStatus = reprogram_read_save_file( cReprogramCmdData, pstParam, pOut );
		break;

	default:
		break;
	}
}
