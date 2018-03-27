/**********************************************************************
Copyright:		YiXiong AUTO S&T Co., Ltd.
Description:	�������ISO15765Э�鴦���շ����ݵ���غ���
History:
	<author>	<time>		<desc>

************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sae_1939.h"
#include "iso_15765.h"
#include "../interface/protocol_interface.h"
#include "../public/protocol_config.h"
#include "../command/command.h"
#include "../InitConfigFromXml/init_config_from_xml_lib.h"
#include "../function/idle_link_lib.h"

/*************************************************
Description:	����SAE1939Э���շ�����
Input:	piCmdIndex	����������ַ

Output:	none
Return:	int	�շ�����ʱ��״̬
Others:	���������������Է��ͺͽ��ն������
		��ECU������Ӧ�����ݷŵ�������Ӧ�Ļ�����
ע�⣺������ECU�����Ǵ�SID�Ļظ���ʼ�ģ��磺
7E0  03 22 15 08 00 00 00 00
7E8  04 62 15 08 79 00 00 00
�Ǵ�62��ʼ���棬��Ϊ������Ӧ���7F��ʼ���档
*************************************************/
int send_and_receive_cmd_by_sae_1939( const int* piCmdIndex )
{
	int iStatus = TIME_OUT;
	int i = 0;
	int iCmdSum = piCmdIndex[0];
	byte cReceiveBuffer[50] = {0};
	STRUCT_CMD stSendCmd = {0};

	for( i = 0; i < iCmdSum; i++ )
	{
		iStatus = send_and_receive_single_cmd_by_sae_1939( ( STRUCT_CMD * )&stSendCmd, piCmdIndex[1 + i], cReceiveBuffer );

		if( NULL != stSendCmd.pcCmd )
			free( stSendCmd.pcCmd );

		if( ( iStatus != SUCCESS ) && ( iStatus != NEGATIVE ) ) //���״̬�Ȳ���SUCCESS�ֲ���NEGATIVE����Ϊ����
		{
			break;
		}
        if (iCmdSum != 1)
        {
			time_delay_ms( g_p_stSAE1939Config->u16TimeBetweenFrames );
        }
		
	}

	return iStatus;
}
/*************************************************
Description:	�շ����������
Input:
	cCmdIndex		��������
	pcReceiveBuffer	���ECU�ظ����ݵĻ���

Output:	none
Return:	int	�շ�����ʱ��״̬
Others:	����SAE1939Э�鴦��
*************************************************/
int send_and_receive_single_cmd_by_sae_1939( STRUCT_CMD *pstSendCmd, const int cCmdIndex, byte* pcReceiveBuffer )
{

	int		iReceiveStatus = TIME_OUT;
	byte	cRetransTime			= g_p_stSAE1939Config->cRetransTime;
	bool	bSendStatus = false;

	pstSendCmd->cBufferOffset	= g_stInitXmlGobalVariable.m_p_stCmdList[cCmdIndex].cBufferOffset;
	pstSendCmd->cReserved		= g_stInitXmlGobalVariable.m_p_stCmdList[cCmdIndex].cReserved;
	pstSendCmd->iCmdLen			= g_stInitXmlGobalVariable.m_p_stCmdList[cCmdIndex].iCmdLen;
	pstSendCmd->pcCmd			= ( byte* )malloc( sizeof( byte ) * pstSendCmd->iCmdLen );
	memcpy( pstSendCmd->pcCmd, g_stInitXmlGobalVariable.m_p_stCmdList[cCmdIndex].pcCmd, pstSendCmd->iCmdLen );



	
	while( 1 )
	{
		bSendStatus = SAE1939_package_and_send_frame( FRAME_HEAD_SAE1939_FRAME, pstSendCmd );

		if( !bSendStatus )
		{
			return FAILE;
		}

		iReceiveStatus = process_1939_receive_Cmd( pstSendCmd, pcReceiveBuffer );

		switch( iReceiveStatus )
		{
		case TIME_OUT:
		{
			if( ( --cRetransTime ) == 0 )
				return iReceiveStatus;


		}
		break;

		case SUCCESS:
		case FRAME_TIME_OUT:
		case NEGATIVE:

		default:
			return iReceiveStatus;

		}
	}
}
/*************************************************
Description:	����1939�Ľ���
Input:	cBufferOffset	�洢����ƫ��
		pcSource		����ECU�ظ����ݵĻ���

Output:	none
Return:	int	�շ�����ʱ��״̬
Others:	����SAE1939Э�鴦����գ�����
		cBufferOffset��Ӧ����g_stBufferGroup
		��pcSource�ǵ���receive_cmd����ʱ�ݴ�
		���ݵĵ�ַ��
*************************************************/
int process_1939_receive_Cmd( STRUCT_CMD *pstSendCmd, byte* pcSource )
{
	int		i = 0;
	byte	cValidLen = 0;//
	bool	bReceiveStatus	= false;
	int     receive_times = g_p_stSAE1939Config->u16ECUResTimeout/100;
	int     idNumer = 0;
	uint16  u16receive_id = 0;
	uint16  u16Filter[10] = {0};
    byte frametiem =0;

// 03 (18 EA FF F1) (18 EC FF 00) (18 EB FF 00) (01) (07) (18 EA FF F1 D3 FE 00)
	idNumer = pstSendCmd->pcCmd[0];
	for (i = 0; i < idNumer; i++)
	{
		u16Filter[i] = (uint16)((pstSendCmd->pcCmd[i*4+2]<<8) | pstSendCmd->pcCmd[i*4+3]);
	}
	while(1)
	{
		do 
		{
			if (!(( bool )receive_cmd( &cValidLen, 1, 100 )))
			{
				continue;
			}
			else
			{
				break;
			}
		} while (receive_times--);

		if (receive_times <= 0)
		{
			return TIME_OUT;
		}             // �յ�һ���ֽ�  

	
		do 
		{
			if (!(( bool )receive_cmd( pcSource, cValidLen+4, 100 )))
			{
				continue;
			}
			else
			{
				break;
			}
		} while (receive_times--);

		if (receive_times <= 0)
		{
			
			return TIME_OUT;
		}              // �յ�ȫ������

		u16receive_id = (uint16)((pcSource[1]<<8) | pcSource[2]);
      
		if (u16receive_id == u16Filter[0])  //�ظ���֡
		{
			save_SEA1939_single_data(pstSendCmd,pcSource+4);
			return SUCCESS;	
		} else if (u16receive_id == u16Filter[idNumer-3])  //4���˲�ʱ�ڶ���ID�յ�֡
		{
			save_SEA1939_single_data_Reply(pstSendCmd,pcSource+4);
			return SUCCESS;	
		} else if(u16receive_id == u16Filter[idNumer-2]) //�ظ���֡�ĵ�һ֡ 20֡
		{
			save_SEA1939_20_frame_data(pstSendCmd,pcSource+4);
			frametiem = 0;
		}else if(u16receive_id == u16Filter[idNumer-1]) //��������ж�֡
		{
			frametiem = save_SEA1939_Multi_Frame_data(pstSendCmd,pcSource+4,frametiem);
		   if (g_stInitProtocolGobalVariable.bGlobal_SEA1939_Multi_Frame_status)
		   {
			   g_stInitProtocolGobalVariable.bGlobal_SEA1939_Multi_Frame_status = false;
			   return SUCCESS;
		   }
		} 
	}
	
    return SUCCESS;

}


