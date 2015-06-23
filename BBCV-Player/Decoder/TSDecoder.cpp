#include "TSDecoder.h"
#include "Decoder.h"

//<summary>
//<param cfilename> �ļ���
//<param iWidht>  ָ�����YUV��widht
//<param iHight> ָ�����YUV��height
TSDecoder_t* init_TS_Decoder(const char* cfilename,HWND hwnd,DecoderControlParam dcParam)
{
	TSDecoder_Instance *p_instanse = new TSDecoder_Instance();
	//printf("init decode type video=%d audio=%d \n",vCodetype,aCodeType);
	if(p_instanse->init_TS_Decoder(cfilename,hwnd,dcParam) == -1)
	{
		//init failed
		printf("init_TS_DecoderFailed \n");
		return NULL;
	}

	return p_instanse;
}


TSDecoder_t* init_TS_Decoder(const char* cfilename,HWND hwnd,bool bflags,bool bSmooth,bool bNeedControlPlay,int iRTPServerPort,VideoCodeType vCodetype,AuidoCodeType aCodeType,const char* strDstIP,int iPort)
{
	TSDecoder_Instance *p_instanse = new TSDecoder_Instance();
	//printf("init decode type video=%d audio=%d \n",vCodetype,aCodeType);
	if(p_instanse->init_TS_Decoder(cfilename,hwnd,bflags,bSmooth,bNeedControlPlay,iRTPServerPort,vCodetype,aCodeType,strDstIP,iPort) == -1)
	{
		//init failed
		printf("init_TS_DecoderFailed \n");
		return NULL;
	}

	return p_instanse;
}

//<summary>
//<param ts_decoder>  instance
//<param pWidth pHeight> OUT ���YUV�Ŀ��͸�
int get_Video_Param(TSDecoder_t *ts_decoder,int *pWidth,int *pHeight)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	int ret = p_instance->get_video_param(pWidth,pHeight);
	return ret;
	
}


//<summary>
//<param ts_decoder>  instance
//<param output_video_yuv420>  OUT ���YUV420����
//<param output_video_size> OUT IN ����output_video_yuv420��������С�� ���output_video_yuv420ʵ�ʳ���
//

int get_Video_data(TSDecoder_t *ts_decoder,unsigned char *output_video_yuv420,int *output_video_size,
	int *pWidth,int *pHeight,unsigned long *video_pts)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	int ret = p_instance->get_Video_data(output_video_yuv420,output_video_size, pWidth,pHeight,video_pts);
		
	return ret;
}

//<summary>
//<param ts_decoder>  instance
//<param output_audio_data>  OUT �����Ƶ����
//<param output_audio_size> OUT IN ����output_video_data��������С�� ���output_audio_dataʵ�ʳ���
//<param audio_pts> OUT �����Ƶ���ݵ�PTS

int get_Audio_data(TSDecoder_t *ts_decoder,unsigned char *output_audio_data,int* input_audio_size,
	unsigned long* audio_pts)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	int ret = p_instance->get_Audio_data(output_audio_data,input_audio_size,audio_pts);
		
	return ret;

}

bool Set_tsDecoder_stat(TSDecoder_t *ts_decoder,bool bStart)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	return p_instance->set_tsDecoder_stat(bStart);
		
}

bool Get_tsDecoder_sem(TSDecoder_t *ts_decoder,void **pSem)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Get_tsDecoder_sem(pSem);
}

bool Set_tsDecoder_Volume(TSDecoder_t *ts_decoder,int iVolume)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Set_tsDecoder_Volume(iVolume);
}

bool Get_tsDecoder_Volume(TSDecoder_t *ts_decoder,int &iVolume)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Get_tsDecoder_Volume(iVolume);

}


int uninit_TS_Decoder(TSDecoder_t *tsdecoder)
{
	if(tsdecoder == NULL)
	{		 
		printf("libtssmooth: Error paraments..\n");		
		return -1;	  
	}

	TSDecoder_Instance* p_instance = (TSDecoder_Instance* )tsdecoder;
	delete p_instance;
	p_instance = NULL;
	return 0;
}


//������������
bool Set_tsRate_period(TSDecoder_t *ts_decoder,int iperiod)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Set_tsRate_period(iperiod);
}

//��ȡ������
bool Get_tsRate(TSDecoder_t *ts_decoder,int* iRate)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Get_tsRate(iRate);
}
//������ʱ
bool Set_tsTime_delay(TSDecoder_t *ts_decoder,int begintime,int* relsutTime)
{

	return true;
}

bool Get_tsIFrame_size(TSDecoder_t *ts_decoder,int* iSize)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Get_tsIFrame_size(iSize);
}

bool Set_tsDecoder_SaveStream(TSDecoder_t *ts_decoder,int isave)
{
	TSDecoder_Instance* p_instance = (TSDecoder_Instance*)ts_decoder;
	if(NULL == p_instance) return false;
	return p_instance->Set_tsDecoder_SaveStream(isave);
}

