// BBCV-Player.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CBBCVPlayerApp:
// �йش����ʵ�֣������ BBCV-Player.cpp
//

class CBBCVPlayerApp : public CWinApp
{
public:
	CBBCVPlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBBCVPlayerApp theApp;
