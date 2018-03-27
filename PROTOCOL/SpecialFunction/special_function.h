/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�������⹦�ܺ�����������غ����������
History:
	<author>	<time>		<desc>

************************************************************************/
#ifndef _SPECIAL_FUNCTION
#define _SPECIAL_FUNCTION

#include "../interface/protocol_define.h"
#include "../command/command.h"
#include "../public/public.h"
#include "../public/protocol_config.h"
#include "../protocol/iso_14230.h"
#include "../protocol/sae_1939.h"
#include "../formula/formula.h"
#include "../formula/formula_comply.h"

#define COMM_INTERRUPT	0X02
#define PROCESS_OK		0X01
#define PROCESS_FAIL	0X00

#define HAVE_JUMP		0X04
#define NO_JUMP			0X00

#define HAVE_TIP_DATA	0X10 //��ʾ��ʱ����������������ʾ����
#define HAVE_TIP		0X08
#define NO_TIP			0X00
#define HAVE_TIPS_DATA	0X18 //֧�ֵ��л������ʾ��ʾ��Ϣ+protocol��������

//�ϴ���������
enum UPLOAD_DATA_TYPE
{
    HEX_PRINT = 0,
    DEC_PRINT,
    ORIGINAL,
	DEC_COMBIN,
};
extern uint8 u8CalibrationFunctionJudge[20];  //��ű궨�����жϵı�־

/********************************ͨ�ú�������**************************************/
void process_special_function( void* pIn, void* pOut );
int get_string_type_data_to_uint32( uint32 *pu32Dest, const byte* pcSource, const int iMaxLen );
int get_string_type_data_to_byte( byte *pcDest, const byte* pcSource, const int iMaxLen );
int special_return_status( const byte cTipMode, const byte* pcLable, const byte* pcTipContent, byte cButtonNum, void* pOut );
int add_data_and_controlsID( const int iAppendDataOffset, const byte* pcData, const byte cDataLen,
                             const int iUploadDataType, const byte* pcControlsID, void* pOut );

/********************************���幦�ܺ�������***********************************/



#endif