#ifndef __TSSTREAMINFO_H_
#define __TSSTREAMINFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>


#include <map>

//typedef unsigned char uint8_t;

//typedef unsigned char uint8_t;

//typedef unsigned long long uint64_t;



typedef std::map<int,int> MapPIDStreamType;


typedef struct _TS_packet_Header
{
	unsigned sync_byte						:8;
	unsigned transport_error_indicator		:1;   //��������־λ��һ�㴫�����Ļ��Ͳ��ᴦ���������
	unsigned payload_unit_start_indicator	:1;   //��Ч���صĿ�ʼ��־�����ݺ�����Ч���ص����ݲ�ͬ����Ҳ��ͬ
	unsigned transport_prority				:1;   //�������ȼ�λ��1��ʾ�����ȼ�
	unsigned PID							:13;
	unsigned transport_scrambling_control	:2;   //���ܱ�־λ,00��ʾδ����
	unsigned adaption_field_control			:2;   //�����ֶο���,��01������Ч���أ�10���������ֶΣ�11���е����ֶκ���Ч���ء�Ϊ00�Ļ������������д���
	unsigned continuity_counter				:4;    //һ��4bit�ļ���������Χ0-15

}TS_packet_Header;

typedef struct _TS_PAT
{
	unsigned table_id					:8;
	unsigned section_syntax_indicator	:1;
	unsigned zero						:1;
	unsigned reserved_1					:2;
	unsigned section_length				:12;
	unsigned transport_stream_id		:16;
	unsigned reserved_2					:2;
	unsigned version_number				:5;
	unsigned current_next_indicator		:1;
	unsigned section_number				:8;
	unsigned last_section_number		:8;
	unsigned program_number				:16;
	unsigned reserved_3					:3;
	unsigned network_PID				:13;
	unsigned program_map_PID			:13;
	unsigned CRC_32						:32;

}TS_PAT;


typedef struct _TS_PMT
{
	unsigned table_id					:8;
	unsigned section_syntax_indicator	:1;
	unsigned zero						:1;
	unsigned reserved_1					:2;
	unsigned section_length				:12;
	unsigned program_number				:16;
	unsigned reserved_2					:2;
	unsigned version_number				:5;
	unsigned current_next_indicator		:1;
	unsigned section_number				:8;
	unsigned last_section_number		:8;
	unsigned reserved_3					:3;
	unsigned PCR_PID					:13;
	unsigned reserved_4					:4;
	unsigned program_info_length		:12;
	unsigned stream_type				:8;
	unsigned reserved_5					:3;
	unsigned elementary_PID				:13;
	unsigned reserved_6					:4;
	unsigned ES_info_length				:12;
	unsigned CRC_32						:32;

}TS_PMT;

typedef struct _RTPHead
{
	unsigned Version					:2;
	unsigned PayloadFlag				:1;
	unsigned ExternData					:1;
	unsigned CSRCCount					:4;
	unsigned MarkFlag					:1;
	unsigned PayloadType				:7;
	unsigned Sequence					:16;
	unsigned Timestamp					:32;
	unsigned SSRCInfo					:32;
	//unsigned CSRCList				
}RTPHead;


typedef struct _TS_PES_PACKET
{

}TS_PES_PACKET;

//pmt ��stream_type ������
#define   STREAMTYPE_UNKNOWN                 0x01

#define   STREAMTYPE_11172_VIDEO                  0x01
#define   STREAMTYPE_13818_VIDEO                  0x02   //mpeg 2
#define   STREAMTYPE_11172_AUDIO                  0x03
#define   STREAMTYPE_13818_AUDIO                  0x04
#define   STREAMTYPE_13818_PRIVATE                0x05
#define   STREAMTYPE_13818_PES_PRIVATE            0x06
#define   STREAMTYPE_13522_MHPEG                  0x07
#define   STREAMTYPE_13818_DSMCC                  0x08
#define   STREAMTYPE_ITU_222_1                    0x09
#define   STREAMTYPE_13818_A                      0x0a
#define   STREAMTYPE_13818_B                      0x0b
#define   STREAMTYPE_13818_C                      0x0c
#define   STREAMTYPE_13818_D                      0x0d
#define   STREAMTYPE_13818_AUX                    0x0e
#define   STREAMTYPE_AAC_AUDIO     0x0f
#define   STREAMTYPE_MPEG4_AUDIO     0x11
#define   STREAMTYPE_H264_VIDEO     0x1b
#define   STREAMTYPE_AVS_VIDEO     0x42
#define   STREAMTYPE_AC3_AUDIO                    0x81
#define   STREAMTYPE_DTS_AUDIO                    0x82




class TSstreamInfo
{
public:
	TSstreamInfo();
	~TSstreamInfo();

	bool GetVideoESInfo(unsigned char *pEsData,int itempLen);

	void Adjust_PMT_table(TS_PMT* packet ,unsigned char *buffer);
	int Adjust_PAT_table(TS_PAT* packet ,unsigned char *buffer);
	void Adjust_TS_packet_header(TS_packet_Header* pHeader,unsigned char *buffer);

	bool Adjust_PES_Pakcet(unsigned char *buffer,int ilen);
	int ParseStreamInfo(uint8_t *buff,int ilen);
	int RecordVideoInfo();
	void get_time(char*buff,int siz);

	int ParseStreamFrame(uint8_t *buff,int ilen);

	uint64_t Parse_PTS(unsigned char *pBuf);
	
	bool ParseH264ES(unsigned char* buffer,int itemplen);

	int64_t Get_PCR_Value(unsigned char* buffer,int itemplen);
	int Set_PCR_Value(unsigned char* buffer,int index,int64_t pcr_value);
	int Find_PCR_Index(unsigned char* buffer,int itemplen,int *iIndex);

	bool Find_Stream_IFrame(unsigned char *buffer,int ilen,int* pIndex);

	bool Find_IFrame(unsigned char *buffer,int ilen);

	int m_iPMTPID;
	int m_iVideoPID;
	int m_iAudioPID;
	int m_iServerPID;
	int m_iPCRPID;

	//pthread_cond_t cond;
//	sem_t m_sem_send;
	uint8_t* buf;
	int bufsize;
	int write_ptr;
	int read_ptr;
	int m_iport;
	int m_iSendPort;
	char m_cdstIP[256];
	bool m_boverlay;
	bool m_hsIDRFrame;
	bool m_bIsOverlaying;
	bool m_bInitDecoder;
	bool m_bDelayFrame;

	unsigned udp_recv_thread;
	bool m_bstop;

	//FILE *m_logfp;
	FILE *m_Mediafp;

	//FILE * �յ�����һ֡���ݵ�ʱ����Ϣ
	FILE *m_FramDatafp;

	//pes֡ͷ��ʶ
	bool m_bPesBeginFlag;

	bool m_bIFramebeginFlag;
	

	MapPIDStreamType m_mapStreamPID;

	int m_itsStreamPacketNumber;
	int m_iIframeSize;
	int m_iframesize;
	
	bool m_bHasPTS ;
	bool m_bHasDTS ;
	uint64_t m_llPCR;
	uint64_t m_llDts;
	uint64_t m_llPts;
	uint64_t m_llLastPts; //���ڼ���֡��

	uint64_t m_llFrameTotal;

	uint64_t m_llFrameNum;
	pthread_mutex_t m_mutexlocker;

	double m_iFrameRate;

	int m_iFramTotal;

	int m_iGopSize;
	//��Ƶ��������
	
	int m_iVideoCodeType;
	//��Ƶ��������
	int m_iAudioCodeType;

private:	

};


#endif


