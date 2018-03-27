/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�������⹦�ܺ�����������غ����������
History:
	<author>	<time>		<desc>

************************************************************************/
#ifndef _REPROGRAM_FUNCTION
#define _REPROGRAM_FUNCTION

#include "../interface/protocol_define.h"
#include "../command/command.h"
#include "../public/public.h"
#include "../public/protocol_config.h"
#include "../protocol/iso_14230.h"
#include "../protocol/iso_15765.h"
#include "../formula/formula.h"
#include "../formula/formula_comply.h"

#define VDI_UPDATE_TIP       0x00 //VDI�������е���һ������ʾ
#define VDI_UPDATE_PROGRESS  0x01 //VDI�����Ľ���

//ˢд�ļ����ܿ���
#define REPROGRAM_FILE_BLOCKS_NUMBER 0x05  

//ˢд�ļ�����ʼ�ڽ�����ַ
#define REPROGRAM_BEGIN_ADDRESS_00	0x80000000
#define REPROGRAM_END_ADDRESS_00	0x8000BFFF

#define REPROGRAM_BEGIN_ADDRESS_01	0x8001C000
#define REPROGRAM_END_ADDRESS_01	0x8001FFFF

#define REPROGRAM_BEGIN_ADDRESS_02	0x8000C000
#define REPROGRAM_END_ADDRESS_02	0x8001BFFF

#define REPROGRAM_BEGIN_ADDRESS_03	0x80040000
#define REPROGRAM_END_ADDRESS_03	0x800FFFFF

#define REPROGRAM_BEGIN_ADDRESS_04	0x80020000
#define REPROGRAM_END_ADDRESS_04	0x8003FFFF

#define REPROGRAM_EACH_BLOCK_SIZE	0xFFF - 2
#define REPROGRAM_READ_DATA_SIZE	0xFFF

//#define SECRET_KEY	'F'
#define SECRET_KEY	0

#define REPROGRAM_SYSTEM_NAME	"_EDC17V720-"	//��ǰϵͳ�����ɵ�rpmˢд�ļ�������

typedef struct
{
	bool cDataFlag;	//��־λ
	UNN_2WORD_4BYTE  FrameTotalNumber;  //ÿ�����ݵ���֡��
	UNN_2WORD_4BYTE  AllDataChecksum;   //ÿ�����ݵ�У��ͣ�F0֡������VDI��У��
	UNN_2WORD_4BYTE  ValueDataChecksum; //ÿ�����ݵ�У��ͣ������ݣ�����ECU��У��
	UNN_2WORD_4BYTE cValueDataLen;	//ÿ�����ݵĴ������ܳ���
	UNN_2WORD_4BYTE cBeginAddress;
	UNN_2WORD_4BYTE cEndAddress;
	byte* pcData;
} STRUCT_REPROGRAM_DATA;	//���ˢд�ļ����ݵĽṹ��

enum security_mode {
	sa_diactivated			=10,
	sa_conti_standard_cus	=20,		/* Conti standard customer SA algorithm */
	sa_customer_reprog		=30,		/* Customer reprog mode */
	sa_customer_dev			=40,		/* Customer development mode */
	sa_other_modes			=50			/* Other possible modes */
};


extern uint32 ReceiveFileByteNumber;
extern byte g_VDILinkStatus;
extern byte g_NegativeCode[3];	//���������Ӧֵ
extern uint16 g_seedCMDBF[2];
extern byte g_LogisticID[18];

extern STRUCT_REPROGRAM_DATA g_ProgramData[REPROGRAM_FILE_BLOCKS_NUMBER];
extern byte * g_ReprogramDllFilePath_Decryption;//�ļ�����Dll·��
extern byte * g_ReprogramDllFilePath_Key;//�ļ�����Dll·��
extern byte * g_ReprogramDllFilePath[5];//�ļ�����Dll·��
extern byte * g_ReprogramFilePath;	//�ļ�·��
extern byte * g_ReprogramFilePath1;	//�ļ�·��
extern byte * g_ReprogramFilePath2;	//�ļ�·��  ����Fp�ļ�

extern byte g_ChooseReprogramBlock; //ָ��������һ������
extern bool g_DataSendOver;	//�����Ƿ��ѷ���
extern bool g_VDIUploadSchedule;//VDI�ϴ�ˢд���ȣ�����ͨѶʱ��
extern byte *pcPackageCmdCache;	//����ú������洢��
extern byte g_CurrentBlockNumber;	//��ǰ��������
extern byte g_ReprogramFileType;	//��ѡˢд�ļ���ʽ 1:RPM 2:HEX(ԭ��) 3:EOL 
extern 	byte ECUVersionReceive[10];	//ECU�汾�� 
extern char *g_Secretkey ; //ԭʼ�ļ�������Կ
extern 	UNN_2WORD_4BYTE CheckSum_Address;



/********************************���幦�ܺ�������***********************************/
void process_reprogram_function( void* pIn, void* pOut );
int reprogram_write_enter( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut );
int reprogram_write_send( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut );

int reprogram_send_usb( byte CurrentBlock,uint32* u32CmdIndex,void * pOut );
int reprogram_send_bluetooth( byte CurrentBlock,uint32* u32CmdIndex,void * pOut );
int reprogram_write_exit( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut );

bool enter_system(void);
byte asc_to_hex(byte FileData);
byte merge_two_byte(byte FileData1,byte FileData2);
byte send_cmd_checksum(byte *buffer,int len);
int repro_send_link_mode(void* pOut);
// int repro_start_cmd(void);
int repro_checksum_and_cmd_sum(byte CurrentBlock);
int repro_start_transfer_data(void);
int repro_stop_transfer_data(void);
int repro_config_negative_response(byte NegativeControlStatus);
int repro_config_insert_Framing_response(byte NegativeControlStatus);
int read_ecu_file_way( byte * cSpecialCmdData, const STRUCT_CHAIN_DATA_INPUT* pstParam, void * pOut );
int repro_config_current_file_block(byte CurrentFileBlock);
int repro_make_file_data_HEX(void* pOut, byte *Filepath);
int repro_change_original_file_to_rpm(void);
int repro_initialize_rpm_file(void* pOut);
void _Encryption( char* pData, int nLen );
int repro_make_file_data(void* pOut);
void reprogram_exit_operate( void );
void get_programe_date( char *DateTemp );
byte merge_two_byte_and_secret(byte *FileData1,byte *FileData2);
int initialize_variable(void);
int repro_make_file_data_eol(void* pOut);
int repro_make_file_data_S19(void* pOut);
int judge_file_secret_hex(void* pOut, byte *Filepath);
void Encryption(char* vDataBuffer,const int nSize,const char* pSeed,const int nSeedLen,int nCount);

typedef int ( * lpEnableSADll)( const char*, int, void* );
typedef int ( * lpLoadSeed2Dll)( const unsigned char *, const unsigned char );
typedef int ( * lpGetSaKey)( unsigned char *, unsigned char * );
typedef unsigned long ( * MYPROC)( unsigned long Seed );
typedef unsigned long ( * MYPROC_CheckSum)( byte *pBuffer,unsigned long u32Size );
typedef unsigned long ( * MYPROC_DecryptHex)( byte *srcFileName, byte *tgtFileName );
unsigned long  Dll_Key(unsigned long Seed);
#endif