void save_SEA1939_single_data(STRUCT_CMD *pstSendCmd,byte* pcSource)
{
	byte cBufferOffset = pstSendCmd->cBufferOffset;
	byte*	pcDestination		= g_stBufferGroup[cBufferOffset].cBuffer;
	g_stBufferGroup[cBufferOffset].iValidLen = 8; //������֡
	memcpy(pcDestination,pcSource,8);

}



void save_SEA1939_single_data_Reply(STRUCT_CMD *pstSendCmd,byte* pcSource)
{
	byte cBufferOffset = pstSendCmd->cBufferOffset;
	byte*	pcDestination		= g_stBufferGroup[cBufferOffset].cBuffer;
	uint16  u16SendID = 0;
	uint16  u16ReceiveID = 0;
	u16ReceiveID = (uint16)((pcSource[6]<<8)|pcSource[5]);//PGN
	u16SendID = (uint16)((pstSendCmd->pcCmd[2]<<8) | pstSendCmd->pcCmd[3]);
	if (u16SendID == u16ReceiveID)
	{
		g_stBufferGroup[cBufferOffset].iValidLen = 8; //������֡
		memcpy(pcDestination,pcSource,8);
	}
}

void save_SEA1939_20_frame_data(STRUCT_CMD *pstSendCmd,byte* pcSource)
{
	byte cBufferOffset = pstSendCmd->cBufferOffset;
	byte*	pcDestination		= g_stBufferGroup[cBufferOffset].cBuffer;
    uint16  u16SendID = 0;
	uint16  u16ReceiveID = 0;
	if (g_stInitProtocolGobalVariable.bGlobal_SEA1939_20Frame_status == false)
	{
		
        u16ReceiveID = (uint16)((pcSource[6]<<8)|pcSource[5]);//PGN
        u16SendID = (uint16)((pstSendCmd->pcCmd[2]<<8) | pstSendCmd->pcCmd[3]);
		if (u16SendID == u16ReceiveID)
        {
			g_stInitProtocolGobalVariable.bGlobal_SEA1939_Multi_Frame_Package_Number = pcSource[3]; //���ն�֡�İ���
			g_stInitProtocolGobalVariable.u16Global_SEA1939_Multi_Frame_ID = u16ReceiveID;
			g_stInitProtocolGobalVariable.bGlobal_SEA1939_20Frame_status = true;
			g_stBufferGroup[cBufferOffset].iValidLen = (int)((pcSource[2]<<8)|pcSource[1]);//
			memcpy(pcDestination+g_stBufferGroup[cBufferOffset].iValidLen,pcSource+1,7);
			
		}
	}

}

