#include ".\audioplay.h"
#include <math.h>
#include <stdio.h>

AudioPlay::AudioPlay(void)
{
	m_bEnableSound = false;
	m_bPlayFlag = false;
	m_dwNextPlayOffset = 0;
	m_iSetBufferCount = 0;
	m_wVolume = 0xFFFF;
	m_lpDS = NULL;
	m_lpDSB = NULL;
	InitializeCriticalSection(&m_csDSound);
}

AudioPlay::~AudioPlay(void)
{
	DeleteCriticalSection(&m_csDSound);
}

// <summary>��ʼ��DirectSound</summary >
// <param name=" hWnd ">���Ŵ��ھ�� / IN</param>
// <returns>true��ʾ�ɹ���false��ʾʧ��</returns>
bool AudioPlay::InitDSound(HWND hWnd,DWORD dwSamplesPerSec)
{
	EnterCriticalSection(&m_csDSound);
	HRESULT hRet = DirectSoundCreate8(NULL, &m_lpDS, NULL);
	if(hRet != DS_OK)
	{
		LeaveCriticalSection(&m_csDSound);
		m_lpDS = NULL;
		m_lpDSB = NULL;
		return false;
	}

	hRet = m_lpDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(hRet != DS_OK)
	{
		LeaveCriticalSection(&m_csDSound);
		m_lpDS = NULL;
		m_lpDSB = NULL;
		return false;
	}

	WAVEFORMATEX wfx;
	DSBUFFERDESC dsbdesc;
	// Set up WAV format structure.
	memset(&wfx, 0, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = dwSamplesPerSec;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels * ( wfx.wBitsPerSample / 8 );
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	// Set up DSBUFFERDESC structure.
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
	// ��buffer
	dsbdesc.dwBufferBytes = wfx.nAvgBytesPerSec;  //
	dsbdesc.lpwfxFormat = &wfx;


	LPDIRECTSOUNDBUFFER lpDSB=NULL;
	hRet = m_lpDS->CreateSoundBuffer(&dsbdesc, &lpDSB, NULL);
	if(hRet != DS_OK)
	{
		LeaveCriticalSection(&m_csDSound);
		m_lpDS = NULL;
		m_lpDSB = NULL;
		return false;
	}


	m_dwPlayBufferSize = dsbdesc.dwBufferBytes;
	hRet = lpDSB->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_lpDSB);
	if(hRet != DS_OK)
	{
	LeaveCriticalSection(&m_csDSound);
	m_lpDS = NULL;
	m_lpDSB = NULL;
	return false;
	}
	lpDSB->Release();

	LeaveCriticalSection(&m_csDSound);
	return true;

/*	// Get the primary buffer 
	DSBUFFERDESC dsbd;
	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat   = NULL;
	
	LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;
	hRet = m_lpDS->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL);
	if(hRet != DS_OK)
	{
		LeaveCriticalSection(&m_csDSound);
		m_lpDS = NULL;
		m_lpDSB = NULL;
		return false;
	}

	WAVEFORMATEX wfx;
	ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
	wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM; 
	wfx.nChannels       = (WORD) 2; 
	wfx.nSamplesPerSec  = (DWORD) dwSamplesPerSec; 
	wfx.wBitsPerSample  = (WORD) 16; 
	wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
	wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

	if( FAILED( hRet = pDSBPrimary->SetFormat(&wfx) ) )
	{
		LeaveCriticalSection(&m_csDSound);
		m_lpDS = NULL;
		m_lpDSB = NULL;
		return false;
	}

	pDSBPrimary->Release();

	LeaveCriticalSection(&m_csDSound);
	return true;
*/


}

