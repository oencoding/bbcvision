#ifndef _AMPDISPLAYSDK_H_
#define _AMPDISPLAYSDK_H_

#pragma once
#include <Windows.h>
#include <string>

//typedef struct 
//{
//	std::string stringByName1;
//	std::string stringByName2;
//	std::string stringByName3;
//	std::string stringByName4;
//	int			iFontProperity;
//}SubTitleProp;

//#ifdef AMPDISPLAYSDK_EXPORTS
#if defined _WINDLL
#define AMPDISPLAYSDK_API extern "C"  _declspec(dllexport)
#else
#define AMPDISPLAYSDK_API extern "C"  _declspec(dllimport)
#endif

#define AMP_PLAY_MAX_SUPPORTS 128

#define AMP_PLAY_NOERROR 0
#define AMP_PLAY_NOINIT	1						//����Ѵ��ڣ���Ϊ��0
#define AMP_PLAY_HANDLE_ALREADY_USED	2		//��Handle�Ѿ���ʹ��
#define AMP_PLAY_PARA_ERROR				3		//��������
#define AMP_PLAY_ORDER_ERROR			4		//����˳�����
#define AMP_PLAY_HEADERBUF_ERROR		5		//����ͷ����
#define AMP_PLAY_OVER_CHANNEL			6		//���������֧��·��
#define AMP_PLAY_OPENFILE_ERROR			7		//���ļ�ʧ��
#define AMP_PLAY_UNKNOWN				8		//δ֪����
#define AMP_PLAY_OBJNOTFIND				9		//���󲻴���

//<summary> ��ȡ���ž��
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_GetHandle(LONG &lHandle);

//<summary> �ͷŲ��ž��
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_FreeHandle(LONG lHandle);

//<summary> ��ʼ����ʾ���󣬰󶨴���
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_InitDisplay(LONG lHandle,HWND hWnd,int iAdpterNum=0);

//<summary> �ͷ���ʾ����
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_FreeDisplay(LONG lHandle);

//<summary> ������ʾ����
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetParam(LONG lHandle,long lWidth,long lHeight);

//<summary> ���³�ʼ����ʾ����
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_reInitDisplay(LONG lHandle,HWND hWnd,int iAdpterNum);

//<summary> ������Ļ��Ϣ
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetSubTitleProp(LONG lHandle,const char* subTitleProperity,int iLen,unsigned char iFontProp);

//<summary> ��ȡ���Ų�����Ϣ
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_GetDisplayParam(LONG lHandle,int &iWidth,int &iHeight);

//<summary> ��ʾYUVͼ��
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_DisplayYUV(LONG lHandle,unsigned char * pOutBuffer, void *csLockYUV);

//<summary> ֹͣ��ʾͼ��
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_StopDisplay(LONG lHandle);

//<summary> ��ʾͼ�����
//<param name= "lHandle"> ���ž�� /IN</param>
//<param name= "bVideoScale"> /IN FalseΪ���ڱ�����TrueΪ��Ƶͼ�������
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetScale(LONG lHandle,bool bVideoScale);

//<summary> ������Ļ��ʾ��Χ
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetSubtitleScope(LONG lHandle,int xLeft,int yTop,int xRight,int yBottom);

#endif