byte save_SEA1939_Multi_Frame_data(STRUCT_CMD *pstSendCmd,byte* pcSource,byte frametiem)
{
	byte cBufferOffset = pstSendCmd->cBufferOffset;
	byte*	pcDestination		= g_stBufferGroup[cBufferOffset].cBuffer;
	
	if (g_stInitProtocolGobalVariable.bGlobal_SEA1939_20Frame_status == true) //�Ѿ��յ�20֡
	{
		frametiem++;
		if (frametiem == pcSource[0])
		{
			
			if (frametiem == g_stInitProtocolGobalVariable.bGlobal_SEA1939_Multi_Frame_Package_Number)
			{
				g_stInitProtocolGobalVariable.bGlobal_SEA1939_20Frame_status = false;
				g_stInitProtocolGobalVariable.bGlobal_SEA1939_Multi_Frame_status = true; //����
				memcpy(pcDestination+(frametiem-1)*7, pcSource+1, g_stBufferGroup[cBufferOffset].iValidLen-7*(frametiem-1));
			}
			else
			{
				memcpy(pcDestination+(frametiem-1)*7,pcSource+1,7);
				g_stInitProtocolGobalVariable.bGlobal_SEA1939_Multi_Frame_status = false;
			}
		}
		else
		{
			frametiem--;

		}
	}
	return frametiem;

}


/*************************************************
Description:	�������������֡
Input:
	cFrameHead		����֡ͷ
	pstFrameContent	��������ṹ��ָ��
	cReservedByte	�����б����ֽ�

Output:	none
Return:	bool	���ط���״̬���ɹ���ʧ�ܣ�
Others:	�ú����᳢�Է������Σ������շ�װ��
		�ظ��������жϷ���״̬��
*************************************************/

bool SAE1939_package_and_send_frame( const byte cFrameHead, STRUCT_CMD* pstFrameContent )
{
	UNN_2WORD_4BYTE uFrameLen;
	//bool bReturnStatus = false;
	byte *pcSendCache = NULL;
	byte cReceiveCache[20] = {0};//���յĻ���
	byte cCheckNum = 0;
	int i = 0;
    int receive_times = 0;
    byte cValidLen = 0;//

    receive_times = g_p_stSAE1939Config->u16VDI5ByteTimeout/100;

	uFrameLen.u32Bit = 1 + 2 + 1 + pstFrameContent->iCmdLen + 1;

	pcSendCache = ( byte * )malloc( ( uFrameLen.u32Bit ) * sizeof( byte ) ); //����֡�Ļ���

	pcSendCache[0] = cFrameHead;
	pcSendCache[1] = uFrameLen.u8Bit[1];
	pcSendCache[2] = uFrameLen.u8Bit[0];
	pcSendCache[3] = pstFrameContent->cReserved;

	memcpy( &pcSendCache[4], pstFrameContent->pcCmd, pstFrameContent->iCmdLen );

	for( i = 0; ( i < ( int )uFrameLen.u32Bit - 1 ); i++ )
	{
		cCheckNum += pcSendCache[i];
	}

	pcSendCache[ uFrameLen.u32Bit - 1] = cCheckNum;

	send_cmd( pcSendCache, uFrameLen.u32Bit );
    
	while (1)
	{
		do 
		{
			if (!(( bool )receive_cmd( &cValidLen, 1, 100 )))
			{
				continue;
			}
			else
			{
				break;
			}
		} while (receive_times--);

		if (receive_times <= 0)
		{
			free( pcSendCache );
			return false;
		}             // �յ�һ���ֽ�  
		
        if (cValidLen == 0)
        {
			break; //�յ�ȷ���ֽ�
        }

		do 
		{
			if (!(( bool )receive_cmd( cReceiveCache, cValidLen+4, 100 )))
			{
				continue;
			}
			else
			{
				break;
			}
		} while (receive_times--);

		if (receive_times <= 0)
		{
			free( pcSendCache );
			return false;
		}             // �յ�ȫ������  
	}

	do 
	{
		if (!(( bool )receive_cmd( cReceiveCache, 4, 100 )))
		{
			continue;
		}
		else
		{
			if( cReceiveCache[2] == 0x00 )
			{
				free( pcSendCache );
				return true;
			}
		}
	} while (receive_times--);

	if (receive_times <= 0)
	{
		free( pcSendCache );
		return false;
	} 
	return true;

}