// <summary>����PCM����</summary >
// <param name=" pBuffer ">PCM���� / IN</param>
// <param name=" iDataLength ">���� / IN</param>
// <returns>��</returns>
void AudioPlay::PlayData(void * pBuffer, int iDataLength)
{
	if(m_bPlayFlag && m_lpDSB != NULL)
	{
		EnterCriticalSection(&m_csDSound);
		HRESULT hRet;
		VOID* pbPlayData = NULL;
		DWORD dwPlayLength;
		VOID* pbPlayData2 = NULL;
		DWORD dwPlayLength2;

		hRet = m_lpDSB->Lock(m_dwNextPlayOffset, iDataLength, &pbPlayData, &dwPlayLength, &pbPlayData2, &dwPlayLength2, 0L);
		if(hRet != DS_OK)
		{
			LeaveCriticalSection(&m_csDSound);
			return;
		}

		memcpy(pbPlayData, pBuffer, dwPlayLength);
		m_dwNextPlayOffset += dwPlayLength;
		m_dwNextPlayOffset %= m_dwPlayBufferSize;
		if(pbPlayData2 != NULL)
		{
			memcpy(pbPlayData2, (char *)pBuffer + dwPlayLength, dwPlayLength2);
			m_dwNextPlayOffset += dwPlayLength2;
			m_dwNextPlayOffset %= m_dwPlayBufferSize;
		}
		hRet = m_lpDSB->Unlock(pbPlayData, dwPlayLength, pbPlayData2, dwPlayLength2);
		if(hRet != DS_OK)
		{
			LeaveCriticalSection(&m_csDSound);
			return;
		}
		m_iSetBufferCount++;
		if(m_iSetBufferCount == 5)
		{
			hRet = m_lpDSB->Play(0, 0, DSBPLAY_LOOPING);//DSBPLAY_LOOPING
			if(hRet != DS_OK)
			{
				LeaveCriticalSection(&m_csDSound);
				return;
			}
			m_bPlayFlag = true;
		}
		LeaveCriticalSection(&m_csDSound);
	}
}

// <summary>ֹͣ����</summary >
// <returns>��</returns>
void AudioPlay::StopPlay()
{
	if(m_bPlayFlag && m_lpDSB != NULL)
	{
		m_lpDSB->Stop();
		m_bPlayFlag = false;
	}
	m_bEnableSound = false;
	m_iSetBufferCount = 0;
	m_dwNextPlayOffset = 0;
}

// <summary>��ʼ����</summary >
// <returns>��</returns>
void AudioPlay::StartPlay()
{
	int iTime = 20;
	while (m_lpDSB == NULL && iTime > 0)
	{
		Sleep(5);
		iTime--;
	}
	m_bEnableSound = true;
	m_bPlayFlag = true;
	HRESULT hRet;
	EnterCriticalSection(&m_csDSound);
	if(m_lpDSB != NULL)
	{
		hRet = m_lpDSB->SetCurrentPosition(0);
	}
	LeaveCriticalSection(&m_csDSound);
}

// <summary>����PCM����</summary >
// <param name=" pBuffer ">PCM���� / IN</param>
// <param name=" iDataLength ">���� / IN</param>
// <returns>��</returns>
void AudioPlay::FreeDSound()
{
	EnterCriticalSection(&m_csDSound);
	if(m_lpDSB != NULL)
	{
		m_lpDSB->Release();
		m_lpDSB = NULL;
	}
	if(m_lpDS != NULL)
	{
		m_lpDS->Release();
		m_lpDS = NULL;
	}
	LeaveCriticalSection(&m_csDSound);
}

// <summary>��������</summary >
// <param name=" nVolume ">������ֵ����Χ0-0xFFFF / IN</param>
// <returns>true��ʾ�ɹ���false��ʾʧ��</returns>
bool AudioPlay::SetVolume(DWORD nVolume)
{
	if(nVolume < 0 || nVolume > 0xFFFF)
	{
		return false;
	}
	m_wVolume = nVolume;
	//����������������2��n�η�����
	double dLogVolume = (log((double)(nVolume + 1)) * 10000) / (log((double)2) * 16);
	HRESULT hRet;
	EnterCriticalSection(&m_csDSound);
	if(m_lpDSB != NULL)
	{
		hRet = m_lpDSB->SetVolume(dLogVolume - 10000);
	}
	else
	{
		//���m_lpDSB==NULLʱ��hRetû�г�ʼ�������ߵĻ�����������
		LeaveCriticalSection(&m_csDSound);
		return false;
	}
	LeaveCriticalSection(&m_csDSound);
	if(hRet != DS_OK)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// <summary>�������</summary >
// <param name=" nVolume ">������ֵ����Χ0-0xFFFF / OUT</param>
// <returns>true��ʾ�ɹ���false��ʾʧ��</returns>
bool AudioPlay::GetVolume(DWORD &nVolume)
{
	nVolume = m_wVolume;
	return true;
}
