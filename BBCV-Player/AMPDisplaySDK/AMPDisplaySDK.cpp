#include "AMPDisplaySDK.h"
#include "D3D9VideoPlayer.h"
#include <conio.h>

LONG g_dwError = AMP_PLAY_NOERROR;

//bool __stdcall APIENTRY DllMain( HANDLE hModule, 
//					  DWORD  ul_reason_for_call, 
//					  LPVOID lpReserved
//					  )
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//
//}
static CD3DDisplay *pPlayChannel[AMP_PLAY_MAX_SUPPORTS+1] = {0};
static LONG st_iCurrentObjNumber = 1;

//<summary> ��ȡ���ž��
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_GetHandle(LONG &lHandle)
{

	for(int i=1; i<=AMP_PLAY_MAX_SUPPORTS; i++)
	{
		CD3DDisplay *pChannel = pPlayChannel[i];

		if(pChannel == NULL)
		{
			pPlayChannel[i] = new CD3DDisplay();

			lHandle = i;
#ifdef _DEBUG
				_cprintf(" *************return port %d \n",lHandle);
#endif
			return TRUE;
		}			
	}
	g_dwError = AMP_PLAY_OVER_CHANNEL;
	return FALSE;

}

//<summary> �ͷŲ��ž��
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_FreeHandle(LONG lHandle)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
#ifdef _DEBUG
			_cprintf(" *************free port %d \n",lHandle);
#endif

			delete pChannel;
			pPlayChannel[lHandle] = NULL;
			return TRUE;
		}
	}

	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;

}

//<summary> ��ʼ����ʾ���󣬰󶨴���
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_InitDisplay(LONG lHandle,HWND hWnd,int iAdpterNum)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->InitDraw(hWnd,iAdpterNum);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;
}

//<summary> ������ʾ����
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetParam(LONG lHandle,long lWidth,long lHeight)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->SetVideoParam(lWidth,lHeight);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;
}

//<summary> ���³�ʼ����ʾ����
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_reInitDisplay(LONG lHandle,HWND hWnd,int iAdpterNum)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->ReInitDraw(hWnd,iAdpterNum);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;

}

//<summary> ������Ļ��Ϣ
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall  AMP_Display_SetSubTitleProp(LONG lHandle,const char* subTitle,int iLen,unsigned char iFontProp)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->SetSubTitleProp(iFontProp,subTitle,iLen);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;
}

//<summary> ��ȡ���Ų�����Ϣ
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_GetDisplayParam(LONG lHandle,int &iWidth,int &iHeight)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->GetVideoParam(&iWidth,&iHeight);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;
}


//<summary> ��ʾYUVͼ��
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_DisplayYUV(LONG lHandle,unsigned char * pOutBuffer, void *csLockYUV)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->SetOutBuffer(pOutBuffer,(CRITICAL_SECTION*)csLockYUV);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;

}

//<summary> ��ʼ����ʾ���󣬰󶨴���
//<param >
//<return> trueΪ�ɹ���false ʧ��
AMPDISPLAYSDK_API bool __stdcall AMP_Display_FreeDisplay(LONG lHandle)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->FreeDraw();
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;

}

//<summary> ֹͣ��ʾͼ��
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_StopDisplay(LONG lHandle)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		 CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->StopPlay();
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;

}
//<summary> ֹͣ��ʾͼ��
//<param name= "lHandle"> ���ž�� /IN</param>
//<param name= "bVideoScale"> /IN FalseΪ���ڱ�����TrueΪ��Ƶͼ�������
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetScale(LONG lHandle,bool bVideoScale)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->SetDisplayScale(bVideoScale);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;
}
//<summary> ������Ļλ��
//<param name= "lHandle"> ���ž�� /IN</param>
//<returns>���سɹ� TRUE��ʧ�� FALSE
AMPDISPLAYSDK_API bool __stdcall AMP_Display_SetSubtitleScope(LONG lHandle,int xLeft,int yTop,int xRight,int yBottom)
{
	if (lHandle >= 1 && lHandle <= AMP_PLAY_MAX_SUPPORTS)
	{
		CD3DDisplay *pChannel = pPlayChannel[lHandle];
		if(pChannel != NULL)
		{
			pChannel->SetSubtitleScope(xLeft,yTop,xRight,yBottom);
			return true;
		}
		g_dwError = AMP_PLAY_OBJNOTFIND;
	}
	g_dwError = AMP_PLAY_PARA_ERROR;
	return FALSE;
}