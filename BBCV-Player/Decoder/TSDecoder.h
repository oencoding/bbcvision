/*
	author yyd
		
*/

#ifndef __TSDECODER_H__
#define __TSDECODER_H__

#include <Windows.h>
typedef void TSDecoder_t;

typedef enum enVideoType
{
	CODE_HD_VIDEO = 1,
	CODE_SD_VIDEO
}VideoCodeType;

typedef enum enAuidoCodeType
{
	CODE_AUIDO_MP2=1,
	CODE_AUIDO_MP3,
	CODE_AUDIO_AAC
}AuidoCodeType;


typedef enum
{
	tsUDP =1,
	tsRTSP = 2,
	tsLocalFile =3,
	othertype
}URLTYPE;

typedef struct _DecodeParam
{
	bool bNeedLog;				//��Ҫ��־
	bool bNeedControlPlay;		//���Ʋ���
	bool bSmooth;				//ƽ������
	bool bAutoMatch;			//�Զ�ƥ��
	bool bSaveVideoData;		//������Ƶ����
	bool bDelayCheckModel;		//��ʱ������
	VideoCodeType vCodetype;    //��Ƶ����
	AuidoCodeType aCodetype;	//��Ƶ����
	char strDstIP[128];			//Ŀ��ip
	int iport;					//Ŀ�Ķ˿�

}DecoderControlParam;

//<summary>
//<param cfilename> �ļ���
//<param iWidht>  ָ�����YUV��widht
//<param iHight> ָ�����YUV��height
TSDecoder_t* init_TS_Decoder(const char* cfilename,HWND hwnd,DecoderControlParam dcParam);

//<summary>
//<param cfilename> �ļ���
//<param iWidht>  ָ�����YUV��widht
//<param iHight> ָ�����YUV��height
TSDecoder_t* init_TS_Decoder(const char* cfilename,HWND hwnd, bool bflags,bool bSmooth,bool bNeedControlPlay=false, int iRTPServerPort=0,VideoCodeType vCodetype=CODE_HD_VIDEO,AuidoCodeType aCodeType=CODE_AUIDO_MP2,
									const char* strDstIP="192.168.60.246",int iPort=10000);

//<summary>
//<param ts_decoder>  instance
//<param pWidth pHeight> OUT ���YUV�Ŀ�͸�
int get_Video_Param(TSDecoder_t *ts_decoder,int *pWidth,int *pHeight); 


//<summary>
//<param ts_decoder>  instance
//<param output_video_yuv420>  OUT ���YUV420����
//<param output_video_size> OUT IN ����output_video_yuv420��������С�� ���output_video_yuv420ʵ�ʳ���
//<param pWidth pHeight> OUT ���YUV�Ŀ�͸�
//<param video_pts> OUT video PTS

int get_Video_data(TSDecoder_t *ts_decoder,unsigned char *output_video_yuv420,int *output_video_size,
	int *pWidth,int *pHeight,unsigned long *video_pts=0); 

//<summary>
//<param ts_decoder>  instance
//<param output_audio_data>  OUT �����Ƶ����
//<param output_audio_size> OUT IN ����output_video_data��������С�� ���output_audio_dataʵ�ʳ���
//<param audio_pts> OUT �����Ƶ���ݵ�PTS

int get_Audio_data(TSDecoder_t *ts_decoder,unsigned char *output_audio_data,int* input_audio_size,
	unsigned long* audio_pts);

//<summary> ���ý��뿪�أ�true Ϊ�Զ�ƥ�䣬FALSEΪָ������
bool Set_tsDecoder_stat(TSDecoder_t *ts_decoder,bool bStart);

//������������
bool Set_tsRate_period(TSDecoder_t *ts_decoder,int iperiod);

//��ȡ������
bool Get_tsRate(TSDecoder_t *ts_decoder,int* iRate);
//������ʱ
bool Set_tsTime_delay(TSDecoder_t *ts_decoder,int begintime,int* relsutTime);

bool Get_tsIFrame_size(TSDecoder_t *ts_decoder,int* iSize);


bool Get_tsDecoder_sem(TSDecoder_t *ts_decoder,void **pSem);

bool Set_tsDecoder_Volume(TSDecoder_t *ts_decoder,int iVolume);

bool Get_tsDecoder_Volume(TSDecoder_t *ts_decoder,int &iVolume);

bool Set_tsDecoder_SaveStream(TSDecoder_t *ts_decoder,int bsave);

int uninit_TS_Decoder(TSDecoder_t *audioencoder);

#endif

